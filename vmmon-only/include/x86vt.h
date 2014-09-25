/*********************************************************
 * Copyright (C) 2004-2012 VMware, Inc. All rights reserved.
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

#define MSR_VMX_BASIC_VMCS_ID_SHIFT           0
#define MSR_VMX_BASIC_VMCS_ID_MASK            0xffffffff
#define MSR_VMX_BASIC_VMCS_SIZE_SHIFT         32
#define MSR_VMX_BASIC_VMCS_SIZE_MASK          0x1fff
#define MSR_VMX_BASIC_MEMTYPE_SHIFT           50
#define MSR_VMX_BASIC_MEMTYPE_MASK            0xf
#define MSR_VMX_BASIC_32BITPA                 (1ULL << 48)
#define MSR_VMX_BASIC_DUALVMM                 (1ULL << 49)
#define MSR_VMX_BASIC_ADVANCED_IOINFO         (1ULL << 54)
#define MSR_VMX_BASIC_TRUE_CTLS               (1ULL << 55)

#define MSR_VMX_MISC_TMR_RATIO_SHIFT          0
#define MSR_VMX_MISC_TMR_RATIO_MASK           0x1f
#define MSR_VMX_MISC_VMEXIT_SAVES_LMA         (1ULL << 5)
#define MSR_VMX_MISC_ACTSTATE_HLT             (1ULL << 6)
#define MSR_VMX_MISC_ACTSTATE_SHUTDOWN        (1ULL << 7)
#define MSR_VMX_MISC_ACTSTATE_SIPI            (1ULL << 8)
#define MSR_VMX_MISC_CR3_TARGETS_SHIFT        16
#define MSR_VMX_MISC_CR3_TARGETS_MASK         0x1ff
#define MSR_VMX_MISC_MAX_MSRS_SHIFT           25
#define MSR_VMX_MISC_MAX_MSRS_MASK            0x7
#define MSR_VMX_MISC_MSEG_ID_SHIFT            32
#define MSR_VMX_MISC_MSEG_ID_MASK             0xffffffff

#define MSR_VMX_VMCS_ENUM_MAX_INDEX_SHIFT     1
#define MSR_VMX_VMCS_ENUM_MAX_INDEX_MASK      0x1ff

#define MSR_VMX_EPT_VPID_EPTE_X                 (1ULL << 0)
#define MSR_VMX_EPT_VPID_GAW_48                 (1ULL << 6)
#define MSR_VMX_EPT_VPID_ETMT_UC                (1ULL << 8)
#define MSR_VMX_EPT_VPID_ETMT_WB                (1ULL << 14)
#define MSR_VMX_EPT_VPID_SP_2MB                 (1ULL << 16)
#define MSR_VMX_EPT_VPID_SP_1GB                 (1ULL << 17)
#define MSR_VMX_EPT_VPID_INVEPT                 (1ULL << 20)
#define MSR_VMX_EPT_ACCESS_DIRTY                (1ULL << 21)
#define MSR_VMX_EPT_VPID_INVEPT_EPT_CTX         (1ULL << 25)
#define MSR_VMX_EPT_VPID_INVEPT_GLOBAL          (1ULL << 26)
#define MSR_VMX_EPT_VPID_INVVPID                (1ULL << 32)
#define MSR_VMX_EPT_VPID_INVVPID_ADDR           (1ULL << 40)
#define MSR_VMX_EPT_VPID_INVVPID_VPID_CTX       (1ULL << 41)
#define MSR_VMX_EPT_VPID_INVVPID_ALL_CTX        (1ULL << 42)
#define MSR_VMX_EPT_VPID_INVVPID_VPID_CTX_LOCAL (1ULL << 43)

#define VT_VMCS_STANDARD_TAG           0x00000000

/*
 * Structure of VMCS Component Encoding (Table 20-16)
 */
#define VT_ENCODING_ACCESS_HIGH        0x00000001
#define VT_ENCODING_INDEX_MASK         0x000003fe
#define VT_ENCODING_INDEX_SHIFT                 1
#define VT_ENCODING_TYPE_MASK          0x00000c00
#define VT_ENCODING_TYPE_SHIFT                 10
#define VT_ENCODING_TYPE_CTL                    0
#define VT_ENCODING_TYPE_RODATA                 1
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

/* 16-bit control field: table H-1 */
#define VT_VMCS_VPID                   0x00000000
#define VT_VMCS_PI_NOTIFY              0x00000002

/* 16-bit guest state: table H-2 */
#define VT_VMCS_ES                     0x00000800
#define VT_VMCS_CS                     0x00000802
#define VT_VMCS_SS                     0x00000804
#define VT_VMCS_DS                     0x00000806
#define VT_VMCS_FS                     0x00000808
#define VT_VMCS_GS                     0x0000080A
#define VT_VMCS_LDTR                   0x0000080C
#define VT_VMCS_TR                     0x0000080E
#define VT_VMCS_INTR_STATUS            0x00000810

