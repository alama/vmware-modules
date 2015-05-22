/*********************************************************
 * Copyright (C) 2004-2014 VMware, Inc. All rights reserved.
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

#ifndef _X86VT_H_
#define _X86VT_H_

#define INCLUDE_ALLOW_USERLEVEL
#define INCLUDE_ALLOW_VMX

#define INCLUDE_ALLOW_VMKERNEL
#define INCLUDE_ALLOW_MODULE
#define INCLUDE_ALLOW_DISTRIBUTE
#define INCLUDE_ALLOW_VMK_MODULE
#define INCLUDE_ALLOW_VMCORE
#define INCLUDE_ALLOW_VMMON
#include "includeCheck.h"

#include "community_source.h"
#include "x86msr.h"
#include "vm_basic_defs.h"
#if defined(USERLEVEL) || defined(MONITOR_APP)
#include "vm_basic_asm.h"
#else
#include "vm_asm.h"
#endif
#include "x86cpuid_asm.h"

/* VMX related MSRs */
#define MSR_VMX_BASIC                  0x00000480
#define MSR_VMX_PINBASED_CTLS          0x00000481
#define MSR_VMX_PROCBASED_CTLS         0x00000482
#define MSR_VMX_EXIT_CTLS              0x00000483
#define MSR_VMX_ENTRY_CTLS             0x00000484
#define MSR_VMX_MISC                   0x00000485
#define MSR_VMX_CR0_FIXED0             0x00000486
#define MSR_VMX_CR0_FIXED1             0x00000487
#define MSR_VMX_CR4_FIXED0             0x00000488
#define MSR_VMX_CR4_FIXED1             0x00000489
#define MSR_VMX_VMCS_ENUM              0x0000048a
#define MSR_VMX_2ND_CTLS               0x0000048b
#define MSR_VMX_EPT_VPID               0x0000048c
#define MSR_VMX_TRUE_PINBASED_CTLS     0x0000048d
#define MSR_VMX_TRUE_PROCBASED_CTLS    0x0000048e
#define MSR_VMX_TRUE_EXIT_CTLS         0x0000048f
#define MSR_VMX_TRUE_ENTRY_CTLS        0x00000490
#define MSR_VMX_VMFUNC                 0x00000491
#define NUM_VMX_MSRS                   (MSR_VMX_VMFUNC - MSR_VMX_BASIC + 1)

/*
 * An alias to accommodate Intel's naming convention in feature masks.
 */
#define MSR_VMX_PROCBASED_CTLS2        MSR_VMX_2ND_CTLS


/*
 * Single bit macros for backwards compatibility.  These can't be
 * defined as enumeration values using the VMXCAP macro, because
 * the Microsoft compiler doesn't like 64-bit enumeration values.
 */

#define MSR_VMX_BASIC_32BITPA                   \
   (CONST64U(1) << MSR_VMX_BASIC_32BITPA_SHIFT)
#define MSR_VMX_BASIC_DUALVMM                   \
   (CONST64U(1) << MSR_VMX_BASIC_DUALVMM_SHIFT)
#define MSR_VMX_BASIC_ADVANCED_IOINFO           \
   (CONST64U(1) << MSR_VMX_BASIC_ADVANCED_IOINFO_SHIFT)
#define MSR_VMX_BASIC_TRUE_CTLS                 \
   (CONST64U(1) << MSR_VMX_BASIC_TRUE_CTLS_SHIFT)
                                               
#define MSR_VMX_MISC_VMEXIT_SAVES_LMA           \
   (CONST64U(1) << MSR_VMX_MISC_VMEXIT_SAVES_LMA_SHIFT)
#define MSR_VMX_MISC_ACTSTATE_HLT               \
   (CONST64U(1) << MSR_VMX_MISC_ACTSTATE_HLT_SHIFT)
#define MSR_VMX_MISC_ACTSTATE_SHUTDOWN          \
   (CONST64U(1) << MSR_VMX_MISC_ACTSTATE_SHUTDOWN_SHIFT)
#define MSR_VMX_MISC_ACTSTATE_SIPI              \
   (CONST64U(1) << MSR_VMX_MISC_ACTSTATE_SIPI_SHIFT)
#define MSR_VMX_MISC_RDMSR_SMBASE_IN_SMM        \
   (CONST64U(1) << MSR_VMX_MISC_RDMSR_SMBASE_IN_SMM_SHIFT)
#define MSR_VMX_MISC_ALLOW_ALL_VMWRITES         \
   (CONST64U(1) << MSR_VMX_MISC_ALLOW_ALL_VMWRITES_SHIFT)


#define MSR_VMX_EPT_VPID_EPTE_X                 \
   (CONST64U(1) << MSR_VMX_EPT_VPID_EPTE_X_SHIFT)
#define MSR_VMX_EPT_VPID_GAW_48                 \
   (CONST64U(1) << MSR_VMX_EPT_VPID_GAW_48_SHIFT)
#define MSR_VMX_EPT_VPID_ETMT_UC                \
   (CONST64U(1) << MSR_VMX_EPT_VPID_ETMT_UC_SHIFT)
#define MSR_VMX_EPT_VPID_ETMT_WB                \
   (CONST64U(1) << MSR_VMX_EPT_VPID_ETMT_WB_SHIFT)
#define MSR_VMX_EPT_VPID_SP_2MB                 \
   (CONST64U(1) << MSR_VMX_EPT_VPID_SP_2MB_SHIFT)
#define MSR_VMX_EPT_VPID_SP_1GB                 \
   (CONST64U(1) << MSR_VMX_EPT_VPID_SP_1GB_SHIFT)
#define MSR_VMX_EPT_VPID_INVEPT                 \
   (CONST64U(1) << MSR_VMX_EPT_VPID_INVEPT_SHIFT)
#define MSR_VMX_EPT_VPID_ACCESS_DIRTY           \
   (CONST64U(1) << MSR_VMX_EPT_VPID_ACCESS_DIRTY_SHIFT)
#define MSR_VMX_EPT_VPID_INVEPT_EPT_CTX         \
   (CONST64U(1) << MSR_VMX_EPT_VPID_INVEPT_EPT_CTX_SHIFT)
#define MSR_VMX_EPT_VPID_INVEPT_GLOBAL          \
   (CONST64U(1) << MSR_VMX_EPT_VPID_INVEPT_GLOBAL_SHIFT)
#define MSR_VMX_EPT_VPID_INVVPID                \
   (CONST64U(1) << MSR_VMX_EPT_VPID_INVVPID_SHIFT)
#define MSR_VMX_EPT_VPID_INVVPID_ADDR           \
   (CONST64U(1) << MSR_VMX_EPT_VPID_INVVPID_ADDR_SHIFT)
#define MSR_VMX_EPT_VPID_INVVPID_VPID_CTX       \
   (CONST64U(1) << MSR_VMX_EPT_VPID_INVVPID_VPID_CTX_SHIFT)
#define MSR_VMX_EPT_VPID_INVVPID_ALL_CTX        \
   (CONST64U(1) << MSR_VMX_EPT_VPID_INVVPID_ALL_CTX_SHIFT)
#define MSR_VMX_EPT_VPID_INVVPID_VPID_CTX_LOCAL \
   (CONST64U(1) << MSR_VMX_EPT_VPID_INVVPID_VPID_CTX_LOCAL_SHIFT)

#define VT_VMCS_STANDARD_TAG           0x00000000
#define VT_VMCS_SHADOW_TAG             0x80000000

