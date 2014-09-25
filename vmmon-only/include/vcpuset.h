/*********************************************************
 * Copyright (C) 2002-2011 VMware, Inc. All rights reserved.
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

/*
 * vcpuset.h --
 *
 *	ADT for a set of VCPUs. Currently implemented as a bitmask.
 */

#ifndef _VCPUSET_H_
#define _VCPUSET_H_


#define INCLUDE_ALLOW_VMX
#define INCLUDE_ALLOW_MODULE
#define INCLUDE_ALLOW_VMMON
#define INCLUDE_ALLOW_VMKERNEL
#define INCLUDE_ALLOW_USERLEVEL
#define INCLUDE_ALLOW_VMCORE
#include "includeCheck.h"

#include "vm_basic_asm.h"
#include "vm_atomic.h"
#include "vcpuid.h"

/*
 * If you update this type, you also need to update the SEND_IPI line in
 * bora/public/iocontrolsMacosTable.h.
 */
typedef uint64 VCPUSet;

#ifndef VMM
extern VCPUSet vcpusetFull;
#endif

#define FOR_EACH_VCPU_IN_SET(_vcpuSet, _v)                                    \
   do {                                                                       \
      Vcpuid  _v;                                                             \
      VCPUSet __vcs = (_vcpuSet);                                             \
      while ((_v = VCPUSet_FindFirst(__vcs)) != VCPUID_INVALID) {             \
         __vcs = VCPUSet_Remove(__vcs, _v);

#define ROF_EACH_VCPU_IN_SET()                                                \
      }                                                                       \
   } while (0)

static INLINE VCPUSet
VCPUSet_Empty(void)
{
   return 0;
}

static INLINE VCPUSet
VCPUSet_Singleton(Vcpuid v)
{
   ASSERT_ON_COMPILE(VCPUID_INVALID >= 64); // Ensure test below catches invalid VCPUs
   ASSERT(v < 64);                          // Shift by 64+ is undefined.
   return CONST64U(1) << v;
}

static INLINE VCPUSet
VCPUSet_SingletonChecked(Vcpuid v)
{
   return v == VCPUID_INVALID ? VCPUSet_Empty() : VCPUSet_Singleton(v);
}

static INLINE Bool
VCPUSet_IsSingleton(VCPUSet vcs)
{
   return vcs != 0 && (vcs & (vcs - 1)) == 0;
}

/*
 *----------------------------------------------------------------------
 * VCPUSet_FindFirst --
 *      
 *      First (least significant) Vcpuid in a set.
 *
 * Results:
 *      Vcpuid if at least one is present in a set.
 *      VCPUID_INVALID if the set is empty.
 *----------------------------------------------------------------------
 */

static INLINE Vcpuid
VCPUSet_FindFirst(VCPUSet vcs)
{
   ASSERT(VCPUID_INVALID == (Vcpuid)-1);
   return lssb64_0(vcs);
}

static INLINE Bool
VCPUSet_Equals(VCPUSet vcs1, VCPUSet vcs2)
{
   return vcs1 == vcs2;
}

static INLINE Bool
VCPUSet_IsEmpty(VCPUSet vcs)
{
   return VCPUSet_Equals(vcs, VCPUSet_Empty());
}

static INLINE VCPUSet
VCPUSet_Union(VCPUSet vcs1, VCPUSet vcs2)
{
   return vcs1 | vcs2;
}

static INLINE VCPUSet
VCPUSet_Intersection(VCPUSet s1, VCPUSet s2)
{
   return s1 & s2;
}

static INLINE VCPUSet
VCPUSet_Difference(VCPUSet s1, VCPUSet s2)
{
   return s1 & ~s2;
}

static INLINE VCPUSet
VCPUSet_Remove(VCPUSet vcs, Vcpuid v)
{
   return VCPUSet_Intersection(vcs, ~VCPUSet_Singleton(v));
}

static INLINE VCPUSet
VCPUSet_Include(VCPUSet vcs, Vcpuid v)
{
   return VCPUSet_Union(vcs, VCPUSet_Singleton(v));
}

static INLINE Bool
VCPUSet_IsMember(VCPUSet vcs, Vcpuid v)
{
   return !VCPUSet_IsEmpty(VCPUSet_Intersection(vcs, VCPUSet_Singleton(v)));
}

/*
 *----------------------------------------------------------------------
 * VCPUSet_IsSuperset --
 *    Returns true iff vcs1 contains a superset of the vcpus contained
 *    by vcs2.
 *----------------------------------------------------------------------
 */
static INLINE Bool
VCPUSet_IsSuperset(VCPUSet vcs1, VCPUSet vcs2)
{
   return !(vcs2 & ~vcs1);
}