/* 16-bit host state: table H-3 */
#define VT_VMCS_HOST_ES                0x00000C00
#define VT_VMCS_HOST_CS                0x00000C02
#define VT_VMCS_HOST_SS                0x00000C04
#define VT_VMCS_HOST_DS                0x00000C06
#define VT_VMCS_HOST_FS                0x00000C08
#define VT_VMCS_HOST_GS                0x00000C0A
#define VT_VMCS_HOST_TR                0x00000C0C

/* 64-bit control fields: table H-4 */
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

/* 64-bit read-only data field: table H-5 */
#define VT_VMCS_PHYSADDR               0x00002400

/* 64-bit guest state: table H-6 */
#define VT_VMCS_LINK_PTR               0x00002800
#define VT_VMCS_DEBUGCTL               0x00002802
#define VT_VMCS_PAT                    0x00002804
#define VT_VMCS_EFER                   0x00002806
#define VT_VMCS_PGC                    0x00002808
#define VT_VMCS_PDPTE0                 0x0000280A
#define VT_VMCS_PDPTE1                 0x0000280C
#define VT_VMCS_PDPTE2                 0x0000280E
#define VT_VMCS_PDPTE3                 0x00002810

/* 64-bit host state: table H-7 */
#define VT_VMCS_HOST_PAT               0x00002C00
#define VT_VMCS_HOST_EFER              0x00002C02
#define VT_VMCS_HOST_PGC               0x00002C04

/* 32-bit control fields: table H-8 */
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

/* 32-bit read-only data fields: table H-9 */
#define VT_VMCS_VMINSTR_ERR            0x00004400
#define VT_VMCS_EXIT_REASON            0x00004402
#define VT_VMCS_EXIT_INTR_INFO         0x00004404
#define VT_VMCS_EXIT_INTR_ERR          0x00004406
#define VT_VMCS_IDTVEC_INFO            0x00004408
#define VT_VMCS_IDTVEC_ERR             0x0000440A
#define VT_VMCS_INSTRLEN               0x0000440C
#define VT_VMCS_INSTR_INFO             0x0000440E

/* 32-bit guest state: table H-10 */
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

/* 32-bit host state: table H-11 */
#define VT_VMCS_HOST_SYSENTER_CS       0x00004C00

/* natural-width control fields: table H-12 */
#define VT_VMCS_CR0_GHMASK             0x00006000
#define VT_VMCS_CR4_GHMASK             0x00006002
#define VT_VMCS_CR0_SHADOW             0x00006004
#define VT_VMCS_CR4_SHADOW             0x00006006
#define VT_VMCS_CR3_TARGVAL0           0x00006008
#define VT_VMCS_CR3_TARGVAL1           0x0000600A
#define VT_VMCS_CR3_TARGVAL2           0x0000600C
#define VT_VMCS_CR3_TARGVAL3           0x0000600E

/* natural-width read-only data fields: table H-13 */
#define VT_VMCS_EXIT_QUAL              0x00006400
#define VT_VMCS_IO_ECX                 0x00006402
#define VT_VMCS_IO_ESI                 0x00006404
#define VT_VMCS_IO_EDI                 0x00006406
#define VT_VMCS_IO_EIP                 0x00006408
#define VT_VMCS_LINEAR_ADDR            0x0000640A

/* natural-width guest state: table H-14 */
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

/* natural-width host state: table H-15 */
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
 * Bits for execution control
 */
#define VT_VMCS_PIN_VMEXEC_CTL_EXTINT_EXIT  0x00000001
#define VT_VMCS_PIN_VMEXEC_CTL_NMI_EXIT     0x00000008
#define VT_VMCS_PIN_VMEXEC_CTL_VNMI         0x00000020
#define VT_VMCS_PIN_VMEXEC_CTL_TIMER        0x00000040
#define VT_VMCS_PIN_VMEXEC_CTL_POSTED_INTR  0x00000080

#define VT_VMCS_CPU_VMEXEC_CTL_VINTR_WINDOW 0x00000004
#define VT_VMCS_CPU_VMEXEC_CTL_TSCOFF       0x00000008
#define VT_VMCS_CPU_VMEXEC_CTL_HLT          0x00000080
#define VT_VMCS_CPU_VMEXEC_CTL_INVLPG       0x00000200
#define VT_VMCS_CPU_VMEXEC_CTL_MWAIT        0x00000400
#define VT_VMCS_CPU_VMEXEC_CTL_RDPMC        0x00000800
#define VT_VMCS_CPU_VMEXEC_CTL_RDTSC        0x00001000
#define VT_VMCS_CPU_VMEXEC_CTL_LDCR3        0x00008000
#define VT_VMCS_CPU_VMEXEC_CTL_STCR3        0x00010000
#define VT_VMCS_CPU_VMEXEC_CTL_LDCR8        0x00080000
#define VT_VMCS_CPU_VMEXEC_CTL_STCR8        0x00100000
#define VT_VMCS_CPU_VMEXEC_CTL_TPR_SHADOW   0x00200000
#define VT_VMCS_CPU_VMEXEC_CTL_VNMI_WINDOW  0x00400000
#define VT_VMCS_CPU_VMEXEC_CTL_MOVDR        0x00800000
#define VT_VMCS_CPU_VMEXEC_CTL_IO           0x01000000
#define VT_VMCS_CPU_VMEXEC_CTL_IOBITMAP     0x02000000
#define VT_VMCS_CPU_VMEXEC_CTL_MTF          0x08000000
#define VT_VMCS_CPU_VMEXEC_CTL_MSRBITMAP    0x10000000
#define VT_VMCS_CPU_VMEXEC_CTL_MONITOR      0x20000000
#define VT_VMCS_CPU_VMEXEC_CTL_PAUSE        0x40000000
#define VT_VMCS_CPU_VMEXEC_CTL_USE_2ND      0x80000000