/*
 * Structure of VMCS Component Encoding.
 */
#define VT_ENCODING_ACCESS_HIGH        0x00000001
#define VT_ENCODING_INDEX_MASK         0x000003fe
#define VT_ENCODING_INDEX_SHIFT                 1
#define VT_ENCODING_TYPE_MASK          0x00000c00
#define VT_ENCODING_TYPE_SHIFT                 10
#define VT_ENCODING_TYPE_CTL                    0
#define VT_ENCODING_TYPE_VMEXIT_INFO            1
#define VT_ENCODING_TYPE_GUEST                  2
#define VT_ENCODING_TYPE_HOST                   3
#define VT_ENCODING_NUM_TYPES                   4
#define VT_ENCODING_SIZE_MASK          0x00006000
#define VT_ENCODING_SIZE_SHIFT                 13
#define VT_ENCODING_SIZE_16BIT                  0
#define VT_ENCODING_SIZE_64BIT                  1
#define VT_ENCODING_SIZE_32BIT                  2
#define VT_ENCODING_SIZE_NATURAL                3
#define VT_ENCODING_NUM_SIZES                   4
#define VT_ENCODING_RSVD               0xffff9000

/*
 * VMCS encodings; volume 3B Appendix H. These are the values passed to
 * VMWrite and VMRead.
 */

/* 16-bit control field. */
#define VT_VMCS_VPID                   0x00000000
#define VT_VMCS_PI_NOTIFY              0x00000002
#define VT_VMCS_EPTP_INDEX             0x00000004

/* 16-bit guest state. */
#define VT_VMCS_ES                     0x00000800
#define VT_VMCS_CS                     0x00000802
#define VT_VMCS_SS                     0x00000804
#define VT_VMCS_DS                     0x00000806
#define VT_VMCS_FS                     0x00000808
#define VT_VMCS_GS                     0x0000080A
#define VT_VMCS_LDTR                   0x0000080C
#define VT_VMCS_TR                     0x0000080E
#define VT_VMCS_INTR_STATUS            0x00000810

/* 16-bit host state. */
#define VT_VMCS_HOST_ES                0x00000C00
#define VT_VMCS_HOST_CS                0x00000C02
#define VT_VMCS_HOST_SS                0x00000C04
#define VT_VMCS_HOST_DS                0x00000C06
#define VT_VMCS_HOST_FS                0x00000C08
#define VT_VMCS_HOST_GS                0x00000C0A
#define VT_VMCS_HOST_TR                0x00000C0C

/* 64-bit control fields. */
#define VT_VMCS_IOBITMAPA              0x00002000
#define VT_VMCS_IOBITMAPB              0x00002002
#define VT_VMCS_MSRBITMAP              0x00002004
#define VT_VMCS_VMEXIT_MSR_STORE_ADDR  0x00002006
#define VT_VMCS_VMEXIT_MSR_LOAD_ADDR   0x00002008
#define VT_VMCS_VMENTRY_MSR_LOAD_ADDR  0x0000200A
#define VT_VMCS_EXECUTIVE_VMCS_PTR     0x0000200C
#define VT_VMCS_TSC_OFF                0x00002010
#define VT_VMCS_VIRT_APIC_ADDR         0x00002012
#define VT_VMCS_APIC_ACCESS_ADDR       0x00002014
#define VT_VMCS_PI_DESC_ADDR           0x00002016
#define VT_VMCS_VMFUNC_CTLS            0x00002018
#define VT_VMCS_EPTP                   0x0000201A
#define VT_VMCS_EOI_EXIT0              0x0000201C
#define VT_VMCS_EOI_EXIT1              0x0000201E
#define VT_VMCS_EOI_EXIT2              0x00002020
#define VT_VMCS_EOI_EXIT3              0x00002022
#define VT_VMCS_EPTP_LIST_ADDR         0x00002024
#define VT_VMCS_VMREAD_BITMAP          0x00002026
#define VT_VMCS_VMWRITE_BITMAP         0x00002028
#define VT_VMCS_VE_INFO_ADDR           0x0000202A
#define VT_VMCS_XSS_EXITING_BITMAP     0x0000202C

/* 64-bit read-only data field. */
#define VT_VMCS_PHYSADDR               0x00002400

/* 64-bit guest state. */
#define VT_VMCS_LINK_PTR               0x00002800
#define VT_VMCS_DEBUGCTL               0x00002802
#define VT_VMCS_PAT                    0x00002804
#define VT_VMCS_EFER                   0x00002806
#define VT_VMCS_PGC                    0x00002808
#define VT_VMCS_PDPTE0                 0x0000280A
#define VT_VMCS_PDPTE1                 0x0000280C
#define VT_VMCS_PDPTE2                 0x0000280E
#define VT_VMCS_PDPTE3                 0x00002810

/* 64-bit host state. */
#define VT_VMCS_HOST_PAT               0x00002C00
#define VT_VMCS_HOST_EFER              0x00002C02
#define VT_VMCS_HOST_PGC               0x00002C04

/* 32-bit control fields. */
#define VT_VMCS_PIN_VMEXEC_CTL         0x00004000
#define VT_VMCS_CPU_VMEXEC_CTL         0x00004002
#define VT_VMCS_XCP_BITMAP             0x00004004
#define VT_VMCS_PF_ERR_MASK            0x00004006
#define VT_VMCS_PF_ERR_MATCH           0x00004008
#define VT_VMCS_CR3_TARG_COUNT         0x0000400A
#define VT_VMCS_VMEXIT_CTL             0x0000400C
#define VT_VMCS_VMEXIT_MSR_STORE_COUNT 0x0000400E
#define VT_VMCS_VMEXIT_MSR_LOAD_COUNT  0x00004010
#define VT_VMCS_VMENTRY_CTL            0x00004012
#define VT_VMCS_VMENTRY_MSR_LOAD_COUNT 0x00004014
#define VT_VMCS_VMENTRY_INTR_INFO      0x00004016
#define VT_VMCS_VMENTRY_XCP_ERR        0x00004018
#define VT_VMCS_VMENTRY_INSTR_LEN      0x0000401A
#define VT_VMCS_TPR_THRESHOLD          0x0000401C
#define VT_VMCS_2ND_VMEXEC_CTL         0x0000401E
#define VT_VMCS_PAUSE_LOOP_GAP         0x00004020
#define VT_VMCS_PAUSE_LOOP_WINDOW      0x00004022

/* 32-bit read-only data fields. */
#define VT_VMCS_VMINSTR_ERR            0x00004400
#define VT_VMCS_EXIT_REASON            0x00004402
#define VT_VMCS_EXIT_INTR_INFO         0x00004404
#define VT_VMCS_EXIT_INTR_ERR          0x00004406
#define VT_VMCS_IDTVEC_INFO            0x00004408
#define VT_VMCS_IDTVEC_ERR             0x0000440A
#define VT_VMCS_INSTRLEN               0x0000440C
#define VT_VMCS_INSTR_INFO             0x0000440E

