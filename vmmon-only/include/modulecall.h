/*********************************************************
 * Copyright (C) 1998-2011 VMware, Inc. All rights reserved.
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
 * modulecall.h
 *
 *        Monitor <-->  Module (kernel driver) interface
 */

#ifndef _MODULECALL_H
#define _MODULECALL_H

#define INCLUDE_ALLOW_VMCORE
#define INCLUDE_ALLOW_VMMON
#include "includeCheck.h"

#include "x86types.h"
#include "x86desc.h"
#include "ptsc.h"
#include "vcpuid.h"
#include "vcpuset.h"
#include "vmm_constants.h"
#include "contextinfo.h"
#include "rateconv.h"
#include "modulecallstructs.h"
#include "mon_assert.h"

#define NUM_EXCEPTIONS 20       /* EXC_DE ... EXC_XF. */
 
#define MODULECALL_TABLE                                                      \
   MC(INTR)                                                                   \
   MC(SEMAWAIT)                                                               \
   MC(SEMASIGNAL)                                                             \
   MC(SEMAFORCEWAKEUP)                                                        \
   MC(IPI)          /* Hit thread with IPI. */                                \
   MC(USERRETURN)   /* Return codes for user calls. */                        \
   MC(GET_RECYCLED_PAGES)                                                     \
   MC(RELEASE_ANON_PAGES)                                                     \
   MC(IS_ANON_PAGE)                                                           \
   MC(LOOKUP_MPN)                                                             \
   MC(COSCHED)                                                                \
   MC(START_VMX_OP) /* Intel VT VMX operation. Not VMX userlevel process. */  \
   MC(ALLOC_VMX_PAGE)                                                         \
   MC(ALLOC_TMP_GDT)

/*
 *----------------------------------------------------------------------
 *
 * ModuleCallType --
 *
 *      Enumeration of support calls done by the module.
 *
 *      If anything changes in the enum, please update kstatModuleCallPtrs
 *      for stats purposes.
 *
 *----------------------------------------------------------------------
 */

typedef enum ModuleCallType {
   MODULECALL_NONE = 100,
#define MC(_modulecall) MODULECALL_##_modulecall,
   MODULECALL_TABLE
#undef MC
   MODULECALL_LAST                   // Number of entries. Must be the last one
} ModuleCallType;

#define MODULECALL_USER_START 300
#define MODULECALL_USER_END   400

#define MODULECALL_CROSS_PAGE_LEN    1
#define MODULECALL_CROSS_PAGE_START  6

#define MODULECALL_USERCALL_NONE     300

/*
 * Define VMX86_UCCOST in the makefiles (Local.mk,
 * typically) if you want a special build whose only purpose
 * is to measure the overhead of a user call and its
 * breakdown.
 *
 * WINDOWS NOTE: I don't know how to pass VMX86_UCCOST to
 * the driver build on Windows.  It must be defined by hand.
 *
 * ESX Note: we don't have a crosspage in which to store these
 * timestamps.  Such a feature would perhaps be nice (if we
 * ever tire of the argument that esx does so few usercalls
 * that speed doesn't matter).
 */