#define VT_VMCS_2ND_VMEXEC_CTL_APIC         0x00000001
#define VT_VMCS_2ND_VMEXEC_CTL_EPT          0x00000002
#define VT_VMCS_2ND_VMEXEC_CTL_DT           0x00000004
#define VT_VMCS_2ND_VMEXEC_CTL_RDTSCP       0x00000008
#define VT_VMCS_2ND_VMEXEC_CTL_X2APIC       0x00000010
#define VT_VMCS_2ND_VMEXEC_CTL_VPID         0x00000020
#define VT_VMCS_2ND_VMEXEC_CTL_WBINVD       0x00000040
#define VT_VMCS_2ND_VMEXEC_CTL_UNRESTRICTED 0x00000080
#define VT_VMCS_2ND_VMEXEC_CTL_APICREG      0x00000100
#define VT_VMCS_2ND_VMEXEC_CTL_VINTR        0x00000200
#define VT_VMCS_2ND_VMEXEC_CTL_PAUSE_LOOP   0x00000400
#define VT_VMCS_2ND_VMEXEC_CTL_RDRAND       0x00000800
#define VT_VMCS_2ND_VMEXEC_CTL_INVPCID      0x00001000
#define VT_VMCS_2ND_VMEXEC_CTL_VMFUNC       0x00002000

/*
 * Bits for entry control.
 */
#define VT_VMCS_VMENTRY_CTL_LOAD_DEBUGCTL     0x00000004
#define VT_VMCS_VMENTRY_CTL_LONGMODE          0x00000200
#define VT_VMCS_VMENTRY_CTL_ENTRY_TO_SMM      0x00000400
#define VT_VMCS_VMENTRY_CTL_SMM_TEARDOWN      0x00000800
#define VT_VMCS_VMENTRY_CTL_LOAD_PGC          0x00002000
#define VT_VMCS_VMENTRY_CTL_LOAD_PAT          0x00004000
#define VT_VMCS_VMENTRY_CTL_LOAD_EFER         0x00008000

/*
 * Bits for exit control.
 */
#define VT_VMCS_VMEXIT_CTL_SAVE_DEBUGCTL     0x00000004
#define VT_VMCS_VMEXIT_CTL_LONGMODE          0x00000200
#define VT_VMCS_VMEXIT_CTL_LOAD_PGC          0x00001000
#define VT_VMCS_VMEXIT_CTL_INTRACK           0x00008000
#define VT_VMCS_VMEXIT_CTL_SAVE_PAT          0x00040000
#define VT_VMCS_VMEXIT_CTL_LOAD_PAT          0x00080000
#define VT_VMCS_VMEXIT_CTL_SAVE_EFER         0x00100000
#define VT_VMCS_VMEXIT_CTL_LOAD_EFER         0x00200000
#define VT_VMCS_VMEXIT_CTL_SAVE_TIMER        0x00400000


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
#define VT_VMCS_PENDDBG_MBZ        0xffffaff0

/* Exception error must-be-zero bits for VMEntry */
#define VT_XCP_ERR_MBZ             0xffff8000

/* Exit reasons: table I-1 */
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

/*
 * VT synthesized exit reasons:
 *
 * Faked up reasons, not overlapping with any real exit codes, which
 * help save repeated VMREADS in HVExit and HVTryFastExit of
 * VT_VMCS_EXIT_INTR_INFO to extract the TYPE_MASK and VECTOR_MASK.
 * See HVExitGlue.
 *
 * ASSERT_ON_COMPILEs to ensure that these codes don't overlap
 * real codes in hardware can be found in HVExitGlue.
 */
#define VT_EXITREASON_SYNTH_BASE     61
#define VT_EXITREASON_SYNTH_IRET     61
#define VT_EXITREASON_SYNTH_NMI      62
#define VT_EXITREASON_SYNTH_ICEBP    63
#define VT_EXITREASON_SYNTH_EXC_BASE 64
#define VT_EXITREASON_SYNTH_MAX      95

#define VT_EXITREASON_SYNTH_EXC(gatenum) \
        (VT_EXITREASON_SYNTH_EXC_BASE + gatenum) /* 0-31 */

#define VT_EXITREASON_VMENTRYFAIL   (1U << 31)

/* Instruction error codes:  table 5-1 (volume 2) */
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