/* 32-bit guest state. */
#define VT_VMCS_ES_LIMIT               0x00004800
#define VT_VMCS_CS_LIMIT               0x00004802
#define VT_VMCS_SS_LIMIT               0x00004804
#define VT_VMCS_DS_LIMIT               0x00004806
#define VT_VMCS_FS_LIMIT               0x00004808
#define VT_VMCS_GS_LIMIT               0x0000480A
#define VT_VMCS_LDTR_LIMIT             0x0000480C
#define VT_VMCS_TR_LIMIT               0x0000480E
#define VT_VMCS_GDTR_LIMIT             0x00004810
#define VT_VMCS_IDTR_LIMIT             0x00004812
#define VT_VMCS_ES_AR                  0x00004814
#define VT_VMCS_CS_AR                  0x00004816
#define VT_VMCS_SS_AR                  0x00004818
#define VT_VMCS_DS_AR                  0x0000481A
#define VT_VMCS_FS_AR                  0x0000481C
#define VT_VMCS_GS_AR                  0x0000481E
#define VT_VMCS_LDTR_AR                0x00004820
#define VT_VMCS_TR_AR                  0x00004822
#define VT_VMCS_HOLDOFF                0x00004824
#define VT_VMCS_ACTSTATE               0x00004826
#define VT_VMCS_SMBASE                 0x00004828
#define VT_VMCS_SYSENTER_CS            0x0000482A
#define VT_VMCS_TIMER                  0x0000482E

/* 32-bit host state. */
#define VT_VMCS_HOST_SYSENTER_CS       0x00004C00

/* natural-width control fields. */
#define VT_VMCS_CR0_GHMASK             0x00006000
#define VT_VMCS_CR4_GHMASK             0x00006002
#define VT_VMCS_CR0_SHADOW             0x00006004
#define VT_VMCS_CR4_SHADOW             0x00006006
#define VT_VMCS_CR3_TARGVAL0           0x00006008
#define VT_VMCS_CR3_TARGVAL1           0x0000600A
#define VT_VMCS_CR3_TARGVAL2           0x0000600C
#define VT_VMCS_CR3_TARGVAL3           0x0000600E

/* natural-width read-only data fields. */
#define VT_VMCS_EXIT_QUAL              0x00006400
#define VT_VMCS_IO_ECX                 0x00006402
#define VT_VMCS_IO_ESI                 0x00006404
#define VT_VMCS_IO_EDI                 0x00006406
#define VT_VMCS_IO_EIP                 0x00006408
#define VT_VMCS_LINEAR_ADDR            0x0000640A

/* natural-width guest state. */
#define VT_VMCS_CR0                    0x00006800
#define VT_VMCS_CR3                    0x00006802
#define VT_VMCS_CR4                    0x00006804
#define VT_VMCS_ES_BASE                0x00006806
#define VT_VMCS_CS_BASE                0x00006808
#define VT_VMCS_SS_BASE                0x0000680A
#define VT_VMCS_DS_BASE                0x0000680C
#define VT_VMCS_FS_BASE                0x0000680E
#define VT_VMCS_GS_BASE                0x00006810
#define VT_VMCS_LDTR_BASE              0x00006812
#define VT_VMCS_TR_BASE                0x00006814
#define VT_VMCS_GDTR_BASE              0x00006816
#define VT_VMCS_IDTR_BASE              0x00006818
#define VT_VMCS_DR7                    0x0000681A
#define VT_VMCS_ESP                    0x0000681C
#define VT_VMCS_EIP                    0x0000681E
#define VT_VMCS_EFLAGS                 0x00006820
#define VT_VMCS_PENDDBG                0x00006822
#define VT_VMCS_SYSENTER_ESP           0x00006824
#define VT_VMCS_SYSENTER_EIP           0x00006826

/* natural-width host state. */
#define VT_VMCS_HOST_CR0               0x00006C00
#define VT_VMCS_HOST_CR3               0x00006C02
#define VT_VMCS_HOST_CR4               0x00006C04
#define VT_VMCS_HOST_FSBASE            0x00006C06
#define VT_VMCS_HOST_GSBASE            0x00006C08
#define VT_VMCS_HOST_TRBASE            0x00006C0A
#define VT_VMCS_HOST_GDTRBASE          0x00006C0C
#define VT_VMCS_HOST_IDTRBASE          0x00006C0E
#define VT_VMCS_HOST_SYSENTER_ESP      0x00006C10
#define VT_VMCS_HOST_SYSENTER_EIP      0x00006C12
#define VT_VMCS_HOST_ESP               0x00006C14
#define VT_VMCS_HOST_EIP               0x00006C16

/*
 * Sizes of referenced fields
 */
#define VT_VMCS_IO_BITMAP_SIZE    (2 * PAGE_SIZE)
#define VT_VMCS_MSR_BITMAP_SIZE   (1 * PAGE_SIZE)

/*
 * Execution control bit capabilities come in pairs: a "required" bit in
 * the low dword, and an "allowed" bit in the high dword.
 */
#define VMXCTL(_msrName, _field, _pos)                   \
   VMXREQUIRE(_msrName, _field, _pos,        1)          \
     VMXALLOW(_msrName, _field, (_pos) + 32, 1)

/*
 * VMX-fixed bit MSRs come in pairs: allowed to be zero and allowed to
 * be one.  Each has its own capability bits, but the field names and bit
 * positions are the same.
 */