#if defined(VMX86_UCCOST) && !defined(VMX86_SERVER)
#define UCTIMESTAMP(cp, stamp) \
             do { (cp)->ucTimeStamps[UCCOST_ ## stamp] = RDTSC(); } while (0)
#else
#define UCTIMESTAMP(cp, stamp)
#endif

#ifdef VMX86_SERVER
typedef struct UCCostResults {
   uint32 vmksti;
   uint32 vmkcli;
   uint32 ucnop;
} UCCostResults;
#else

typedef struct UCCostResults {
   uint32 htom;
   uint32 mtoh;
   uint32 ucnop;
} UCCostResults;

typedef enum UCCostStamp {
#define UC(x) UCCOST_ ## x,
#include "uccostTable.h"
   UCCOST_MAX
} UCCostStamp;
#endif // VMX86_SERVER

/*
 * Header for the wsBody32.S and wsBody64.S worldswitch code files.
 * The wsBody32 or wsBody64 module loaded depends on the host bitsize.
 */
typedef struct WSModule {
   uint32 vmmonVersion;  // VMMON_VERSION when assembled as part of monitor
   uint16 moduleSize;    // size of whole wsBody{32,64} module
   uint16 hostToVmm;     // offset from beg of header to Host{32,64}toVmm
   uint16 vmm64ToHost;
   uint16 handleUD;
   uint16 handleGP;

   struct {              // offsets to patches (32-bit worldswitch only)
      uint16 ljmplma;
      uint16 ljmplmabig;
      uint16 va2pa;
      uint16 jump32Dest;
      uint16 pa2va;
   } ws32Patches;

   uint8 code[1024];      // big enough for MAX('.wsBody32', '.wsBody64',
                          //                    'wsBody32Log', 'wsBody64Log')
} WSModule;

typedef
#include "vmware_pack_begin.h"
struct SwitchNMIOffsets {
   uint16         db;    // offset to start of  #DB handler
   uint16         nmi;   // offset to start of #NMI handler
   uint16         df;    // offset to start of  #DF handler
   uint16         gp;    // offset to start of  #GP handler
   uint16         pf;    // offset to start of  #PF handler
   uint16         mce;   // offset to start of #MCE handler
}
#include "vmware_pack_end.h"
SwitchNMIOffsets;

/*
 * This is a header for the switchNMI.S module.  It contains code for
 * exceptions occurring during worldswitch.  The code gets copied to
 * the crosspage by initialization.
 */
typedef
#include "vmware_pack_begin.h"
struct SwitchNMI {                                // see switchNMI.S
   uint16           switchNMISize;
   SwitchNMIOffsets host32;                       // offsets to 32-bit handlers
   SwitchNMIOffsets host64;                       // offsets to 64-bit handlers
   volatile Bool    wsException[NUM_EXCEPTIONS];  // EXC_DE ... EXC_XF
                                                  // TRUE -> fault occurred in
                                                  //   worldswitch
   uint8            codeBlock[768];               // Enough for
                                                  //   max('.switchNMI',
                                                  //       '.switchNMILog').
}
#include "vmware_pack_end.h"
SwitchNMI;

#define SHADOW_DR(cpData, n)    (cpData)->shadowDR[n].ureg64


/*----------------------------------------------------------------------
 *
 * MAX_SWITCH_PT_PATCHES
 *
 *   This is the maximum number of patches that must be placed into
 *   the monitor page tables so that two pages of the host GDT and the
 *   crosspage can be accessed during worldswitch.
 *
 *----------------------------------------------------------------------
 */
#define MAX_SWITCH_PT_PATCHES 3

/*----------------------------------------------------------------------
 *
 * WS_NMI_STRESS
 *
 *   When set to non-zero, this causes the NMI-safe worldswitch code
 *   to be automatically stress tested by simulating NMIs arriving
 *   between various instructions.
 *
 *   When set to zero, normal worldswitch operation occurs.
 *
 *   See the worldswitch assembly code for details.
 *
 *----------------------------------------------------------------------
 */
#define WS_NMI_STRESS 0


/*----------------------------------------------------------------------
 *
 * VMM64PageTablePatch
 *
 *    Describes an entry in the monitor page table which needs to be
 *    patched during the back-to-host worldswitch.
 *
 *    o A patch can appear at any place in the page table, and so four
 *      items are required to uniquely describe the patch:
 *
 *      o level
 *
 *        This is the level in the page table to which the patch must
 *        be applied: L4, L3, L2, L1.  This information is used to
 *        determine the base of the region of memory which must be
 *        patched.  The level value corresponds to the following
 *        regions in monitor memory:
 *
 *          MMU_ROOT_64
 *          MMU_L3_64
 *          MMU_L2_64
 *          MON_PAGE_TABLE_64
 *
 *        The value zero is reserved to indicate an empty spot in the
 *        array of patches.
 *
 *      o level offset
 *
 *        The monitor memory regions corresponding to the page table
 *        levels may be more than one page in length, so a 'page
 *        offset' is required to know the starting address of the page
 *        table page which must be patched in 'level'.
 *
 *      o page index
 *
 *        The 'index' value specifies the element in the page which
 *        should be patched.
 *
 *      o pte
 *
 *        This is the PTE value which will be patched into the monitor
 *        page table.
 *
 *----------------------------------------------------------------------
 */
typedef
#include "vmware_pack_begin.h"
struct VMM64PageTablePatch {
#define PTP_EMPTY    (0U) /* Unused array entry. (must be 0) */
#define PTP_LEVEL_L1 (1U)       /* leaf level */
#define PTP_LEVEL_L2 (2U)
#define PTP_LEVEL_L3 (3U)
#define PTP_LEVEL_L4 (4U)       /* root level */
   uint16   level;              /* [0, 4]  (maximal size: 3 bits) */
   uint16   page;               /* Index of 'page' in 'level'.    */
   uint32   index;              /* Index of 'pte' in 'page'.      */
   VM_PDPTE pte;                /* PTE.                           */
}
#include "vmware_pack_end.h"
VMM64PageTablePatch;

#define MAX_DUMMY_VMCSES    16
#define MODULECALL_NUM_ARGS  8

/*
 *----------------------------------------------------------------------
 *
 * VMCrossPageData --
 *
 *      Data which is stored on the VMCrossPage.
 *
 *----------------------------------------------------------------------
 */
typedef
#include "vmware_pack_begin.h"
struct VMCrossPageData {
   /*
    * Tiny stack that is used during switching so it can remain valid.
    * It's good to keep the end 16-byte aligned for 64-bit processors.
    * There must be enough room for two interrupt frames (in case of
    * NMI during #DB handling), plus a half dozen registers.
    */
   uint32   tinyStack[46];

   uint64   hostCR4;            // host CR4 & ~CR4_PGE
   uint32   crosspageMA;

   uint8    hostDRSaved;        // Host DR spilled to hostDR[x].
   uint8    hostDRInHW;         // 0 -> shadowDR in h/w, 1 -> hostDR in h/w.
                                //   contains host-sized DB,NMI,MCE entries
   uint16   hostSS;
   uint64   hostSwitchCR3;
   uint64   hostRSP;
   uint64   hostCR3;
   uint64   hostDR[8];
   uint64   hostRBX;
   uint64   hostRSI;
   uint64   hostRDI;
   uint64   hostRBP;
   uint64   hostR12;
   uint64   hostR13;
   uint64   hostR14;
   uint64   hostR15;
   LA64     hostCrossPageLA;   // where host has crosspage mapped
   uint16   hostInitial32CS;
   uint16   hostInitial64CS;
   uint32   _pad0;

   DTR64    crossGDTHKLADesc;   // always uses host kernel linear address
   uint16   _pad1[3];
   DTR64    crossGDTMADesc;     // always uses machine address,
                                //   contains 32-bit DB, NMI, MCE entries
   uint16   _pad2[3];
   DTR64    mon64GDTR;
   uint16   mon64ES;
   uint16   mon64SS;
   uint16   mon64DS;
   uint64   mon64CR3;
   uint64   mon64RBX;
   uint64   mon64RSP;
   uint64   mon64RBP;
   uint64   mon64RSI;
   uint64   mon64RDI;
   uint64   mon64R12;
   uint64   mon64R13;
   uint64   mon64R14;
   uint64   mon64R15;
   uint64   mon64RIP;
   Task64   monTask64;          /* vmm64's task */

   FarPtr32 jump32Code;         // &worldswitch_64h_32v_mode_32compat
   uint16   _pad3;

   VMM64PageTablePatch vmm64PTP[MAX_SWITCH_PT_PATCHES]; /* page table patch */
   LA64                vmm64CrossPageLA;
   LA64                vmm64CrossGDTLA;   // where crossGDT mapped by PT patch
                                          //  32-bit host: machine address
                                          //  64-bit host: host kernel linear
                                          // address

   /*
    * The monitor may requests up to two actions when returning to the
    * host.  The moduleCallType field and args encode a request for
    * some action in the driver.  The userCallType field (together
    * with the RPC block) encodes a user call request.  The two
    * requests are independent.  The user call is executed first, with
    * the exception of MODULECALL_INTR which has a special effect.
    */
   ModuleCallType moduleCallType;
   uint32         args[MODULECALL_NUM_ARGS];
   uint32         retval;
   int            userCallType;
   uint32         pcpuNum;

   VCPUSet yieldVCPUs;

#if !defined(VMX86_SERVER)
   uint64 ucTimeStamps[UCCOST_MAX];
#endif

   /*
    * The values in the shadow debug registers must match those in the
    * hardware debug register immediately after a task switch in
    * either direction.  They are used to minimize moves to and from
    * the debug registers.
    */
   SharedUReg64     shadowDR[8];
   uint8            shadowDRInHW; // bit n set iff %DRn == shadowDR[n]

   SystemCallState  systemCall;
   uint8            _pad5[7];

   /*
    * Adjustment for machines where the hardware TSC does not run
    * constantly (laptops) or is out of sync between different PCPUs.
    * Updated as needed by vmmon.  See VMK_SharedData for the ESX
    * analog, which is updated by the vmkernel.
    */
   RateConv_ParamsVolatile pseudoTSCConv;
   VmAbsoluteTS            worldSwitchPTSC; // PTSC value immediately before
                                            // last worldswitch.

   /*
    * PTSC value of the next MonTimer callback for this vcpu.  When the
    * time arrives, if a target VCPU thread is in the monitor, it wants
    * to receive a hardware interrupt (e.g., an IPI) as soon as
    * possible; if it has called up to userlevel to halt, it wants to
    * wake up as soon as possible.
    */
   VmAbsoluteTS monTimerExpiry;


   /*
    * A VMXON page used to return to VMX operation when leaving the
    * monitor, if we had to leave VMX operation to enter the monitor.
    * These pages are allocated per-PCPU, and this field must contain
    * the VMXON page for the current PCPU when performing a
    * world-switch.
    */
   MA     rootVMCS;

   /*
    * Dummy VMCSes used to ensure that the cached state of a foreign VMCS
    * gets flushed to memory.
    */
   MA     dummyVMCS[MAX_DUMMY_VMCSES];

   /*
    * The current VMCS of a foreign hypervisor when we leave VMX
    * operation to disable paging.
    */
   MA     foreignVMCS;

   /*
    * Set if the host is in VMX operation and we need to disable
    * paging to switch between legacy mode and long mode.
    */
   uint32   inVMXOperation;

   Bool     retryWorldSwitch;   // TRUE -> return to host on host->vmm switch
   /*
    * TRUE if moduleCall was interrupted by signal. Only
    * vmmon uses this field to remember that it should
    * restart RunVM call, nobody else should look at it.
    */
   Bool   moduleCallInterrupted;
   uint16    _pad6;

   DTR64    switchHostIDTR;     // baseLA = switchHostIDT's host knl LA
   DTR64    switchMixIDTR;      // has baseLA = switchMixIDT's MA
                                //   contains 32-bit and 64-bit NMI, MCE entries
   DTR64    switchMon64IDTR;    // has baseLA = switchMon64IDT's monitor LA
                                //   contains 64-bit DB,NMI,MCE entries
   uint16   _pad7;

   /*
    * Descriptors and interrupt tables for switchNMI handlers.  Each
    * IDT has only enough space for the hardware exceptions; they are
    * sized to accommodate 32-bit & 64-bit descriptors.
    */
   uint8 switchHostIDT [sizeof(Gate64) * NUM_EXCEPTIONS]; // hostCS:hostVA
   uint8 switchMon64IDT[sizeof(Gate64) * NUM_EXCEPTIONS]; // 64-bit monCS:monVA
   uint8 switchMixIDT  [sizeof(Gate64) * NUM_EXCEPTIONS]; // CROSSGDT_*CS:MA
}
#include "vmware_pack_end.h"
VMCrossPageData;

/*
 *----------------------------------------------------------------------
 *
 * VMCrossPageCode --
 *
 *      Code which is stored on the VMCrossPage.
 *
 *----------------------------------------------------------------------
 */
typedef
#include "vmware_pack_begin.h"
struct VMCrossPageCode {
   WSModule   worldswitch;
   SwitchNMI  faultHandler;
}
#include "vmware_pack_end.h"
VMCrossPageCode;


/*
 *----------------------------------------------------------------------
 *
 * VMCrossPage --
 *
 *      Data structure shared between the monitor and the module
 *      that is used for crossing between the two.
 *      Accessible as vm->cross (kernel module) and CROSS_PAGE
 *      (monitor)
 *
 *      Exactly one page long
 *
 *----------------------------------------------------------------------
 */

typedef
#include "vmware_pack_begin.h"
struct VMCrossPage {
   uint32          version;         /* 4 bytes. Must be at offset zero. */
   uint32          crosspage_size;  /* 4 bytes. Must be at offset 4.    */
   VMCrossPageData crosspageData;
   uint8           _pad[PAGE_SIZE - (sizeof(uint32) /* version */        +
                                     sizeof(uint32) /* crosspage_size */ +
                                     sizeof(VMCrossPageData)             +
                                     sizeof(VMCrossPageCode))];
   VMCrossPageCode crosspageCode;
}
#include "vmware_pack_end.h"
VMCrossPage;

#define CROSSPAGE_VERSION_BASE 0xbea /* increment by 1 */
#define CROSSPAGE_VERSION    ((CROSSPAGE_VERSION_BASE << 1) + WS_NMI_STRESS)

#if !defined(VMX86_SERVER) && defined(VMM)
#define CROSS_PAGE  ((VMCrossPage * const) VPN_2_VA(CROSS_PAGE_START))
#define VMM_SWITCH_SHARED_DATA ((VMCrossPageData *)&CROSS_PAGE->crosspageData)
#endif

#define NULLPAGE_LINEAR_START  (MONITOR_LINEAR_START + \
                                PAGE_SIZE * CPL0_GUARD_PAGE_START)

#define MX_WAITINTERRUPTED     3
#define MX_WAITTIMEDOUT        2
#define MX_WAITNORMAL          1  // Must equal one; see linux module code.
#define MX_WAITERROR           0  // Use MX_ISWAITERROR() to test for error.

// Any zero or negative value denotes error.
#define MX_ISWAITERROR(e)      ((e) <= MX_WAITERROR)
#endif