static INLINE Bool
VCPUSet_IsSubset(VCPUSet vcs1, VCPUSet vcs2)
{
   return VCPUSet_IsSuperset(vcs2, vcs1);
}

static INLINE void
VCPUSet_AtomicInit(volatile VCPUSet *dst, VCPUSet vcs)
{
   Atomic_Write64(Atomic_VolatileToAtomic64(dst), vcs);
}

static INLINE VCPUSet
VCPUSet_AtomicReadWrite(volatile VCPUSet *dst, VCPUSet vcs)
{
   return Atomic_ReadWrite64(Atomic_VolatileToAtomic64(dst), vcs);
}

static INLINE VCPUSet
VCPUSet_AtomicRead(volatile VCPUSet *src)
{
   return Atomic_Read64(Atomic_VolatileToAtomic64(src));
}

static INLINE void
VCPUSet_AtomicUnion(volatile VCPUSet *dst, VCPUSet newset)
{
   Atomic_Or64(Atomic_VolatileToAtomic64(dst), newset);
}

static INLINE void
VCPUSet_AtomicDifference(volatile VCPUSet *dst, VCPUSet gone)
{
   Atomic_And64(Atomic_VolatileToAtomic64(dst), ~gone);
}

static INLINE void
VCPUSet_AtomicRemove(volatile VCPUSet *dst, Vcpuid v)
{
   Atomic_And64(Atomic_VolatileToAtomic64(dst), ~VCPUSet_Singleton(v));
}

static INLINE void
VCPUSet_AtomicInclude(volatile VCPUSet *dst, Vcpuid v)
{
   Atomic_Or64(Atomic_VolatileToAtomic64(dst), VCPUSet_Singleton(v));
}

static INLINE Bool
VCPUSet_AtomicIsMember(volatile VCPUSet *vcs, Vcpuid v)
{
   VCPUSet ivcs = Atomic_Read64(Atomic_VolatileToAtomic64(vcs));
   return VCPUSet_IsMember(ivcs, v);
}

static INLINE Bool
VCPUSet_AtomicIsEmpty(volatile VCPUSet *vcs)
{
   VCPUSet ivcs = Atomic_Read64(Atomic_VolatileToAtomic64(vcs));
   return VCPUSet_IsEmpty(ivcs);
}

/*
 *----------------------------------------------------------------------
 *
 * VCPUSet_Size --
 *
 *    Return the number of VCPUs in this set.
 *
 *----------------------------------------------------------------------
 */
static INLINE int
VCPUSet_Size(VCPUSet vcs)
{
   int     n = 0;
   while (vcs != 0) {
      vcs = vcs & (vcs - 1);
      n++;
   }
   return n;
}

/*
 *----------------------------------------------------------------------
 *
 * VCPUSet_Full --
 *
 *  Return the set representing all the VCPUs in the system.
 *
 *----------------------------------------------------------------------
 */
static INLINE VCPUSet
VCPUSet_Full(void)
{
#if defined(VMM)
   VCPUSet v;
   __asm__(".extern vcpusetFullAsAddr\n\t"
           "movabs $vcpusetFullAsAddr, %0" : "=r"(v));
   return v;
#else
#if defined(VMX)
   /*
    * Read too early, we may get the wrong notion of how many
    * vcpus the VM has. Cf. pr286243 and pr289186.
    */
   ASSERT(NumVCPUs() != 0 && vcpusetFull != 0);
#endif
   return vcpusetFull;
#endif
}

/*
 *----------------------------------------------------------------------
 *
 * VCPUSet_IsFull --
 *
 *  Returns true iff v contains the set of all vcpus.
 *
 *----------------------------------------------------------------------
 */
static INLINE Bool
VCPUSet_IsFull(VCPUSet v)
{
   return VCPUSet_Equals(v, VCPUSet_Full());
}

static INLINE Bool
VCPUSet_AtomicIsFull(volatile VCPUSet *vcs)
{
   VCPUSet ivcs = Atomic_Read64(Atomic_VolatileToAtomic64(vcs));
   return VCPUSet_IsFull(ivcs);
}

/*
 *----------------------------------------------------------------------
 *
 * VCPUSet_Mask --
 *
 *  Returns a vcpuset containing VCPUs [0, numVCPUs).
 *
 *----------------------------------------------------------------------
 */
static INLINE VCPUSet
VCPUSet_Mask(unsigned numVCPUs)
{
   /*
    * We cannot use (CONST64U(1) << numVCPUs) - 1 because the behavior
    * for numVCPUs = 64 is undefined in ANSI C.
    * The following works because the VM always has at least one VCPU.
    */
   ASSERT(numVCPUs > 0);
   return (CONST64U(2) << (numVCPUs - 1)) - 1;
}
#endif /* _VCPUSET_H_ */
