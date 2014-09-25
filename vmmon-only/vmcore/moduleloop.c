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

/*
 * moduleloop.c --
 *
 *     Platform independent routines, private to VMCORE,  to
 *     support module calls and user calls in the module.
 *
 */

#if defined(linux)
/* Must come before any kernel header file */
#   include "driver-config.h"
#   include <linux/kernel.h>
#   include <linux/sched.h>
#endif
#include "vmware.h"
#include "modulecall.h"
#include "vmx86.h"
#include "task.h"
#include "initblock.h"
#include "vm_basic_asm.h"
#include "iocontrols.h"
#include "hostif.h"
#include "memtrack.h"
#include "driver_vmcore.h"
#include "usercalldefs.h"

/*
 *----------------------------------------------------------------------
 *
 * Vmx86_RunVM  --
 *
 *      Main interaction between the module and the monitor:
 *
 *      Run the monitor
 *      Process module calls from the monitor
 *      Make cross user calls to the main thread
 *      Return to userlevel to process normal user calls
 *      and to signal timeout or errors.
 *
 * Results:
 *      Positive: user call number.
 *      USERCALL_RESTART: (Linux only)
 *      USERCALL_VMX86ALLOCERR: error (message already output)
 *
 * Side effects:
 *      Not really, just a switch to monitor and back, that's all.
 *
 *----------------------------------------------------------------------
 */

