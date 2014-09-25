/*********************************************************
 * Copyright (C) 1998 VMware, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2 and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 *********************************************************/

#ifdef linux
/* Must come before any kernel header file --hpreg */
#   include "driver-config.h"

#   include <linux/string.h>
#endif
#ifdef __APPLE__
#   include <string.h> // For strcmp().
#endif

#include "vmware.h"
#include "vm_assert.h"
#include "hostif.h"
#include "cpuid.h"
#include "x86cpuid_asm.h"

uint32 cpuidFeatures;
static CpuidVendor vendor = CPUID_NUM_VENDORS;
static uint32 version;


/*
 *-----------------------------------------------------------------------------
 *
 * CPUIDExtendedSupported --
 *
 *     Determine whether processor supports extended CPUID (0x8000xxxx)
 *     and how many of them.
 *
 * Results:
 *     0         if extended CPUID is not supported
 *     otherwise maximum extended CPUID supported (bit 31 set)
 *
 * Side effects:
 *     None.
 *
 *-----------------------------------------------------------------------------
 */

static uint32
CPUIDExtendedSupported(void)
{
   uint32 eax;

   eax = __GET_EAX_FROM_CPUID(0x80000000);
   if ((eax & 0x80000000) != 0x80000000) {
      return 0;
   }

   return eax;
}


void
CPUID_Init(void)
{
   CPUIDRegs regs;
   uint32 *ptr;
   char name[16];

   __GET_CPUID(1, &regs);
   version = regs.eax;
   cpuidFeatures = regs.edx;

   __GET_CPUID(0, &regs);
   ptr = (uint32 *)name;
   ptr[0] = regs.ebx;
   ptr[1] = regs.edx;
   ptr[2] = regs.ecx;
   ptr[3] = 0;

   if (strcmp(name, CPUID_INTEL_VENDOR_STRING_FIXED) == 0) {
      vendor = CPUID_VENDOR_INTEL;
   } else if (strcmp(name, CPUID_AMD_VENDOR_STRING_FIXED) == 0) {
      vendor = CPUID_VENDOR_AMD;
   } else if (strcmp(name, CPUID_CYRIX_VENDOR_STRING_FIXED) == 0) {
      vendor = CPUID_VENDOR_CYRIX;
   } else {
      Warning("VMMON CPUID: Unrecognized CPU\n");
      vendor = CPUID_VENDOR_UNKNOWN;
   }
}


CpuidVendor
CPUID_GetVendor(void)
{
   ASSERT(vendor != CPUID_NUM_VENDORS);
   return vendor;
}


uint32
CPUID_GetVersion(void)
{
   ASSERT(vendor != CPUID_NUM_VENDORS);
   return version;
}


/*
 *-----------------------------------------------------------------------------
 *
 * CPUID_SyscallSupported --
 *
 *     Determine whether processor supports syscall opcode and MSRs.
 *
 * Results:
 *     FALSE     if processor does not support syscall
 *     TRUE      if processor supports syscall
 *
 * Side effects:
 *     It determines value only on first call, caching it for future.
 *
 *-----------------------------------------------------------------------------
 */

Bool
CPUID_SyscallSupported(void)
{
   /*
    * It is OK to use local static variables here as 'result' does not depend
    * on any work done in CPUID_Init(). It purely depends on the CPU.
    */
   static Bool initialized = FALSE;
   static Bool result;

   if (UNLIKELY(!initialized)) {
      result =    CPUIDExtendedSupported() >= 0x80000001
               && (__GET_EDX_FROM_CPUID(0x80000001) & (1 << 11));
      initialized = TRUE;
   }

   return result;
}


Bool
CPUID_LongModeSupported(void)
{
   /*
    * It is OK to use local static variables here as 'result' does not depend
    * on any work done in CPUID_Init(). It purely depends on the CPU.
    */
   static Bool initialized = FALSE;
   static Bool result;

   if (UNLIKELY(!initialized)) {
      result =       CPUIDExtendedSupported() >= 0x80000001
                  && (__GET_EDX_FROM_CPUID(0x80000001) & (1 << 29));
      initialized = TRUE;
   }

   return result;
}


/*
 *-----------------------------------------------------------------------------
 *
 * CPUID_AddressSizeSupported --
 *
 *     Determine whether processor supports the address size cpuid
 *     extended leaf.
 *
 * Results:
 *     True iff the processor supports CPUID 0x80000008.
 *
 * Side effects:
 *     It determines value only on first call, caching it for future.
 *
 *-----------------------------------------------------------------------------
 */

Bool
CPUID_AddressSizeSupported(void)
{
   /*
    * It is OK to use local static variables here as 'result' does not depend
    * on any work done in CPUID_Init(). It purely depends on the CPU.
    */
   static Bool initialized = FALSE;
   static Bool result;

   if (UNLIKELY(!initialized)) {
      result = CPUIDExtendedSupported() >= 0x80000008;
      initialized = TRUE;
   }

   return result;
}


/*
 *----------------------------------------------------------------------
 *
 *  CPUID_HypervisorCPUIDSig --
 *
 *      Get the hypervisor signature string from CPUID.
 *
 * Results:
 *      TRUE on success FALSE otherwise.
 *      Unqualified 16 byte nul-terminated hypervisor string which may contain
 *      garbage.
 *
 * Side effects:
 *	None
 *
 *----------------------------------------------------------------------
 */

Bool
CPUID_HypervisorCPUIDSig(uint32 *name)
{
   CPUIDRegs regs;

   __GET_CPUID(1, &regs);
   if (!CPUID_ISSET(1, ECX, HYPERVISOR, regs.ecx)) {
      return FALSE;
   }

   regs.ebx = 0;
   regs.ecx = 0;
   regs.edx = 0;

   __GET_CPUID(0x40000000, &regs);

   if (regs.eax < 0x40000000) {
      Log(" CPUID hypervisor bit is set, but no "
          "hypervisor vendor signature is present\n");
      return FALSE;
   }

   name[0] = regs.ebx;
   name[1] = regs.ecx;
   name[2] = regs.edx;
   name[3] = 0;

   return TRUE;
}