#define VMXFIXED(_msrName, _field, _pos)                 \
   VMXCAP(_msrName ## 0, _field, _pos, 1)                \
   VMXCAP(_msrName ## 1, _field, _pos, 1)

/*
 * Basic VMX Information
 */
#define VMX_BASIC(_field, _pos, _len)                    \
   VMXCAP(_BASIC, _field, _pos, _len)
#define VMX_BASIC_CAP_NDA
#define VMX_BASIC_CAP_PUB                                \
   VMX_BASIC(VMCS_ID,               0, 32)               \
   VMX_BASIC(VMCS_SIZE,            32, 13)               \
   VMX_BASIC(32BITPA,              48,  1)               \
   VMX_BASIC(DUALVMM,              49,  1)               \
   VMX_BASIC(MEMTYPE,              50,  4)               \
   VMX_BASIC(ADVANCED_IOINFO,      54,  1)               \
   VMX_BASIC(TRUE_CTLS,            55,  1)

#define VMX_BASIC_CAP                                    \
        VMX_BASIC_CAP_NDA                                \
        VMX_BASIC_CAP_PUB

/*
 * Pin-Based VM-Execution Controls
 */
#define VMX_PIN(_field, _pos)                            \
   VMXCTL(_PINBASED_CTLS, _field, _pos)
#define VMX_PINBASED_CTLS_CAP_NDA

#define VMX_PINBASED_CTLS_CAP_PUB                        \
   VMX_PIN(EXTINT_EXIT,          0)                      \
   VMX_PIN(NMI_EXIT,             3)                      \
   VMX_PIN(VNMI,                 5)                      \
   VMX_PIN(TIMER,                6)                      \
   VMX_PIN(POSTED_INTR,          7)

#define VMX_PINBASED_CTLS_CAP                            \
        VMX_PINBASED_CTLS_CAP_NDA                        \
        VMX_PINBASED_CTLS_CAP_PUB

/*
 * Primary Processor-Based VM-Execution Controls
 */
#define VMX_CPU(_field, _pos)                            \
   VMXCTL(_PROCBASED_CTLS, _field, _pos)
#define VMX_PROCBASED_CTLS_CAP_NDA

#define VMX_PROCBASED_CTLS_CAP_PUB                       \
   VMX_CPU(VINTR_WINDOW,         2)                      \
   VMX_CPU(TSCOFF,               3)                      \
   VMX_CPU(HLT,                  7)                      \
   VMX_CPU(INVLPG,               9)                      \
   VMX_CPU(MWAIT,               10)                      \
   VMX_CPU(RDPMC,               11)                      \
   VMX_CPU(RDTSC,               12)                      \
   VMX_CPU(LDCR3,               15)                      \
   VMX_CPU(STCR3,               16)                      \
   VMX_CPU(LDCR8,               19)                      \
   VMX_CPU(STCR8,               20)                      \
   VMX_CPU(TPR_SHADOW,          21)                      \
   VMX_CPU(VNMI_WINDOW,         22)                      \
   VMX_CPU(MOVDR,               23)                      \
   VMX_CPU(IO,                  24)                      \
   VMX_CPU(IOBITMAP,            25)                      \
   VMX_CPU(MTF,                 27)                      \
   VMX_CPU(MSRBITMAP,           28)                      \
   VMX_CPU(MONITOR,             29)                      \
   VMX_CPU(PAUSE,               30)                      \
   VMX_CPU(USE_2ND,             31)

#define VMX_PROCBASED_CTLS_CAP                           \
        VMX_PROCBASED_CTLS_CAP_NDA                       \
        VMX_PROCBASED_CTLS_CAP_PUB

/*
 * Secondary Processor-Based VM-Execution Controls
 */
#define VMX_CPU2(_field, _pos)                           \
   VMXCTL(_PROCBASED_CTLS2, _field, _pos)
#define VMX_PROCBASED_CTLS2_CAP_NDA
#define VMX_PROCBASED_CTLS2_CAP_PUB                      \
   VMX_CPU2(APIC,                0)                      \
   VMX_CPU2(EPT,                 1)                      \
   VMX_CPU2(DT,                  2)                      \
   VMX_CPU2(RDTSCP,              3)                      \
   VMX_CPU2(X2APIC,              4)                      \
   VMX_CPU2(VPID,                5)                      \
   VMX_CPU2(WBINVD,              6)                      \
   VMX_CPU2(UNRESTRICTED,        7)                      \
   VMX_CPU2(APICREG,             8)                      \
   VMX_CPU2(VINTR,               9)                      \
   VMX_CPU2(PAUSE_LOOP,         10)                      \
   VMX_CPU2(RDRAND,             11)                      \
   VMX_CPU2(INVPCID,            12)                      \
   VMX_CPU2(VMFUNC,             13)                      \
   VMX_CPU2(VMCS_SHADOW,        14)                      \
   VMX_CPU2(RDSEED,             16)                      \
   VMX_CPU2(EPT_VIOL_VE,        18)                      \
   VMX_CPU2(XSAVES,             20)

#define VMX_PROCBASED_CTLS2_CAP                          \
        VMX_PROCBASED_CTLS2_CAP_NDA                      \
        VMX_PROCBASED_CTLS2_CAP_PUB

/*
 * VM-Exit Controls
 */
#define VMX_EXIT(_field, _pos)                           \
   VMXCTL(_EXIT_CTLS, _field, _pos)
#define VMX_EXIT_CTLS_CAP_NDA
#define VMX_EXIT_CTLS_CAP_PUB                            \
   VMX_EXIT(SAVE_DEBUGCTL,       2)                      \
   VMX_EXIT(LONGMODE,            9)                      \
   VMX_EXIT(LOAD_PGC,           12)                      \
   VMX_EXIT(INTRACK,            15)                      \
   VMX_EXIT(SAVE_PAT,           18)                      \
   VMX_EXIT(LOAD_PAT,           19)                      \
   VMX_EXIT(SAVE_EFER,          20)                      \
   VMX_EXIT(LOAD_EFER,          21)                      \
   VMX_EXIT(SAVE_TIMER,         22)

#define VMX_EXIT_CTLS_CAP                                \
        VMX_EXIT_CTLS_CAP_NDA                            \
        VMX_EXIT_CTLS_CAP_PUB

/*
 * VM-Entry Controls
 */
#define VMX_ENTRY(_field, _pos)                          \
   VMXCTL(_ENTRY_CTLS, _field, _pos)
#define VMX_ENTRY_CTLS_CAP_NDA
#define VMX_ENTRY_CTLS_CAP_PUB                           \
   VMX_ENTRY(LOAD_DEBUGCTL,      2)                      \
   VMX_ENTRY(LONGMODE,           9)                      \
   VMX_ENTRY(ENTRY_TO_SMM,      10)                      \
   VMX_ENTRY(SMM_TEARDOWN,      11)                      \
   VMX_ENTRY(LOAD_PGC,          13)                      \
   VMX_ENTRY(LOAD_PAT,          14)                      \
   VMX_ENTRY(LOAD_EFER,         15)

#define VMX_ENTRY_CTLS_CAP                               \
        VMX_ENTRY_CTLS_CAP_NDA                           \
        VMX_ENTRY_CTLS_CAP_PUB

/*
 * Miscellaneoous Data
 */
#define VMX_MISC(_field, _pos, _len)                     \
   VMXCAP(_MISC, _field, _pos, _len)
#define VMX_MISC_CAP_NDA
#define VMX_MISC_CAP_PUB                                 \
   VMX_MISC(TMR_RATIO,              0,  5)               \
   VMX_MISC(VMEXIT_SAVES_LMA,       5,  1)               \
   VMX_MISC(ACTSTATE_HLT,           6,  1)               \
   VMX_MISC(ACTSTATE_SHUTDOWN,      7,  1)               \
   VMX_MISC(ACTSTATE_SIPI,          8,  1)               \
   VMX_MISC(RDMSR_SMBASE_IN_SMM,   15,  1)               \
   VMX_MISC(CR3_TARGETS,           16,  9)               \
   VMX_MISC(MAX_MSRS,              25,  3)               \
   VMX_MISC(ALLOW_ALL_VMWRITES,    29,  1)               \
   VMX_MISC(MSEG_ID,               32, 32)               \

#define VMX_MISC_CAP                                     \
        VMX_MISC_CAP_NDA                                 \
        VMX_MISC_CAP_PUB

/*
 * VMX-Fixed Bits in CR0
 */
#define VMX_FIXED_CR0(_field, _pos)                      \
   VMXFIXED(_CR0_FIXED, _field, _pos)
#define VMX_FIXED_CR0_CAP_NDA
#define VMX_FIXED_CR0_CAP_PUB                            \
   VMX_FIXED_CR0(PE,          0)                         \
   VMX_FIXED_CR0(MP,          1)                         \
   VMX_FIXED_CR0(EM,          2)                         \
   VMX_FIXED_CR0(TS,          3)                         \
   VMX_FIXED_CR0(ET,          4)                         \
   VMX_FIXED_CR0(NE,          5)                         \
   VMX_FIXED_CR0(WP,         16)                         \
   VMX_FIXED_CR0(AM,         18)                         \
   VMX_FIXED_CR0(NW,         29)                         \
   VMX_FIXED_CR0(CD,         30)                         \
   VMX_FIXED_CR0(PG,         31)

#define VMX_FIXED_CR0_CAP                                \
        VMX_FIXED_CR0_CAP_NDA                            \
        VMX_FIXED_CR0_CAP_PUB

/*
 * VMX-Fixed Bits in CR4
 */
#define VMX_FIXED_CR4(_field, _pos)                      \
   VMXFIXED(_CR4_FIXED, _field, _pos)
#define VMX_FIXED_CR4_CAP_NDA
#define VMX_FIXED_CR4_CAP_PUB                            \
   VMX_FIXED_CR4(VME,         0)                         \
   VMX_FIXED_CR4(PVI,         1)                         \
   VMX_FIXED_CR4(TSD,         2)                         \
   VMX_FIXED_CR4(DE,          3)                         \
   VMX_FIXED_CR4(PSE,         4)                         \
   VMX_FIXED_CR4(PAE,         5)                         \
   VMX_FIXED_CR4(MCE,         6)                         \
   VMX_FIXED_CR4(PGE,         7)                         \
   VMX_FIXED_CR4(PCE,         8)                         \
   VMX_FIXED_CR4(OSFXSR,      9)                         \
   VMX_FIXED_CR4(OSXMMEXCPT, 10)                         \
   VMX_FIXED_CR4(VMXE,       13)                         \
   VMX_FIXED_CR4(SMXE,       14)                         \
   VMX_FIXED_CR4(FSGSBASE,   16)                         \
   VMX_FIXED_CR4(PCIDE,      17)                         \
   VMX_FIXED_CR4(OSXSAVE,    18)                         \
   VMX_FIXED_CR4(SMEP,       20)                         \
   VMX_FIXED_CR4(SMAP,       21)

#define VMX_FIXED_CR4_CAP                                \
        VMX_FIXED_CR4_CAP_NDA                            \
        VMX_FIXED_CR4_CAP_PUB

/*
 * VMCS Enumeration
 */
#define VMX_VMCS_ENUM_CAP_NDA
#define VMX_VMCS_ENUM_CAP_PUB                           \
   VMXCAP(_VMCS_ENUM, MAX_INDEX,    1,  9)

#define VMX_VMCS_ENUM_CAP                               \
        VMX_VMCS_ENUM_CAP_NDA                           \
        VMX_VMCS_ENUM_CAP_PUB

/*
 * VPID and EPT Capabilities
 */
#define VMX_EPT(_field, _pos, _len)                     \
   VMXCAP(_EPT_VPID, _field, _pos, _len)
#define VMX_EPT_VPID_CAP_NDA
#define VMX_EPT_VPID_CAP_PUB                            \
   VMX_EPT(EPTE_X,                  0,  1)              \
   VMX_EPT(GAW_48,                  6,  1)              \
   VMX_EPT(ETMT_UC,                 8,  1)              \
   VMX_EPT(ETMT_WB,                14,  1)              \
   VMX_EPT(SP_2MB,                 16,  1)              \
   VMX_EPT(SP_1GB,                 17,  1)              \
   VMX_EPT(INVEPT,                 20,  1)              \
   VMX_EPT(ACCESS_DIRTY,           21,  1)              \
   VMX_EPT(INVEPT_EPT_CTX,         25,  1)              \
   VMX_EPT(INVEPT_GLOBAL,          26,  1)              \
   VMX_EPT(INVVPID,                32,  1)              \
   VMX_EPT(INVVPID_ADDR,           40,  1)              \
   VMX_EPT(INVVPID_VPID_CTX,       41,  1)              \
   VMX_EPT(INVVPID_ALL_CTX,        42,  1)              \
   VMX_EPT(INVVPID_VPID_CTX_LOCAL, 43,  1)

#define VMX_EPT_VPID_CAP                                \
        VMX_EPT_VPID_CAP_NDA                            \
        VMX_EPT_VPID_CAP_PUB

/*
 * VM Functions
 */
#define VMX_VMFUNC_CAP_NDA
#define VMX_VMFUNC_CAP_PUB                              \
   VMXCAP(_VMFUNC, EPTP_SWITCHING,  0,  1)

#define VMX_VMFUNC_CAP     \
        VMX_VMFUNC_CAP_NDA \
        VMX_VMFUNC_CAP_PUB




/*
 * Match the historical names for these fields:
 * <field>_SHIFT is the lsb of the field.
 * <field>_MASK is an unshifted bit-mask the width of the field.
 * <field> is the bit-mask shifted into the field's position in the MSR.
 */
enum {
#define VMXCAP(_msrName, _field, _pos, _len)                              \
   MSR_VMX ## _msrName ## _ ## _field ## _SHIFT = (_pos),                 \
   MSR_VMX ## _msrName ## _ ## _field ## _MASK  = (int)MASK64(_len),      \

   VMX_BASIC_CAP
   VMX_MISC_CAP
   VMX_VMCS_ENUM_CAP
   VMX_EPT_VPID_CAP

#undef VMXCAP
};

/*
 * Convert capabilities into VMCS control bit names.
 */
enum {
#define _PINBASED_CTLS        VT_VMCS_PIN_VMEXEC_CTL_
#define _PROCBASED_CTLS       VT_VMCS_CPU_VMEXEC_CTL_
#define _PROCBASED_CTLS2      VT_VMCS_2ND_VMEXEC_CTL_
#define _EXIT_CTLS            VT_VMCS_VMEXIT_CTL_
#define _ENTRY_CTLS           VT_VMCS_VMENTRY_CTL_

#define VMXREQUIRE(_msrName, _field, _pos, _len)                      \
        VMXCAP(_msrName, _field, _pos, _len)
#define VMXALLOW(_msrName, _field, _pos, _len)
#define VMXCAP(_msrName, _field, _pos, _len)                          \
   CONC(_msrName, _field) = 1ULL << (_pos),

   VMX_PINBASED_CTLS_CAP
   VMX_PROCBASED_CTLS_CAP
   VMX_EXIT_CTLS_CAP
   VMX_ENTRY_CTLS_CAP
   VMX_PROCBASED_CTLS2_CAP

#undef VMXCAP
#undef VMXALLOW
#undef VMXREQUIRE

#undef _ENTRY_CTLS
#undef _EXIT_CTLS
#undef _PROCBASED_CTLS2
#undef _PROCBASED_CTLS
#undef _PINBASED_CTLS
};

/*
 * The AR format is mostly the same as the SMM segment format; i.e.,
 * a descriptor shifted by a byte. However, there is an extra bit in
 * the high-order word which indicates an "unusable" selector.  A NULL
 * selector is generally unusable, as are a few other corner cases.
 */
#define VT_VMCS_AR_UNUSABLE   0x00010000
#define VT_VMCS_AR_RESERVED   0xfffe0f00

/*
 * Pending debug bits partially follow their DR6 counterparts.
 * However, there are no must-be-one bits, the bits corresponding
 * to DR6_BD and DR6_BT must be zero, and bit 12 indicates an
 * enabled breakpoint.
 */
#define VT_VMCS_PENDDBG_B0         0x00000001
#define VT_VMCS_PENDDBG_B1         0x00000002
#define VT_VMCS_PENDDBG_B2         0x00000004
#define VT_VMCS_PENDDBG_B3         0x00000008
#define VT_VMCS_PENDDBG_BE         0x00001000
#define VT_VMCS_PENDDBG_BS         0x00004000
#define VT_VMCS_PENDDBG_RTM        0x00010000
#define VT_VMCS_PENDDBG_MBZ        0xfffeaff0

/* Exception error must-be-zero bits for VMEntry */
#define VT_XCP_ERR_MBZ             0xffff8000

/* Exit reasons. */
#define VT_EXITREASON_EXC_OR_NMI            0
#define VT_EXITREASON_EXTINT                1
#define VT_EXITREASON_TRIPLEFAULT           2
#define VT_EXITREASON_INIT                  3
#define VT_EXITREASON_SIPI                  4
#define VT_EXITREASON_IOSMI                 5
#define VT_EXITREASON_OTHERSMI              6
#define VT_EXITREASON_VINTR_WINDOW          7
#define VT_EXITREASON_VNMI_WINDOW           8
#define VT_EXITREASON_TS                    9
#define VT_EXITREASON_CPUID                10
#define VT_EXITREASON_GETSEC               11
#define VT_EXITREASON_HLT                  12
#define VT_EXITREASON_INVD                 13
#define VT_EXITREASON_INVLPG               14
#define VT_EXITREASON_RDPMC                15
#define VT_EXITREASON_RDTSC                16
#define VT_EXITREASON_RSM                  17
#define VT_EXITREASON_VMCALL               18
#define VT_EXITREASON_VMCLEAR              19
#define VT_EXITREASON_VMLAUNCH             20
#define VT_EXITREASON_VMPTRLD              21
#define VT_EXITREASON_VMPTRST              22
#define VT_EXITREASON_VMREAD               23
#define VT_EXITREASON_VMRESUME             24
#define VT_EXITREASON_VMWRITE              25
#define VT_EXITREASON_VMXOFF               26
#define VT_EXITREASON_VMXON                27
#define VT_EXITREASON_CR                   28
#define VT_EXITREASON_DR                   29
#define VT_EXITREASON_IO                   30
#define VT_EXITREASON_RDMSR                31
#define VT_EXITREASON_WRMSR                32
#define VT_EXITREASON_VMENTRYFAIL_GUEST   (33 | VT_EXITREASON_VMENTRYFAIL)
#define VT_EXITREASON_VMENTRYFAIL_MSR     (34 | VT_EXITREASON_VMENTRYFAIL)
#define VT_EXITREASON_MWAIT                36
#define VT_EXITREASON_MTF                  37
#define VT_EXITREASON_MONITOR              39
#define VT_EXITREASON_PAUSE                40
#define VT_EXITREASON_VMENTRYFAIL_MC      (41 | VT_EXITREASON_VMENTRYFAIL)
#define VT_EXITREASON_TPR                  43
#define VT_EXITREASON_APIC                 44
#define VT_EXITREASON_EOI                  45
#define VT_EXITREASON_GDTR_IDTR            46
#define VT_EXITREASON_LDTR_TR              47
#define VT_EXITREASON_EPT_VIOLATION        48
#define VT_EXITREASON_EPT_MISCONFIG        49
#define VT_EXITREASON_INVEPT               50
#define VT_EXITREASON_RDTSCP               51
#define VT_EXITREASON_TIMER                52
#define VT_EXITREASON_INVVPID              53
#define VT_EXITREASON_WBINVD               54
#define VT_EXITREASON_XSETBV               55
#define VT_EXITREASON_APIC_WRITE           56
#define VT_EXITREASON_RDRAND               57
#define VT_EXITREASON_INVPCID              58
#define VT_EXITREASON_VMFUNC               59
#define VT_EXITREASON_RDSEED               61
#define VT_EXITREASON_XSAVES               63
#define VT_EXITREASON_XRSTORS              64

/*
 * VT synthesized exit reasons:
 *
 * Faked up reasons, not overlapping with any real exit codes, which
 * help save repeated VMREADS in HVExit and HVTryFastExit of
 * VT_VMCS_EXIT_INTR_INFO to extract the TYPE_MASK and VECTOR_MASK.
 * See HVExitGlue.
 *
 * We shouldn't have to worry about new hardware introducing conflicting
 * exit reasons, because we shouldn't encounter any new exit reasons
 * unless we opt-in to the features that produce them.
 */
#define VT_EXITREASON_SYNTH_BASE     77
#define VT_EXITREASON_SYNTH_IRET     77
#define VT_EXITREASON_SYNTH_NMI      78
#define VT_EXITREASON_SYNTH_ICEBP    79
#define VT_EXITREASON_SYNTH_EXC_BASE 80
#define VT_EXITREASON_SYNTH_MAX      111

#define VT_EXITREASON_SYNTH_EXC(gatenum) \
        (VT_EXITREASON_SYNTH_EXC_BASE + gatenum) /* 0-31 */

#define VT_EXITREASON_VMENTRYFAIL   (1U << 31)

/* Instruction error codes. */
#define VT_ERROR_VMCALL_VMX_ROOT            1
#define VT_ERROR_VMCLEAR_INVALID_PA         2
#define VT_ERROR_VMCLEAR_ROOT_PTR           3
#define VT_ERROR_VMLAUNCH_NOT_CLEAR         4
#define VT_ERROR_VMRESUME_NOT_LAUNCHED      5
#define VT_ERROR_VMRESUME_AFTER_VMXOFF      6
#define VT_ERROR_VMENTRY_INVALID_CTL        7
#define VT_ERROR_VMENTRY_INVALID_HOST       8
#define VT_ERROR_VMPTRLD_INVALID_PA         9
#define VT_ERROR_VMPTRLD_ROOT_PTR          10
#define VT_ERROR_VMPTRLD_BAD_REVISION      11
#define VT_ERROR_VMACCESS_UNSUPPORTED      12
#define VT_ERROR_VMWRITE_READ_ONLY         13
#define VT_ERROR_VMXON_VMX_ROOT            15
#define VT_ERROR_VMENTRY_INVALID_EXEC      16
#define VT_ERROR_VMENTRY_EXEC_NOT_LAUNCHED 17
#define VT_ERROR_VMENTRY_EXEC_NOT_ROOT     18
#define VT_ERROR_VMCALL_NOT_CLEAR          19
#define VT_ERROR_VMCALL_INVALID_CTL        20
#define VT_ERROR_VMCALL_WRONG_MSEG         22
#define VT_ERROR_VMXOFF_DUALVMM            23
#define VT_ERROR_VMCALL_INVALID_SMM        24
#define VT_ERROR_VMENTRY_INVALID_EXEC_CTL  25
#define VT_ERROR_VMENTRY_MOVSS_SHADOW      26
#define VT_ERROR_INVALIDATION_INVALID      28

/* interrupt information fields. Low order 8 bits are vector. */
#define VT_INTRINFO_TYPE_SHIFT      8
#define VT_INTRINFO_TYPE_MASK       (7 << VT_INTRINFO_TYPE_SHIFT)
#define VT_INTRINFO_TYPE_EXTINT     (0 << VT_INTRINFO_TYPE_SHIFT)
#define VT_INTRINFO_TYPE_RSVD       (1 << VT_INTRINFO_TYPE_SHIFT)
#define VT_INTRINFO_TYPE_NMI        (2 << VT_INTRINFO_TYPE_SHIFT)
#define VT_INTRINFO_TYPE_EXC        (3 << VT_INTRINFO_TYPE_SHIFT)
#define VT_INTRINFO_TYPE_INTN       (4 << VT_INTRINFO_TYPE_SHIFT)
#define VT_INTRINFO_TYPE_PRIVTRAP   (5 << VT_INTRINFO_TYPE_SHIFT)
#define VT_INTRINFO_TYPE_UNPRIVTRAP (6 << VT_INTRINFO_TYPE_SHIFT)
#define VT_INTRINFO_TYPE_OTHER      (7 << VT_INTRINFO_TYPE_SHIFT)
#define VT_INTRINFO_ERRORCODE       (1 << 11)
#define VT_INTRINFO_NMIUNMASK       (1 << 12)
#define VT_INTRINFO_VALID           (1U << 31)
#define VT_INTRINFO_VECTOR_MASK     ((1 << VT_INTRINFO_TYPE_SHIFT) - 1)
#define VT_INTRINFO_RESERVED        0x7fffe000

/* Activity State */
#define VT_ACTSTATE_ACTIVE     0
#define VT_ACTSTATE_HLT        1
#define VT_ACTSTATE_SHUT_DOWN  2
#define VT_ACTSTATE_WFSIPI     3

/* Interruptibility */
#define VT_HOLDOFF_STI         0x1
#define VT_HOLDOFF_MOVSS       0x2
#define VT_HOLDOFF_SMI         0x4
#define VT_HOLDOFF_NMI         0x8
#define VT_HOLDOFF_INST        (VT_HOLDOFF_STI | VT_HOLDOFF_MOVSS)
#define VT_HOLDOFF_RSV         0xFFFFFFF0

/* VM Functions */
#define VT_VMFUNC_MASK(_vmfunc)        (1ULL << (VT_VMFUNC_ ## _vmfunc))
#define VT_VMFUNC_SWITCH_EPTP          0

/* EPT Violation Qualification */
#define VT_EPT_QUAL_ACCESS_SHIFT       0
#define VT_EPT_QUAL_ACCESS_MASK        (0x7 << VT_EPT_QUAL_ACCESS_SHIFT)
#define VT_EPT_QUAL_ACCESS_R           (1 << 0)
#define VT_EPT_QUAL_ACCESS_W           (1 << 1)
#define VT_EPT_QUAL_ACCESS_X           (1 << 2)
#define VT_EPT_QUAL_PROT_SHIFT         3
#define VT_EPT_QUAL_PROT_MASK          (0x7 << VT_EPT_QUAL_PROT_SHIFT)
#define VT_EPT_QUAL_PROT_R             (1 << 3)
#define VT_EPT_QUAL_PROT_W             (1 << 4)
#define VT_EPT_QUAL_PROT_X             (1 << 5)
#define VT_EPT_QUAL_LA_VALID           (1 << 7)
#define VT_EPT_QUAL_FINAL_ADDR         (1 << 8)
#define VT_EPT_QUAL_NMIUNMASK          (1 << 12)


/* IOIO Qualification */
#define VT_IO_QUAL_SIZE_SHIFT          0
#define VT_IO_QUAL_SIZE_MASK           (0x7 << VT_IO_QUAL_SIZE_SHIFT)
#define VT_IO_QUAL_IN                  (1 << 3)
#define VT_IO_QUAL_STR                 (1 << 4)
#define VT_IO_QUAL_REP                 (1 << 5)
#define VT_IO_QUAL_IMM                 (1 << 6)
#define VT_IO_QUAL_PORT_SHIFT          16
#define VT_IO_QUAL_PORT_MASK           (0xffff << VT_IO_QUAL_PORT_SHIFT)

/* Invalid Guest State Qualification */
#define VT_GUESTFAIL_QUAL_UNUSED       1
#define VT_GUESTFAIL_QUAL_PDPTE        2
#define VT_GUESTFAIL_QUAL_NMI          3
#define VT_GUESTFAIL_QUAL_LINK         4

/* VMX abort indicators. */

#define VT_VMX_ABORT_GUEST_MSRS        1
#define VT_VMX_ABORT_HOST_PDPTES       2
#define VT_VMX_ABORT_CORRUPT_VMCS      3
#define VT_VMX_ABORT_HOST_MSRS         4
#define VT_VMX_ABORT_VMEXIT_MC         5
#define VT_VMX_ABORT_LM_TO_LEGACY      6


/* Default-to-one bits for VMCS control fields. */

#define VT_PINBASED_CTLS_DEFAULT1      0x00000016
#define VT_PROCBASED_CTLS_DEFAULT1     0x0401e172
#define VT_EXIT_CTLS_DEFAULT1          0x00036dff
#define VT_ENTRY_CTLS_DEFAULT1         0x000011ff


/* Required feature bits. */

#define VT_REQUIRED_PINBASED_CTLS                      \
   (VT_PINBASED_CTLS_DEFAULT1                        | \
    VT_VMCS_PIN_VMEXEC_CTL_EXTINT_EXIT               | \
    VT_VMCS_PIN_VMEXEC_CTL_NMI_EXIT)

#define VT_REQUIRED_PROCBASED_CTLS                     \
   (VT_PROCBASED_CTLS_DEFAULT1                       | \
    VT_VMCS_CPU_VMEXEC_CTL_VINTR_WINDOW              | \
    VT_VMCS_CPU_VMEXEC_CTL_TSCOFF                    | \
    VT_VMCS_CPU_VMEXEC_CTL_HLT                       | \
    VT_VMCS_CPU_VMEXEC_CTL_INVLPG                    | \
    VT_VMCS_CPU_VMEXEC_CTL_MWAIT                     | \
    VT_VMCS_CPU_VMEXEC_CTL_RDPMC                     | \
    VT_VMCS_CPU_VMEXEC_CTL_RDTSC                     | \
    VT_VMCS_CPU_VMEXEC_CTL_IO                        | \
    VT_VMCS_CPU_VMEXEC_CTL_MOVDR                     | \
    VT_VMCS_CPU_VMEXEC_CTL_LDCR8                     | \
    VT_VMCS_CPU_VMEXEC_CTL_STCR8                     | \
    VT_VMCS_CPU_VMEXEC_CTL_TPR_SHADOW                | \
    VT_VMCS_CPU_VMEXEC_CTL_MONITOR)

#define VT_REQUIRED_EXIT_CTLS                          \
   (VT_EXIT_CTLS_DEFAULT1                            | \
    VT_VMCS_VMEXIT_CTL_LONGMODE                      | \
    VT_VMCS_VMEXIT_CTL_INTRACK)

#define VT_REQUIRED_ENTRY_CTLS                         \
   (VT_ENTRY_CTLS_DEFAULT1                           | \
    VT_VMCS_VMENTRY_CTL_LONGMODE)

#define VT_REQUIRED_VPID_SUPPORT                       \
   (MSR_VMX_EPT_VPID_INVVPID                         | \
    MSR_VMX_EPT_VPID_INVVPID_ADDR                    | \
    MSR_VMX_EPT_VPID_INVVPID_VPID_CTX                | \
    MSR_VMX_EPT_VPID_INVVPID_ALL_CTX)

#define VT_REQUIRED_EPT_SUPPORT                        \
   (MSR_VMX_EPT_VPID_EPTE_X                          | \
    MSR_VMX_EPT_VPID_GAW_48                          | \
    MSR_VMX_EPT_VPID_ETMT_WB                         | \
    MSR_VMX_EPT_VPID_SP_2MB                          | \
    MSR_VMX_EPT_VPID_INVEPT                          | \
    MSR_VMX_EPT_VPID_INVEPT_EPT_CTX)

typedef uint64 VTConfig[NUM_VMX_MSRS];

/*
 *----------------------------------------------------------------------
 * VTEncodingHighDword --
 *
 *   Does the VMCS component encoding reference the high 32-bits of a
 *   64-bit component?
 *----------------------------------------------------------------------
 */
static INLINE Bool
VTEncodingHighDword(uint32 encoding)
{
   return (encoding & VT_ENCODING_ACCESS_HIGH) != 0;
}

/*
 *----------------------------------------------------------------------
 * VTEncodingIndex --
 *
 *   Extract the index field from a VMCS component encoding.
 *----------------------------------------------------------------------
 */
static INLINE unsigned
VTEncodingIndex(uint32 encoding)
{
   return (encoding & VT_ENCODING_INDEX_MASK) >> VT_ENCODING_INDEX_SHIFT;
}

/*
 *----------------------------------------------------------------------
 * VTEncodingType --
 *
 *   Extract the type field from a VMCS component encoding.
 *----------------------------------------------------------------------
 */
static INLINE unsigned
VTEncodingType(uint32 encoding)
{
   return (encoding & VT_ENCODING_TYPE_MASK) >> VT_ENCODING_TYPE_SHIFT;
}

/*
 *----------------------------------------------------------------------
 * VTEncodingSize --
 *
 *   Extract the size field from a VMCS component encoding.
 *----------------------------------------------------------------------
 */
static INLINE unsigned
VTEncodingSize(uint32 encoding)
{
   return (encoding & VT_ENCODING_SIZE_MASK) >> VT_ENCODING_SIZE_SHIFT;
}

/*
 *----------------------------------------------------------------------
 * VTComputeMandatoryBits --
 *
 *   Compute the mandatory bits for a VMCS field, based on the allowed
 *   ones and allowed zeros as reported in the appropriate VMX MSR, and
 *   the desired bits.
 *----------------------------------------------------------------------
 */
static INLINE uint32
VTComputeMandatoryBits(uint64 msrVal, uint32 bits)
{
   uint32 ones = LODWORD(msrVal);
   uint32 zeros = HIDWORD(msrVal);
   return (bits | ones) & zeros;
}

/*
 *----------------------------------------------------------------------
 *
 * VT_EnabledFromFeatures --
 *
 *  Returns TRUE if VT is enabled in the given feature control bits.
 *
 *----------------------------------------------------------------------
 */
static INLINE Bool
VT_EnabledFromFeatures(uint64 featCtl)
{
   return ((featCtl & (MSR_FEATCTL_VMXE | MSR_FEATCTL_LOCK)) ==
           (MSR_FEATCTL_VMXE | MSR_FEATCTL_LOCK));
}

/*
 *----------------------------------------------------------------------
 *
 * VT_LockedFromFeatures --
 *
 *  Returns TRUE if VT is locked in the given feature control bits.
 *
 *----------------------------------------------------------------------
 */
static INLINE Bool
VT_LockedFromFeatures(uint64 featCtl)
{
   return (featCtl & MSR_FEATCTL_LOCK) != 0;
}

/*
 *----------------------------------------------------------------------
 *
 * VT_SupportedFromFeatures --
 *
 *   Returns TRUE if the given VMX features are compatible with our VT
 *   monitor.
 *
 *----------------------------------------------------------------------
 */
static INLINE Bool
VT_SupportedFromFeatures(uint64 pinBasedCtl, uint64 procBasedCtl,
                         uint64 entryCtl, uint64 exitCtl, uint64 basicCtl)
{
   unsigned memType;

   if ((VT_REQUIRED_PINBASED_CTLS &
        ~VTComputeMandatoryBits(pinBasedCtl, VT_REQUIRED_PINBASED_CTLS)) ||
       (VT_REQUIRED_PROCBASED_CTLS &
        ~VTComputeMandatoryBits(procBasedCtl, VT_REQUIRED_PROCBASED_CTLS)) ||
       (VT_REQUIRED_ENTRY_CTLS &
        ~VTComputeMandatoryBits(entryCtl, VT_REQUIRED_ENTRY_CTLS)) ||
       (VT_REQUIRED_EXIT_CTLS &
        ~VTComputeMandatoryBits(exitCtl, VT_REQUIRED_EXIT_CTLS))) {
      return FALSE;
   }

   memType = (unsigned)((basicCtl >> MSR_VMX_BASIC_MEMTYPE_SHIFT) &
                        MSR_VMX_BASIC_MEMTYPE_MASK);

   if (memType != MTRR_TYPE_WB) {
      return FALSE;
   }

   return TRUE;
}


/*
 *----------------------------------------------------------------------
 *
 * VT_RealModeSupportedFromFeatures --
 *
 *   Returns TRUE if the given VMX features provide real-address
 *   mode guest support.
 *
 *   Assumes that VT is supported.
 *
 *----------------------------------------------------------------------
 */
static INLINE Bool
VT_RealModeSupportedFromFeatures(uint64 secondary)
{
   return (HIDWORD(secondary) & VT_VMCS_2ND_VMEXEC_CTL_UNRESTRICTED) != 0;
}


#if !defined(USERLEVEL) && !defined(MONITOR_APP) /* { */
/*
 *----------------------------------------------------------------------
 *
 * VT_EnabledCPU --
 *
 *   Returns TRUE if VT is enabled on this CPU.  This function assumes
 *   that the processor is VT_Capable().
 *
 *----------------------------------------------------------------------
 */
static INLINE Bool
VT_EnabledCPU(void)
{
   return VT_EnabledFromFeatures(__GET_MSR(MSR_FEATCTL));
}


/*
 *----------------------------------------------------------------------
 *
 * VT_SupportedCPU --
 *
 *   Returns TRUE if this CPU has all of the features that we need to
 *   run our VT monitor.  This function assumes that the processor is
 *   VT_Capable().
 *
 *   Note that all currently shipping VT-capable processors meet these
 *   criteria, and that we do not expect any surprises in the field.
 *
 *----------------------------------------------------------------------
 */
static INLINE Bool
VT_SupportedCPU(void)
{
   if (__GET_MSR(MSR_VMX_BASIC) & MSR_VMX_BASIC_TRUE_CTLS) {
      return VT_SupportedFromFeatures(__GET_MSR(MSR_VMX_TRUE_PINBASED_CTLS),
                                      __GET_MSR(MSR_VMX_TRUE_PROCBASED_CTLS),
                                      __GET_MSR(MSR_VMX_TRUE_ENTRY_CTLS),
                                      __GET_MSR(MSR_VMX_TRUE_EXIT_CTLS),
                                      __GET_MSR(MSR_VMX_BASIC));
   } else {
      return VT_SupportedFromFeatures(__GET_MSR(MSR_VMX_PINBASED_CTLS),
                                      __GET_MSR(MSR_VMX_PROCBASED_CTLS),
                                      __GET_MSR(MSR_VMX_ENTRY_CTLS),
                                      __GET_MSR(MSR_VMX_EXIT_CTLS),
                                      __GET_MSR(MSR_VMX_BASIC));
   }
}

#endif /* } !defined(USERLEVEL) */

#if !defined(VMM) /* { */
/*
 *----------------------------------------------------------------------
 * VT_CapableCPU --
 *
 *   Verify that this CPU is VT-capable.
 *----------------------------------------------------------------------
 */
static INLINE Bool
VT_CapableCPU(void)
{
   return CPUID_ISSET(1, ECX, VMX, __GET_ECX_FROM_CPUID(1));
}
#endif /* } !defined(VMM) */


/*
 *----------------------------------------------------------------------
 *
 * VT_ConfigIndex --
 *
 *      Convert an MSR number to an index into the VTConfig array.
 *
 *----------------------------------------------------------------------
 */
static INLINE unsigned
VT_ConfigIndex(uint32 msrNum)
{
   ASSERT(msrNum >= MSR_VMX_BASIC &&
          msrNum < MSR_VMX_BASIC + NUM_VMX_MSRS);
   return msrNum - MSR_VMX_BASIC;
}


/*
 *----------------------------------------------------------------------
 *
 * VT_ConfigMSRNum --
 *
 *      Convert an index into the VTConfig array to an MSR number.
 *
 *----------------------------------------------------------------------
 */
static INLINE uint32
VT_ConfigMSRNum(unsigned index)
{
   ASSERT(index < NUM_VMX_MSRS);
   return MSR_VMX_BASIC + index;
}


#endif /* _X86VT_H_ */