int
Vmx86_RunVM(VMDriver *vm,   // IN:
            Vcpuid vcpuid)  // IN:
{
   uint32           retval    = MODULECALL_USERRETURN;
   VMCrossPageData *crosspage = &vm->crosspage[vcpuid]->crosspageData;
   int              bailValue = 0;

   ASSERT(crosspage);

   /*
    * Check if we were interrupted by signal.
    */
   if (crosspage->moduleCallInterrupted) {
      crosspage->moduleCallInterrupted = FALSE;
      goto skipTaskSwitch;
   }

   for (;;) {
      /*
       * Task_Switch changes the world to the monitor.
       * The monitor is waiting in the BackToHost routine.
       */
      UCTIMESTAMP(crosspage, SWITCHING_TO_MONITOR);
      Task_Switch(vm, vcpuid);
      UCTIMESTAMP(crosspage, SWITCHED_TO_MODULE);

      /*
       * Wake up anything that was waiting for this vcpu to run
       */

      if ((VCPUSet_IsEmpty(crosspage->yieldVCPUs) &&
          (crosspage->moduleCallType != MODULECALL_COSCHED)) ||
          (crosspage->moduleCallType == MODULECALL_SEMAWAIT)) {
         HostIF_WakeUpYielders(vm, vcpuid);
      }

      if (!VCPUSet_IsEmpty(crosspage->yieldVCPUs) &&
          (crosspage->moduleCallType != MODULECALL_COSCHED) &&
          (crosspage->moduleCallType != MODULECALL_SEMAWAIT)) {
         Vmx86_YieldToSet(vm, vcpuid, crosspage->yieldVCPUs, 0, TRUE);
      }

skipTaskSwitch:;

      retval = MODULECALL_USERRETURN;

      if (crosspage->userCallType != MODULECALL_USERCALL_NONE) {
         /*
          * This is the main user call path.  Handle by returning
          * from the ioctl (back to the userlevel side of a VCPU thread).
          */
         bailValue = crosspage->userCallType;
         crosspage->retval = retval;
         goto bailOut;
      }

      switch (crosspage->moduleCallType) {
      case MODULECALL_NONE:
         break;

      case MODULECALL_INTR:    // Already done in task.c
         break;

      case MODULECALL_GET_RECYCLED_PAGES: {
         MPN32 mpns[MODULECALL_NUM_ARGS];
         int nPages = MIN(crosspage->args[0], MODULECALL_NUM_ARGS);

         retval = Vmx86_AllocLockedPages(vm, PtrToVA64(mpns), nPages, TRUE,
                                         FALSE);
         if (retval <= nPages) {
            int i;
            for (i = 0; i < retval; i++) {
               crosspage->args[i] = mpns[i];
            }
         } else {
            // retval is holding an error code
            Warning("Failed to alloc %u pages: %d\n", nPages, (int)retval);
            retval = 0;
         }
         break;
      }

      case MODULECALL_SEMAWAIT: {
         retval = HostIF_SemaphoreWait(vm, vcpuid, crosspage->args);

         if (retval == MX_WAITINTERRUPTED) {
            crosspage->moduleCallInterrupted = TRUE;
            bailValue = USERCALL_RESTART;
            goto bailOut;
         }
         break;
      }

      case MODULECALL_SEMASIGNAL: {
         retval = HostIF_SemaphoreSignal(crosspage->args);

         if (retval == MX_WAITINTERRUPTED) {
             crosspage->moduleCallInterrupted = TRUE;
             bailValue = USERCALL_RESTART;
             goto bailOut;
         }
         break;
      }

      case MODULECALL_SEMAFORCEWAKEUP: {
         VCPUSet wakeVCPUs = (VCPUSet)QWORD(crosspage->args[0],
                                            crosspage->args[1]);
         FOR_EACH_VCPU_IN_SET(wakeVCPUs, v) {
            HostIF_SemaphoreForceWakeup(vm, v);
         } ROF_EACH_VCPU_IN_SET();
         break;
      }

      case MODULECALL_IPI: {
         HostIFIPIMode mode;
         VCPUSet vcpus = (VCPUSet)QWORD(crosspage->args[0], crosspage->args[1]);
         
         mode = HostIF_IPI(vm, vcpus, TRUE);
         retval = (mode != IPI_NONE);
         break;
      }

      case MODULECALL_RELEASE_ANON_PAGES: {
         unsigned count;
         MPN32 mpns[MODULECALL_NUM_ARGS];
         for (count = 0; count < MODULECALL_NUM_ARGS; count++) {
            if ((mpns[count] = (MPN32)crosspage->args[count]) == INVALID_MPN) {
               break;
            }
         }
         ASSERT(count > 0);
         retval = Vmx86_FreeLockedPages(vm, PtrToVA64(mpns), count, TRUE);
         break;
      }

      case MODULECALL_IS_ANON_PAGE: {
         MPN32 mpn = (MPN32)crosspage->args[0];
         retval = Vmx86_IsAnonPage(vm, mpn);
         break;
      }

      case MODULECALL_LOOKUP_MPN: {
         int i;
         VPN64 vpn = (VPN64)QWORD(crosspage->args[0], crosspage->args[1]);
         uint32 nPages = crosspage->args[2];
         VA64 uAddr = (VA64)VPN_2_VA(vpn);
         ASSERT(nPages <= MODULECALL_NUM_ARGS);
         for (i = 0; i < nPages; i++) {
            HostIF_LookupUserMPN(vm, uAddr + i * PAGE_SIZE,
                                 &crosspage->args[i]);
         }
         break;
      }

      case MODULECALL_COSCHED: {
         VCPUSet wakeVCPUs = (VCPUSet)QWORD(crosspage->args[0],
                                            crosspage->args[1]);
         uint32 spinUS     = (uint32)crosspage->args[2];

         Vmx86_YieldToSet(vm, vcpuid, wakeVCPUs, spinUS, FALSE);
         break;
      }

      case MODULECALL_START_VMX_OP: {
         int numVMCSes;
         int i;

         numVMCSes = crosspage->args[0];
         ASSERT(numVMCSes >= 0);
         ASSERT(numVMCSes <= MAX_DUMMY_VMCSES);
         for (i = 0; i < numVMCSes; i++) {
            MPN dummyVMCS = Task_GetDummyVMCS(i);

            if (dummyVMCS == INVALID_MPN) {
               bailValue = USERCALL_VMX86ALLOCERR;
               goto bailOut;
            }
            crosspage->dummyVMCS[i] = MPN_2_MA(dummyVMCS);
         }

         crosspage->inVMXOperation = 1;
         /*
          * PR 454299: Preserve previous crosspage->retval.
          */
         retval = crosspage->retval;
      } break;

      case MODULECALL_ALLOC_VMX_PAGE: {
         if (Task_GetRootVMCS(crosspage->pcpuNum) == INVALID_MPN) {
            crosspage->inVMXOperation = 0;

            bailValue = USERCALL_VMX86ALLOCERR;
            goto bailOut;
         }

         retval = crosspage->retval;
      } break;

      case MODULECALL_ALLOC_TMP_GDT: {
         if (!Task_GetTmpGDT(crosspage->pcpuNum)) {
            bailValue = USERCALL_VMX86ALLOCERR;
            goto bailOut;
         }

         retval = crosspage->retval;
      } break;

      default:
         Warning("ModuleCall %d not supported\n", crosspage->moduleCallType);
      }

      crosspage->retval = retval;

#if defined(linux)
      cond_resched(); // Other kernels are preemptable
#endif
   }

bailOut:
   return bailValue;
}
