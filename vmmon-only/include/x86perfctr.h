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
 * x86perfctr.h --
 *
 */

#ifndef _X86PERFCTR_H_
#define _X86PERFCTR_H_

#define INCLUDE_ALLOW_USERLEVEL

#define INCLUDE_ALLOW_VMKERNEL
#define INCLUDE_ALLOW_MODULE
#define INCLUDE_ALLOW_VMCORE
#define INCLUDE_ALLOW_VMMON

#include "includeCheck.h"
#include "vm_asm.h"

#define PERFCTR_PENTIUM4_NUM_COUNTERS            18
#define PERFCTR_PENTIUM4_NUM_COUNTERS_WITH_L3    26
#define PERFCTR_AMD_NUM_COUNTERS                 4 
#define PERFCTR_BD_NUM_COUNTERS                  6
#define PERFCTR_P6_NUM_COUNTERS                  2
#define PERFCTR_NEHALEM_NUM_GEN_COUNTERS         4
#define PERFCTR_NEHALEM_NUM_FIXED_COUNTERS       3
#define PERFCTR_SANDYBRIDGE_NUM_GEN_COUNTERS     8 /* When HT is disabled */
#define PERFCTR_CORE_NUM_ARCH_EVENTS             7
#define PERFCTR_PENTIUM4_VAL_MASK                0xffffffffffLL
#define PERFCTR_AMD_VAL_MASK                     0xffffffffffffLL
#define PERF_EVENT_NAME_LEN                      64
/*
 * Even though the performance counters in P6 are 40 bits,
 * we can only write to the lower 32 bits.  Bit 31 is
 * used to sign-extend the upper 8 bits.
 */
#define PERFCTR_P6_VAL_MASK                      0xffffffffffLL
#define PERFCTR_P6_WRITE_MASK                    0xffffffff

/*
 * Performance counter width is determined at runtime in CORE.
 * But the writables bits are fixed. we can only write to the 
 * lower 32 bits.  Bit 31 is used to sign-extend the upper 8 bits.
 */
#define PERFCTR_CORE_WRITE_MASK                  0xffffffff

/* Common Event Selection MSR bits. */
#define PERFCTR_CPU_EVENT_MASK                   0x000000ff
#define PERFCTR_CPU_EVENT_SHIFT                  0
#define PERFCTR_CPU_UNIT_MASK                    0x0000ff00
#define PERFCTR_CPU_UNIT_SHIFT                   8
#define PERFCTR_CPU_USER_MODE                    0x00010000
#define PERFCTR_CPU_KERNEL_MODE                  0x00020000
#define PERFCTR_CPU_EDGE_DETECT                  0x00040000
#define PERFCTR_CPU_PIN_CONTROL                  0x00080000
#define PERFCTR_CPU_APIC_INTR                    0x00100000
#define PERFCTR_CPU_ENABLE                       0x00400000
#define PERFCTR_CPU_INVERT_COUNTER_MASK          0x00800000
#define PERFCTR_CPU_COUNTER_MASK                 0xff000000
#define PERFCTR_CPU_COUNTER_MASK_SHIFT           24

/*
 * ----------------------------------------------------------------------
 *
 * AMD K8
 *
 * ----------------------------------------------------------------------
 */

/* AMD Event Selection MSRs */
#define PERFCTR_AMD_EVENT_MASK                   PERFCTR_CPU_EVENT_MASK
#define PERFCTR_AMD_EXT_EVENT_MASK               (0xfULL << 32)
#define PERFCTR_AMD_EVENT_SHIFT                  PERFCTR_CPU_EVENT_SHIFT
#define PERFCTR_AMD_UNIT_MASK                    PERFCTR_CPU_UNIT_MASK
#define PERFCTR_AMD_UNIT_SHIFT                   PERFCTR_CPU_UNIT_SHIFT
#define PERFCTR_AMD_USER_MODE                    PERFCTR_CPU_USER_MODE
#define PERFCTR_AMD_KERNEL_MODE                  PERFCTR_CPU_KERNEL_MODE
#define PERFCTR_AMD_EDGE_DETECT                  PERFCTR_CPU_EDGE_DETECT
#define PERFCTR_AMD_PIN_CONTROL                  PERFCTR_CPU_PIN_CONTROL
#define PERFCTR_AMD_APIC_INTR                    PERFCTR_CPU_APIC_INTR
#define PERFCTR_AMD_ENABLE                       PERFCTR_CPU_ENABLE
#define PERFCTR_AMD_INVERT_COUNTER_MASK          PERFCTR_CPU_INVERT_COUNTER_MASK
#define PERFCTR_AMD_COUNTER_MASK                 PERFCTR_CPU_COUNTER_MASK
#define PERFCTR_AMD_COUNTER_MASK_SHIFT           PERFCTR_CPU_COUNTER_MASK_SHIFT
#define PERFCTR_AMD_SHIFT_BY_UNITMASK(e)         ((e) << 8 )
#define PERFCTR_AMD_EVTSEL_HOST                  (CONST64U(1) << 41)
#define PERFCTR_AMD_EVTSEL_GUEST                 (CONST64U(1) << 40)

/* AMD Performance Counter MSR Definitions */
#define PERFCTR_AMD_PERFEVTSEL0_ADDR             0xC0010000
#define PERFCTR_AMD_PERFCTR0_ADDR                0xC0010004
#define PERFCTR_BD_BASE_ADDR                     0xC0010200
#define PERFCTR_BD_EVENTSEL                      0
#define PERFCTR_BD_CTR                           1
#define PERFCTR_BD_MSR_STRIDE                    2

/* AMD Clocks */
#define PERFCTR_AMD_CPU_CLK_UNHALTED                           0x76

/* AMD Load/Store unit events */
#define PERFCTR_AMD_SEGMENT_REGISTER_LOADS                     0x20 
#define PERFCTR_AMD_LS_BUFFER2_FULL                            0x23

/*
 * Event 0x2b counts SMIs on Opteron Rev.G (with upatch) and on
 * GH >= Rev.B0 without patches. GH Rev.A has no such capability.
 */
#define PERFCTR_AMD_SMI_COUNT                                  0x2b

/* AMD Data Cache Events */
#define PERFCTR_AMD_DATA_CACHE_ACCESSES                        0x40
#define PERFCTR_AMD_DATA_CACHE_MISSES                          0x41
#define PERFCTR_AMD_DATA_CACHE_REFILLS_FROM_L2_OR_SYSTEM       0x42
#define PERFCTR_AMD_DATA_CACHE_REFILLS_FROM_SYSTEM             0x43
#define PERFCTR_AMD_DATA_CACHE_LINES_EVICTED                   0x44
#define PERFCTR_AMD_L1_DTLB_MISS_AND_L2_DTLB_HIT               0x45
#define PERFCTR_AMD_L1_DTLB_AND_L2_DTLB_MISS                   0x46
#define PERFCTR_AMD_MISALIGNED_ACCESSES                        0x47
#define PERFCTR_AMD_PREFETCH_INSTRS_DISPATCHED                 0x4b
#define PERFCTR_AMD_DCACHE_MISSES_BY_LOCKED_INSTR              0x4c

/* AMD L2 Cache */
#define PERFCTR_AMD_REQUESTS_TO_L2                             0x7d
#define PERFCTR_AMD_L2_MISS                                    0x7e
#define PERFCTR_AMD_L2_FILL_WRITEBACK                          0x7f

/* AMD L3 Cache */
#define PERFCTR_AMD_REQUESTS_TO_L3                             (0x4e0 | 0xf000)
#define PERFCTR_AMD_L3_MISS                                    (0x4e1 | 0xf000)

/* AMD Instruction Cache Events */

#define PERFCTR_AMD_INSTR_FETCHES                              0x80
#define PERFCTR_AMD_INSTR_MISSES                               0x81
#define PERFCTR_AMD_INSTR_REFILLS_FROM_L2                      0x82
#define PERFCTR_AMD_INSTR_REFILLS_FROM_SYSTEM                  0x83
#define PERFCTR_AMD_L1_ITLB_MISS_L2_ITLB_HIT                   0x84
#define PERFCTR_AMD_L1_ITLB_MISS_L2_ITLB_MISS                  0x85
#define PERFCTR_AMD_INSTR_FETCH_STALL                          0x87

/* AMD Execution Unit Events */
#define PERFCTR_AMD_RET_INSTR                                  0xc0
#define PERFCTR_AMD_RET_UOPS                                   0xc1
#define PERFCTR_AMD_RET_BRANCH_INSTR                           0xc2
#define PERFCTR_AMD_RET_MISPRED_BRANCH_INSTR                   0xc3
#define PERFCTR_AMD_RET_TAKEN_BRANCH_INSTR                     0xc4
#define PERFCTR_AMD_RET_TAKEN_BRANCH_INSTR_MISPRED             0xc5
#define PERFCTR_AMD_RET_FAR_CONTROL_TRANSFERS                  0xc6
#define PERFCTR_AMD_RET_BRANCH_RESYNCS                         0xc7
#define PERFCTR_AMD_RET_NEAR_RETURNS                           0xc8
#define PERFCTR_AMD_RET_NEAR_RETURNS_MISPRED                   0xc9
#define PERFCTR_AMD_RET_INDIRECT_BRANCHES_MISPRED              0xca
#define PERFCTR_AMD_RET_FASTPATH_DOUBLE_OP_INSTR               0xcc
#define PERFCTR_AMD_INT_MASKED_CYCLES                          0xcd
#define PERFCTR_AMD_INT_MASKED_CYCLES_WITH_INT_PEND            0xce

#define PERFCTR_AMD_INT_MASKED_COUNT \
  ((0xcd) | PERFCTR_AMD_EDGE_DETECT)

#define PERFCTR_AMD_INT_MASKED_COUNT_WITH_INT_PEND \
  ((0xce) | PERFCTR_AMD_EDGE_DETECT)

#define PERFCTR_AMD_INT_TAKEN                                       0xcf
#define PERFCTR_AMD_DECODER_EMPTY_CYCLES                            0xd0
#define PERFCTR_AMD_DISPATCH_STALLS                                 0xd1
#define PERFCTR_AMD_DISPATCH_MISPRED_BRANCH_STALL_CYCLES            0xd2
#define PERFCTR_AMD_DISPATCH_SERIALIZATION_STALL_CYCLES             0xd3
#define PERFCTR_AMD_DISPATCH_SEGMENTLOAD_STALL_CYCLES               0xd4
#define PERFCTR_AMD_DISPATCH_REORDER_BUFFER_FULL_STALL_CYCLES       0xd5
#define PERFCTR_AMD_DISPATCH_RESERVATION_STATION_FULL_STALL_CYCLES  0xd6
#define PERFCTR_AMD_DISPATCH_LS_FULL_STALL_CYCLES                   0xd8
#define PERFCTR_AMD_DISPATCH_WAIT_ALLQUIET_STALL_CYCLES             0xd9
#define PERFCTR_AMD_DISPATCH_FAR_XFER_OR_RESYNC_RETIRE_STALL_CYCLES 0xda

/* AMD Memory Controller Events */

#define PERFCTR_AMD_MEM_CTRL_PAGE_TABLE_OVERFLOWS    		    0xe1
#define PERFCTR_AMD_CPU_IO_REQUESTS_TO_MEMORY_IO                    0xe9
#define PERFCTR_AMD_PROBE_RESPONSE_AND_UPSTREAM_REQ                 0xec 
 
/* AMD HyperTransport Interface Events */

#define PERFCTR_AMD_HT_L0_TX_BW 				    0xf6 
#define PERFCTR_AMD_HT_L1_TX_BW 				    0xf7 
#define PERFCTR_AMD_HT_L2_TX_BW 				    0xf8

/*
 * ----------------------------------------------------------------------
 *
 * Intel P6 family (excluding newer Core architecture)
 *
 * ----------------------------------------------------------------------
 */

/*
 * Event numbers for P6 Family Processors.
 */

/* P6 Data Cache Unit (DCU) */
#define PERFCTR_P6_DATA_MEM_REFS                 0x00000043  
#define PERFCTR_P6_DCU_LINES_IN                  0x00000045
#define PERFCTR_P6_DCU_M_LINES_IN                0x00000046 
#define PERFCTR_P6_DCU_MISS_OUTSTANDING          0x00000048

/* P6 Instruction Fetch Unit (IFU) */
#define PERFCTR_P6_IFU_IFETCH                    0x00000080
#define PERFCTR_P6_IFU_IFETCH_MISS               0x00000081
#define PERFCTR_P6_ITLB_MISS                     0x00000085
#define PERFCTR_P6_IFU_MEM_STALL                 0x00000086
#define PERFCTR_P6_ILD_STALL                     0x00000087

/* P6 L2 Cache */
#define PERFCTR_P6_L2_IFETCH                     0x00000f28
#define PERFCTR_P6_L2_LD                         0x00000f29
#define PERFCTR_P6_L2_ST                         0x00000f2a
#define PERFCTR_P6_L2_LINES_IN                   0x00000024
#define PERFCTR_P6_L2_LINES_OUT                  0x00000026
#define PERFCTR_P6_L2_LINES_INM                  0x00000025
#define PERFCTR_P6_L2_LINES_OUTM                 0x00000027
#define PERFCTR_P6_L2_RQSTS                      0x00000f2e
#define PERFCTR_P6_L2_ADS                        0x00000021
#define PERFCTR_P6_L2_DBUS_BUSY_RD               0x00000023

/* P6 External Bus Logic */
#define PERFCTR_P6_BUSDRDY_CLOCKS_SELF           0x00000062
#define PERFCTR_P6_BUSDRDY_CLOCKS_ANY            0x00002062
#define PERFCTR_P6_BUS_LOCK_CLOCKS_SELF          0x00000063
#define PERFCTR_P6_BUS_LOCK_CLOCKS_ANY           0x00002063
#define PERFCTR_P6_BUS_REQ_OUTSTANDING           0x00000060
#define PERFCTR_P6_BUS_TRAN_BRD_SELF             0x00000065
#define PERFCTR_P6_BUS_TRAN_BRD_ANY              0x00002065
#define PERFCTR_P6_BUS_TRAN_RFO_SELF             0x00000066
#define PERFCTR_P6_BUS_TRAN_RFO_ANY              0x00002066
#define PERFCTR_P6_BUS_TRAN_WB_SELF              0x00000067
#define PERFCTR_P6_BUS_TRAN_WB_ANY               0x00002067
#define PERFCTR_P6_BUS_TRAN_IFETCH_SELF          0x00000068
#define PERFCTR_P6_BUS_TRAN_IFETCH_ANY           0x00002068
#define PERFCTR_P6_BUS_TRAN_INVAL_SELF           0x00000069
#define PERFCTR_P6_BUS_TRAN_INVAL_ANY            0x00002069
#define PERFCTR_P6_BUS_TRAN_PWR_SELF             0x0000006a
#define PERFCTR_P6_BUS_TRAN_PWR_ANY              0x0000206a
#define PERFCTR_P6_BUS_TRAN_P_SELF               0x0000006b
#define PERFCTR_P6_BUS_TRAN_P_ANY                0x0000206b
#define PERFCTR_P6_BUS_TRAN_IO_SELF              0x0000006c
#define PERFCTR_P6_BUS_TRAN_IO_ANY               0x0000206c
#define PERFCTR_P6_BUS_TRAN_DEF_SELF             0x0000006d
#define PERFCTR_P6_BUS_TRAN_DEF_ANY              0x0000206d
#define PERFCTR_P6_BUS_TRAN_DEF_SELF             0x0000006d
#define PERFCTR_P6_BUS_TRAN_DEF_ANY              0x0000206d
#define PERFCTR_P6_BUS_TRAN_BURST_SELF           0x0000006e
#define PERFCTR_P6_BUS_TRAN_BURST_ANY            0x0000206e
#define PERFCTR_P6_BUS_TRAN_ANY_SELF             0x00000070
#define PERFCTR_P6_BUS_TRAN_ANY_ANY              0x00002070
#define PERFCTR_P6_BUS_TRAN_MEM_SELF             0x0000006f
#define PERFCTR_P6_BUS_TRAN_MEM_ANY              0x0000206f
#define PERFCTR_P6_BUS_TRAN_RCV                  0x00000064
#define PERFCTR_P6_BUS_BNR_DRV                   0x00000061
#define PERFCTR_P6_BUS_HIT_DRV                   0x0000007a
#define PERFCTR_P6_BUS_HITM_DRV                  0x0000007b
#define PERFCTR_P6_BUS_SNOOP_STALL               0x0000007E

/* P6 Floating-Point Unit */
#define PERFCTR_P6_FLOPS                         0x000000c1
#define PERFCTR_P6_FP_COMP_OPS_EXE               0x00000010
#define PERFCTR_P6_FP_ASSIST                     0x00000011
#define PERFCTR_P6_MUL                           0x00000012
#define PERFCTR_P6_DIV                           0x00000013
#define PERFCTR_P6_CYCLES_DIV_BUSY               0x00000014

/* P6 Memory Ordering */
#define PERFCTR_P6_LD_BLOCKS                     0x00000003
#define PERFCTR_P6_SB_DRAINS                     0x00000004
#define PERFCTR_P6_MISALIGN_MEM_REF              0x00000005
#define PERFCTR_P6_EMON_KNI_PREF_DISPATCHED_NTA  0x00000007
#define PERFCTR_P6_EMON_KNI_PREF_DISPATCHED_T1   0x00000107
#define PERFCTR_P6_EMON_KNI_PREF_DISPATCHED_T2   0x00000207
#define PERFCTR_P6_EMON_KNI_PREF_DISPATCHED_WOS  0x00000307
#define PERFCTR_P6_EMON_KNI_PREF_MISS_NTA        0x0000004b
#define PERFCTR_P6_EMON_KNI_PREF_MISS_T1         0x0000014b
#define PERFCTR_P6_EMON_KNI_PREF_MISS_T2         0x0000024b
#define PERFCTR_P6_EMON_KNI_PREF_MISS_WOS        0x0000034b

/* P6 Instruction Decoding and Retirement */
#define PERFCTR_P6_INST_RETIRED                  0x000000c0
#define PERFCTR_P6_UOPS_RETIRED                  0x000000c2
#define PERFCTR_P6_INST_DECODED                  0x000000d0
#define PERFCTR_P6_EMON_KNI_INST_RETIRED         0x000000d8
#define PERFCTR_P6_EMON_KNI_INST_RETIRED_SC      0x000001d8
#define PERFCTR_P6_EMON_KNI_COMP_INST_RETIRED    0x000000d9
#define PERFCTR_P6_EMON_KNI_COMP_INST_RETIRED_SC 0x000001d9

/* P6 Interrupts */
#define PERFCTR_P6_HW_INT_RX                     0x000000c8
#define PERFCTR_P6_CYCLES_INT_MASKED             0x000000c6
#define PERFCTR_P6_CYCLES_INT_PENDING_AND_MASKED 0x000000c7

/* P6 Branches */
#define PERFCTR_P6_BR_INST_RETIRED               0x000000c4
#define PERFCTR_P6_BR_MISS_PRED_RETIRED          0x000000c5
#define PERFCTR_P6_BR_TAKEN_RETIRED              0x000000c9
#define PERFCTR_P6_BR_MISS_PRED_TAKEN_RET        0x000000ca
#define PERFCTR_P6_BR_INST_DECODED               0x000000e0
#define PERFCTR_P6_BTB_MISSES                    0x000000e2
#define PERFCTR_P6_BR_BOGUS                      0x000000e4
#define PERFCTR_P6_BACLEARS                      0x000000e6

/* P6 Stalls */
#define PERFCTR_P6_RESOURCE_STALLS               0x000000a2
#define PERFCTR_P6_PARTIAL_RAT_CLEARS            0x000000d2

/* P6 Segment Register Loads */
#define PERFCTR_P6_SEGMENT_REG_LOADS             0x00000006

/* P6 Clocks */
#define PERFCTR_P6_CPU_CLK_UNHALTED              0x00000079

/* P6 MMX Unit */
#define PERFCTR_P6_MMX_INSTR_EXEC                0x000000b0
#define PERFCTR_P6_MMX_SAT_INSTR_EXEC            0x000000b1
#define PERFCTR_P6_MMX_UOPS_EXEC                 0x000000b2
#define PERFCTR_P6_MMX_INSTR_TYPE_EXEC_PK_MUL    0x000001b3
#define PERFCTR_P6_MMX_INSTR_TYPE_EXEC_PK_SHIFT  0x000002b3
#define PERFCTR_P6_MMX_INSTR_TYPE_EXEC_PK_OP     0x000004b3
#define PERFCTR_P6_MMX_INSTR_TYPE_EXEC_UNPK_OP   0x000008b3
#define PERFCTR_P6_MMX_INSTR_TYPE_EXEC_PK_LOG    0x000010b3
#define PERFCTR_P6_MMX_INSTR_TYPE_EXEC_PK_ARITH  0x000020b3
#define PERFCTR_P6_FP_MMX_TRANS_TO               0x000000cc
#define PERFCTR_P6_FP_MMX_TRANS_FROM             0x000001cc
#define PERFCTR_P6_FP_MMX_ASSIST                 0x000000cd
#define PERFCTR_P6_FP_MMX_INSTR_RET              0x000000ce

/* P6 Segment Register Renaming */
#define PERFCTR_P6_SEG_RENAME_STALLS_ES          0x000001d4
#define PERFCTR_P6_SEG_RENAME_STALLS_DS          0x000002d4
#define PERFCTR_P6_SEG_RENAME_STALLS_FS          0x000004d4
#define PERFCTR_P6_SEG_RENAME_STALLS_GS          0x000008d4
#define PERFCTR_P6_SEG_RENAME_STALLS_ANY         0x00000fd4
#define PERFCTR_P6_SEG_RENAMES_ES                0x000001d5
#define PERFCTR_P6_SEG_RENAMES_DS                0x000002d5
#define PERFCTR_P6_SEG_RENAMES_FS                0x000004d5
#define PERFCTR_P6_SEG_RENAMES_GS                0x000008d5
#define PERFCTR_P6_SEG_RENAMES_ANY               0x00000fd5
#define PERFCTR_P6_RET_SEG_RENAMES               0x000000d6

/*
 * P6 Event Selection MSRs
 */

#define PERFCTR_P6_EVENT_MASK                    0x000000ff
#define PERFCTR_P6_EVENT_SHIFT                   0
#define PERFCTR_P6_UNIT_MASK                     0x0000ff00
#define PERFCTR_P6_UNIT_SHIFT                    8
#define PERFCTR_P6_USER_MODE                     0x00010000
#define PERFCTR_P6_KERNEL_MODE                   0x00020000
#define PERFCTR_P6_EDGE_DETECT                   0x00040000
#define PERFCTR_P6_PIN_CONTROL                   0x00080000
#define PERFCTR_P6_APIC_INTR                     0x00100000
#define PERFCTR_P6_ENABLE                        0x00400000
#define PERFCTR_P6_INVERT_COUNTER_MASK           0x00800000
#define PERFCTR_P6_COUNTER_MASK                  0xff000000        
#define PERFCTR_P6_COUNTER_MASK_SHIFT            24
#define PERFCTR_P6_SHIFT_BY_UNITMASK(e)          (e << 8)

/*
 * P6 Performance Counter MSR Addresses
 */
#define PERFCTR_P6_PERFEVTSEL0_ADDR          	 0x00000186
#define PERFCTR_P6_PERFCTR0_ADDR                 0x000000c1

/*
 * ----------------------------------------------------------------------
 *
 * Intel Core architecture
 *
 *    Use CPUID 0xa to get perf capabilities.  Some (7) events are
 *    architectural; most are version-specific.
 *
 *    V1: Yonah, V2: Merom, V2+: Penryn
 *
 *    V1 is similar to P6, with some additions:
 *       - Global control MSR
 *    V2 introduces:
 *       - 3 fixed counters
 *       - Ability to freeze a counter before PMI delivery.
 *       - Freeze during SMI (Penryn+ only)
 *       - VMCS global enable (Penryn+ only)
 *
 * ----------------------------------------------------------------------
 */
#define PERFCTR_CORE_PERFCTR0_ADDR               0x0c1
#define PERFCTR_CORE_PERFEVTSEL0_ADDR            0x186
#define PERFCTR_CORE_FIXED_CTR0_ADDR             0x309
#define PERFCTR_CORE_FIXED_CTR_CTRL_ADDR         0x38d
#define PERFCTR_CORE_FIXED_CTR_CTRL_PMI_MASK     0x888
#define PERFCTR_CORE_GLOBAL_STATUS_ADDR          0x38e
#define PERFCTR_CORE_GLOBAL_CTRL_ADDR            0x38f
#define PERFCTR_CORE_GLOBAL_OVF_CTRL_ADDR        0x390
#define PERFCTR_CORE_PERFCTR0_FULL_WIDTH_ADDR    0x4c1
#define PERFCTR_CORE_GLOBAL_PMC0_ENABLE          0x1
#define PERFCTR_CORE_GLOBAL_PMC1_ENABLE          0x2
#define PERFCTR_CORE_GLOBAL_FIXED_ENABLE         0x700000000ULL
#define PERFCTR_CORE_USER_MODE                   PERFCTR_CPU_USER_MODE
#define PERFCTR_CORE_KERNEL_MODE                 PERFCTR_CPU_KERNEL_MODE
#define PERFCTR_CORE_APIC_INTR                   PERFCTR_CPU_APIC_INTR
#define PERFCTR_CORE_ENABLE                      PERFCTR_CPU_ENABLE
#define PERFCTR_CORE_ANYTHREAD                   0x00200000
#define PERFCTR_CORE_SHIFT_BY_UNITMASK(e)        ((e) << 8)
#define PERFCTR_CORE_FIXED_CTR0_PMC              0x40000000
#define PERFCTR_CORE_FIXED_CTR1_PMC              0x40000001
#define PERFCTR_CORE_FIXED_PMI_MASKn(n)          (0x8U << ((n) * 4))
#define PERFCTR_CORE_FIXED_ANY_MASKn(n)          (0x4U << ((n) * 4))
#define PERFCTR_CORE_FIXED_KERNEL_MASKn(n)       (0x1U << ((n) * 4))
#define PERFCTR_CORE_FIXED_USER_MASKn(n)         (0x2U << ((n) * 4))
#define PERFCTR_CORE_FIXED_ENABLE_MASKn(n)       (0x3U << ((n) * 4))
#define PERFCTR_CORE_FIXED_MASKn(n)              (0xfU << ((n) * 4))
#define PERFCTR_CORE_FIXED_SHIFTBYn(n)           ((n) * 4)
#define PERFCTR_CORE_FIXED_ANYTHREAD             0x00000444
// XXX serebrin/dhecht: 1-10-11: Make ANYTHREAD depend on number of fixed PMCs

/* Architectural event counters */
#define PERFCTR_CORE_UNHALTED_CORE_CYCLES        0x3c
#define PERFCTR_CORE_INST_RETIRED                0xc0
/* bus cycles */
#define PERFCTR_CORE_UNHALTED_REF_CYCLES         (0x3c | (0x01 << 8))

/*
 * See Tables 30-2, 30-4 of the
 * "Intel 64 and IA-32 Architecture's Software Developer Manual,
 * Volume 3B, System Programming Guide, Part 2
 */
#define PERFCTR_CORE_LLC_REF                     (0x2e | (0x4f << 8))
#define PERFCTR_CORE_LLC_MISSES                  (0x2e | (0x41 << 8))
#define PERFCTR_CORE_LLC_MISSES_PREFETCH         (0x2e | (0x71 << 8))
#define PERFCTR_CORE_LLC_MISSES_ALL              (0x2e | (0xc1 << 8))
#define PERFCTR_CORE_LLC_MISSES_ALL_PREFETCH     (0x2e | (0xf1 << 8))
#define PERFCTR_CORE_BRANCH_RETIRED              0xc4
#define PERFCTR_CORE_BRANCH_MISPRED_RETIRED      0xc5

/* Non-Architectural event counters in Intel Core and Core 2 */
#define PERFCTR_CORE_L2_LINES_IN                 0x24
#define PERFCTR_CORE_L2_M_LINES_IN               0x25
#define PERFCTR_CORE_L2_LINES_OUT                0x26
#define PERFCTR_CORE_L2_M_LINES_OUT              0x27
#define PERFCTR_CORE_DATA_MEM_REF                0x43
#define PERFCTR_CORE_DATA_MEM_CACHE_REF          0x44
#define PERFCTR_CORE_DCACHE_REPL                 0x45
#define PERFCTR_CORE_DCACHE_M_REPL               0x46
#define PERFCTR_CORE_DCACHE_M_EVICT              0x47
#define PERFCTR_CORE_DCACHE_PEND_MISS            0x48
#define PERFCTR_CORE_DTLB_MISS                   0x49
#define PERFCTR_CORE_BUS_TRANS                   0x70
#define PERFCTR_CORE_ICACHE_READS                0x80
#define PERFCTR_CORE_ICACHE_MISSES               0x81
#define PERFCTR_CORE_ITLB_MISSES                 0x85
#define PERFCTR_CORE_UOPS_RETIRED                0xC2
#define PERFCTR_CORE_RESOURCE_STALLS             0xDC
#define PERFCTR_NEHALEM_OFFCORE_RESP0_EVENT      (0xB7 | (0x01 << 8))
#define PERFCTR_NEHALEM_OFFCORE_RESP1_EVENT      (0xBB | (0x01 << 8))


/*
 * Nehalem off-core response events. See Section 30.6.1.2 of the
 * "Intel 64 and IA-32 Architecture's Software Developer Manual,
 * Volume 3B, System Programming Guide, Part 2.
 * One can specify
 * (request from the core, response from the uncore) pairs.
 */
#define PERFCTR_NEHALEM_OFFCORE_RESP0_ADDR             0x1A6
// requests
#define PERFCTR_NEHALEM_OFFCORE_RQST_DMND_DATA_RD       0x1
#define PERFCTR_NEHALEM_OFFCORE_RQST_DMND_RFO           0x2
#define PERFCTR_NEHALEM_OFFCORE_RQST_DMND_IFETCH        0x4
#define PERFCTR_NEHALEM_OFFCORE_RQST_WB                 0x8
#define PERFCTR_NEHALEM_OFFCORE_RQST_PF_DATA_RD         0x10
#define PERFCTR_NEHALEM_OFFCORE_RQST_PF_RFO             0x20
#define PERFCTR_NEHALEM_OFFCORE_RQST_PF_IFETCH          0x40
#define PERFCTR_NEHALEM_OFFCORE_RQST_OTHER              0x80

// responses
#define PERFCTR_NEHALEM_OFFCORE_RESP_UNCORE_HIT         0x100
#define PERFCTR_NEHALEM_OFFCORE_RESP_OTHER_CORE_HIT_SNP 0x200
#define PERFCTR_NEHALEM_OFFCORE_RESP_OTHER_CORE_HITM    0x400
#define PERFCTR_NEHALEM_OFFCORE_RESP_REMOTE_CACHE_FWD   0x1000
#define PERFCTR_NEHALEM_OFFCORE_RESP_REMOTE_DRAM        0x2000
#define PERFCTR_NEHALEM_OFFCORE_RESP_LOCAL_DRAM         0x4000
#define PERFCTR_NEHALEM_OFFCORE_RESP_NON_DRAM           0x8000

// Nehalem Uncore events.

/*
 * Uncore event MSRs. See table B-5 "MSRs in processors based on the
 * Intel micro-architecture (Nehalem), in "Intel 64 and IA-32 Architecture's
 * Software Developer Manual, Volume 3B, System Programming Guide, Part 2".
 * Though note that the addresses listed for PMCs and event select MSRs are
 * incorrect (swapped) in the manual.
 */
#define PERFCTR_NEHALEM_UNCORE_GLOBALCTRL_ADDR   0x391
#define PERFCTR_NEHALEM_UNCORE_PERFEVTSEL0_ADDR  0x3c0
#define PERFCTR_NEHALEM_UNCORE_PERFCTR0_ADDR     0x3b0

/*
 * The uncore event masks. See section 30.6.2 of the
 * "Intel 64 and IA-32 Architecture's Software Developer Manual,
 * Volume 3B, System Programming Guide, Part 2"
 */

// Enable the use of the programmable uncore counter "x".
#define PERFCTR_NEHALEM_UNCORE_ENABLE_CTR(x)     (1 << (x))
#define PERFCTR_NEHALEM_UNCORE_EDGE_DETECT       0x40000
#define PERFCTR_NEHALEM_UNCORE_ENABLE            0x400000
#define PERFCTR_NEHALEM_UNCORE_L3_LINES_IN       (0x0a | (0x0f << 8))


/*
 * ----------------------------------------------------------------------
 *
 * Pentium 4
 *
 * ----------------------------------------------------------------------
 */

/*
 * Pentium 4 Counter Configuration Control Register flags and fields
 */
#define PERFCTR_PENTIUM4_CCCR_ENABLE                   0x00001000
#define PERFCTR_PENTIUM4_CCCR_SET_ESCR(c, e)           ((c) |= (((e) & 0x7) << 13))
#define PERFCTR_PENTIUM4_CCCR_REQRSVD                  0x00030000 // always set these bits
#define PERFCTR_PENTIUM4_CCCR_COMPARE                  0x00040000
#define PERFCTR_PENTIUM4_CCCR_COMPLEMENT               0x00080000
#define PERFCTR_PENTIUM4_CCCR_SET_THRESHOLD(c, e)      ((c) |= (((e) & 0xf) << 20))
#define PERFCTR_PENTIUM4_CCCR_EDGE                     0x01000000
#define PERFCTR_PENTIUM4_CCCR_FORCE_OVF                0x02000000
#define PERFCTR_PENTIUM4_CCCR_OVF_PMI_T0               0x04000000
#define PERFCTR_PENTIUM4_CCCR_OVF_PMI_T1               0x08000000
#define PERFCTR_PENTIUM4_CCCR_CASCADE                  0x40000000
#define PERFCTR_PENTIUM4_CCCR_OVF                      0x80000000

#define PERFCTR_PENTIUM4_COUNTER_BASEADDR              0x00000300
#define PERFCTR_PENTIUM4_CCCR_BASE_ADDR                0x00000360

#define PERFCTR_PENTIUM4_CCCR_THRESHOLD(e)             ((e & 0xf) << 20)
#define PERFCTR_PENTIUM4_OPT_EDGE_DETECT (PERFCTR_PENTIUM4_CCCR_COMPARE | PERFCTR_PENTIUM4_CCCR_EDGE)

/*
 * Pentium 4 Event Selection Control Register flags
 */
#define PERFCTR_PENTIUM4_ESCR_USER_MODE_T0             0x00000004
#define PERFCTR_PENTIUM4_ESCR_KERNEL_MODE_T0           0x00000008
#define PERFCTR_PENTIUM4_ESCR_USER_MODE_T1             0x00000001
#define PERFCTR_PENTIUM4_ESCR_KERNEL_MODE_T1           0x00000002
#define PERFCTR_PENTIUM4_SHIFT_BY_UNITMASK(e)          (e << 9)
#define PENTIUM4_EVTSEL(e)                             (e << 25)

/*
 * Event definitions for Pentium4 Family Processors. 
 * There are many more of these possible.
 * See Appendix A.1 in Volume 3 of the IA32 manual.
 */

#define PERFCTR_PENTIUM4_EVT_INSTR_RETIRED              PENTIUM4_EVTSEL(0x02)
#define PERFCTR_PENTIUM4_EVT_INSTR_COMPLETED            PENTIUM4_EVTSEL(0x07)
#define PERFCTR_PENTIUM4_EVT_BSQ_CACHE_REFERENCE        PENTIUM4_EVTSEL(0x0c)
#define PERFCTR_PENTIUM4_EVT_ITLB_REFERENCE             PENTIUM4_EVTSEL(0x18)
#define PERFCTR_PENTIUM4_EVT_PAGE_WALK_TYPE             PENTIUM4_EVTSEL(0x01)   
#define PERFCTR_PENTIUM4_EVT_BPU_FETCH_REQUEST          PENTIUM4_EVTSEL(0x03)
#define PERFCTR_PENTIUM4_EVT_BRANCH_RETIRED             PENTIUM4_EVTSEL(0x06)  
#define PERFCTR_PENTIUM4_EVT_MISPRED_BRANCH_RETIRED     PENTIUM4_EVTSEL(0x03)
#define PERFCTR_PENTIUM4_EVT_MACHINE_CLEAR              PENTIUM4_EVTSEL(0x02)
#define PERFCTR_PENTIUM4_EVT_MEMORY_CANCEL              PENTIUM4_EVTSEL(0x02) 
#define PERFCTR_PENTIUM4_GLOBAL_POWER_EVENTS            PENTIUM4_EVTSEL(0x13)

/*
 * Pentium4 doesn't have an event for clock cycles, but Intel
 * outlines a method in the IA32 manual, vol 3, sec 14.9.9,
 * for measuring cycles which involves this event strategy.
 */

#define PERFCTR_PENTIUM4_EVT_CLK_CYCLES \
   PENTIUM4_EVTSEL(0x2) | \
   PERFCTR_PENTIUM4_SHIFT_BY_UNITMASK (0x01 | 0x02 | 0x04 | 0x08)


/*
 * PerfCtr_Counter --
 *      Describes a single hardware performance counter
 *
 *      cccrAddr/Val and escrAddr/Val are Pentium-4 specific names, and can
 *      cause confusion for K8/P6 (PR 105843).  Maybe we should use a union.
 *      
 *      On Pentium 4:
 *      index:    Which perf ctr, from 0 to 17.  RDPMC argument.
 *      addr:     MSR # of raw 40 bit perf ctr reg (0x300 + index).
 *      cccrAddr: MSR # of control cfg reg         (0x360 + index).
 *      cccrVal:  Value placed in MSR cccrAddr.  Partially specifies what's
 *                being measured, with help from an escr register.  The escr
 *                MSR number is a function of cccrAddr and the escr specifier
 *                bits of cccrVal.
 *      escrAddr: There are 40+ of these MSRs, skip-numbered 0x3a0 to 0x3f0.
 *                Contains event select/mask, OS/USR flags, but not bits like
 *                enable or overflow, which are in the cccr.
 *      escrVal:  Value placed in MSR #escrAddr.
 *      
 *      On AMD K8 and GH:
 *      index:        Which perf ctr, 0 to 3.  RDPMC argument
 *      addr:         MSR of raw perf ctr              (0xc0010004 + index).
 *      escrAddr:     MSR # of the Perf Event Selector (0xc0010000 + index).
 *      escrVal:      Value placed in PerfEvtSel MSR; what to measure.
 *      cccrAddr/Val: we set these to 0.
 *
 *      On AMD BD:
 *      index:        Which perf ctr, 0 to 5.  RDPMC argument
 *      addr:         MSR of raw perf ctr              (0xc0010201 + 2 * index).
 *                                  aliased PMCs 0 - 3 (0xc0010004 + index).
 *      escrAddr:     MSR # of the Perf Event Selector (0xc0010200 + 2 * index).
 *                                  aliased PMCs 0 - 3 (0xc0010000 + index).
 *      escrVal:      Value placed in PerfEvtSel MSR; what to measure.
 *      cccrAddr/Val: we set these to 0.
 *
 *      On Intel P6 family:
 *      index:        Which perf ctr, 0 or 1.  RDPMC argument.
 *      addr:         MSR of raw perf counter      (0x000000c1 + index).
 *      escrAddr:     MSR # of Perf Event Selector (0x00000186 + index).
 *      escrVal:      Value placed in PerfEvtSel MSR; what to measure.
 *                    Note: 'enable' bit only defined for MSR 0x186 and it
 *                    applies to both perf ctrs, so either both or neither perf
 *                    counter is active at a time.  So it's a good idea to
 *                    sanely define the "other, don't care" MSR, to avoid
 *                    possible undesired overflow intrs (PerfEventSel val 0
 *                    may be OK since OS/USR bits both 0 means no increment).
 *      cccrAddr/Val: If desired, these can hold the MSR addr/val of the other,
 *                    don't-care, perf ctr, so it can be sanely programmed in
 *                    the presence of undesired 'enable' bit behavior (see
 *                    above).  But vmkernel currently sets these to 0, like K8.
 * 
 *      No field should be greater than 32-bit as it is shared with monitor.
 *
 *      On Intel Core architecture:
 *      <to be documented>
 */
typedef struct {
   uint64 escrVal;
   uint32 index;
   uint32 addr;
   uint32 escrAddr;
   uint32 cccrAddr;
   uint32 cccrVal;
   uint32 _pad[1];
} PerfCtr_Counter;

// contains config for a perfctr event
typedef struct PerfCtr_Config {
   PerfCtr_Counter counters[2]; // for hypertwins
   uint32  resetHi;       
   uint32  periodMean;     

   /*
    * Random number (whose absolute value is capped at
    * periodJitterMask) is used to randomize sampling interval.
    */
   uint32  periodJitterMask;
   uint32  seed;    // seed is used to compute next random number        
   uint32  config;
   uint32  unitMask;
   uint64  eventSelReg;
   uint32  eventSel;
   char    eventName[PERF_EVENT_NAME_LEN];
   Bool    valid;
   
} PerfCtr_Config;

/*
 * Trimmed version of PerfCtr_Config, suitable for sharing
 * between monitor and vmkernel.
 */
typedef struct PerfCtr_ConfigBasic {
   PerfCtr_Counter counters[2]; // for hypertwins
   uint32  resetHi;
   uint32  periodMean;
   uint32  periodJitterMask;
   uint32 _pad1;
} PerfCtr_ConfigBasic;

/*
 * Program/reprogram event reg(s) associated w/perfctrs & start or stop perfctrs
 */
static INLINE void
PerfCtrWriteEvtSel(PerfCtr_Counter *ctr,  // IN: counter to write
                   uint32 escrVal,        // IN: event register value
                   uint32 controlVal)     // IN: control reg value to write (P4)
{
   __SET_MSR(ctr->escrAddr, escrVal);
   if (ctr->cccrAddr != 0) {
      WRMSR(ctr->cccrAddr, controlVal, 0);
   }
}


/*
 * Set/reset performance counters to engender desired period before overflow
 */
static INLINE void
PerfCtrWriteCounter(PerfCtr_Counter *ctr,   // IN: counter to write
                    uint32 valueLo,        // IN: low 32 bits of value to write
                    uint32 valueHi)        // IN: high 32 bits of value to write
{
   WRMSR(ctr->addr, valueLo, valueHi);
}


/* Reads the current value of the counter and determines if it overflowed, 
 * by checking if bit 39 is 0. The method below is fine for any
 * latency (between counter overflow and interrupt) up to ~25 seconds 
 * on a 20GHz machine, which should keep us happy for a while. 
 * For Athlon, we could check upto bit 47 but in practice, the counter 
 * value would never reach there before an interrupt is triggered.
 */
static INLINE uint32
PERFCTR_CHECK_OVERFLOW(int ctr) 
{
   uint64 val;

   val = RDPMC(ctr);
   return (!((uint32)(val >> 39) & 1));
}

static INLINE uint64
PerfCtr_SelValidBits(Bool amd)
{
   /*
    * Intel enforces PIN_CONTROL as MBZ; AMD does not.  Always mask on AMD
    * to avoid toggling the physical pin.
    */
   uint64 bits = PERFCTR_CPU_EVENT_MASK  | PERFCTR_CPU_UNIT_MASK |
                 PERFCTR_CPU_USER_MODE   | PERFCTR_CPU_KERNEL_MODE |
                 PERFCTR_CPU_EDGE_DETECT | PERFCTR_CPU_APIC_INTR |
                 PERFCTR_CPU_ENABLE      | PERFCTR_CPU_INVERT_COUNTER_MASK |
                 PERFCTR_CPU_COUNTER_MASK;
   if (amd) {
      bits |= PERFCTR_AMD_EXT_EVENT_MASK | PERFCTR_AMD_EVTSEL_HOST |
              PERFCTR_AMD_EVTSEL_GUEST;
   } else {
      bits |= PERFCTR_CORE_ANYTHREAD;
   }
   return bits;
}

static INLINE uint64
PerfCtr_PgcValidBits(unsigned numGenCtrs, unsigned numFixCtrs)
{
   return MASK64(numGenCtrs) | (MASK64(numFixCtrs) << 32);
}

static INLINE uint64
PerfCtr_FccValidBits(unsigned numFixCtrs)
{
   return MASK64(numFixCtrs * 4);
}

static INLINE uint64
PerfCtr_PgcToOvfValidBits(uint64 pgcValBits)
{
   return pgcValBits | MASKRANGE64(63, 61);
}

/* The following are taken from the Intel Architecture Manual,
 * Book 3, Table 14-4. 
 */ 

/* --------- BEGIN INTEL DEFINES ------------------------ */

#define PENTIUM4_MSR_BPU_COUNTER0_IDX		0
#define PENTIUM4_MSR_BPU_COUNTER0_ADDR		0x300
#define PENTIUM4_MSR_BPU_CCCR0	       	        0x360
#define PENTIUM4_MSR_BPU_COUNTER1_IDX		1
#define PENTIUM4_MSR_BPU_COUNTER1_ADDR		0x301
#define PENTIUM4_MSR_BPU_CCCR1		        0x361
#define PENTIUM4_MSR_BPU_COUNTER2_IDX		2
#define PENTIUM4_MSR_BPU_COUNTER2_ADDR		0x302
#define PENTIUM4_MSR_BPU_CCCR2		        0x362
#define PENTIUM4_MSR_BPU_COUNTER3_IDX		3
#define PENTIUM4_MSR_BPU_COUNTER3_ADDR		0x303
#define PENTIUM4_MSR_BPU_CCCR3		        0x363
#define PENTIUM4_MSR_MS_COUNTER0_IDX		4
#define PENTIUM4_MSR_MS_COUNTER0_ADDR		0x304
#define PENTIUM4_MSR_MS_CCCR0		        0x364
#define PENTIUM4_MSR_MS_COUNTER1_IDX		5
#define PENTIUM4_MSR_MS_COUNTER1_ADDR		0x305
#define PENTIUM4_MSR_MS_CCCR1		        0x365
#define PENTIUM4_MSR_MS_COUNTER2_IDX		6
#define PENTIUM4_MSR_MS_COUNTER2_ADDR		0x306
#define PENTIUM4_MSR_MS_CCCR2		        0x366
#define PENTIUM4_MSR_MS_COUNTER3_IDX		7
#define PENTIUM4_MSR_MS_COUNTER3_ADDR		0x307
#define PENTIUM4_MSR_MS_CCCR3		        0x367
#define PENTIUM4_MSR_FLAME_COUNTER0_IDX         8
#define PENTIUM4_MSR_FLAME_COUNTER0_ADDR        0x308
#define PENTIUM4_MSR_FLAME_CCCR0		0x368
#define PENTIUM4_MSR_FLAME_COUNTER1_IDX         9
#define PENTIUM4_MSR_FLAME_COUNTER1_ADDR	0x309
#define PENTIUM4_MSR_FLAME_CCCR1		0x369
#define PENTIUM4_MSR_FLAME_COUNTER2_IDX		10
#define PENTIUM4_MSR_FLAME_COUNTER2_ADDR	0x30A
#define PENTIUM4_MSR_FLAME_CCCR2		0x36A
#define PENTIUM4_MSR_FLAME_COUNTER3_IDX		11
#define PENTIUM4_MSR_FLAME_COUNTER3_ADDR	0x30B
#define PENTIUM4_MSR_FLAME_CCCR3		0x36B
#define PENTIUM4_MSR_IQ_COUNTER0_IDX		12
#define PENTIUM4_MSR_IQ_COUNTER0_ADDR		0x30C
#define PENTIUM4_MSR_IQ_CCCR0		        0x36C
#define PENTIUM4_MSR_IQ_COUNTER1_IDX		13
#define PENTIUM4_MSR_IQ_COUNTER1_ADDR		0x30D
#define PENTIUM4_MSR_IQ_CCCR1		        0x36D
#define PENTIUM4_MSR_IQ_COUNTER2_IDX		14
#define PENTIUM4_MSR_IQ_COUNTER2_ADDR		0x30E
#define PENTIUM4_MSR_IQ_CCCR2		        0x36E
#define PENTIUM4_MSR_IQ_COUNTER3_IDX		15
#define PENTIUM4_MSR_IQ_COUNTER3_ADDR		0x30F
#define PENTIUM4_MSR_IQ_CCCR3		        0x36F
#define PENTIUM4_MSR_IQ_COUNTER4_IDX		16
#define PENTIUM4_MSR_IQ_COUNTER4_ADDR		0x310
#define PENTIUM4_MSR_IQ_CCCR4		        0x370
#define PENTIUM4_MSR_IQ_COUNTER5_IDX		17
#define PENTIUM4_MSR_IQ_COUNTER5_ADDR		0x311
#define PENTIUM4_MSR_IQ_CCCR5		        0x371

#define PENTIUM4_MSR_ALF_ESCR0_IDX		1
#define PENTIUM4_MSR_ALF_ESCR0_ADDR		0x3CA
#define PENTIUM4_MSR_ALF_ESCR1_IDX		1
#define PENTIUM4_MSR_ALF_ESCR1_ADDR		0x3CB
#define PENTIUM4_MSR_BPU_ESCR0_IDX		0
#define PENTIUM4_MSR_BPU_ESCR0_ADDR		0x3B2
#define PENTIUM4_MSR_BPU_ESCR1_IDX		0
#define PENTIUM4_MSR_BPU_ESCR1_ADDR		0x3B3
#define PENTIUM4_MSR_BSU_ESCR0_IDX		7
#define PENTIUM4_MSR_BSU_ESCR0_ADDR		0x3A0
#define PENTIUM4_MSR_BSU_ESCR1_IDX		7
#define PENTIUM4_MSR_BSU_ESCR1_ADDR		0x3A1
#define PENTIUM4_MSR_CRU_ESCR0_IDX		4
#define PENTIUM4_MSR_CRU_ESCR0_ADDR		0x3B8
#define PENTIUM4_MSR_CRU_ESCR1_IDX		4
#define PENTIUM4_MSR_CRU_ESCR1_ADDR		0x3B9
#define PENTIUM4_MSR_CRU_ESCR2_IDX		5
#define PENTIUM4_MSR_CRU_ESCR2_ADDR		0x3CC
#define PENTIUM4_MSR_CRU_ESCR3_IDX		5
#define PENTIUM4_MSR_CRU_ESCR3_ADDR		0x3CD
#define PENTIUM4_MSR_CRU_ESCR4_IDX		6
#define PENTIUM4_MSR_CRU_ESCR4_ADDR		0x3E0
#define PENTIUM4_MSR_CRU_ESCR5_IDX		6
#define PENTIUM4_MSR_CRU_ESCR5_ADDR		0x3E1
#define PENTIUM4_MSR_DAC_ESCR0_IDX		5
#define PENTIUM4_MSR_DAC_ESCR0_ADDR		0x3A8
#define PENTIUM4_MSR_DAC_ESCR1_IDX		5
#define PENTIUM4_MSR_DAC_ESCR1_ADDR		0x3A9
#define PENTIUM4_MSR_FIRM_ESCR0_IDX		1
#define PENTIUM4_MSR_FIRM_ESCR0_ADDR		0x3A4
#define PENTIUM4_MSR_FIRM_ESCR1_IDX		1
#define PENTIUM4_MSR_FIRM_ESCR1_ADDR		0x3A5
#define PENTIUM4_MSR_FLAME_ESCR0_IDX		0
#define PENTIUM4_MSR_FLAME_ESCR0_ADDR		0x3A6
#define PENTIUM4_MSR_FLAME_ESCR1_IDX		0
#define PENTIUM4_MSR_FLAME_ESCR1_ADDR		0x3A7
#define PENTIUM4_MSR_FSB_ESCR0_IDX		6
#define PENTIUM4_MSR_FSB_ESCR0_ADDR		0x3A2
#define PENTIUM4_MSR_FSB_ESCR1_IDX		6
#define PENTIUM4_MSR_FSB_ESCR1_ADDR		0x3A3
#define PENTIUM4_MSR_IQ_ESCR0_IDX		0
#define PENTIUM4_MSR_IQ_ESCR0_ADDR		0x3BA
#define PENTIUM4_MSR_IQ_ESCR1_IDX		0
#define PENTIUM4_MSR_IQ_ESCR1_ADDR		0x3BB
#define PENTIUM4_MSR_IS_ESCR0_IDX		1
#define PENTIUM4_MSR_IS_ESCR0_ADDR		0x3B4
#define PENTIUM4_MSR_IS_ESCR1_IDX		1
#define PENTIUM4_MSR_IS_ESCR1_ADDR		0x3B5
#define PENTIUM4_MSR_ITLB_ESCR0_IDX		3
#define PENTIUM4_MSR_ITLB_ESCR0_ADDR		0x3B6
#define PENTIUM4_MSR_ITLB_ESCR1_IDX		3
#define PENTIUM4_MSR_ITLB_ESCR1_ADDR		0x3B7
#define PENTIUM4_MSR_IX_ESCR0_IDX		5
#define PENTIUM4_MSR_IX_ESCR0_ADDR		0x3C8
#define PENTIUM4_MSR_IX_ESCR1_IDX		5
#define PENTIUM4_MSR_IX_ESCR1_ADDR		0x3C9
#define PENTIUM4_MSR_MOB_ESCR0_IDX		2
#define PENTIUM4_MSR_MOB_ESCR0_ADDR		0x3AA
#define PENTIUM4_MSR_MOB_ESCR1_IDX		2
#define PENTIUM4_MSR_MOB_ESCR1_ADDR		0x3AB
#define PENTIUM4_MSR_MS_ESCR0_IDX		0
#define PENTIUM4_MSR_MS_ESCR0_ADDR		0x3C0
#define PENTIUM4_MSR_MS_ESCR1_IDX		0
#define PENTIUM4_MSR_MS_ESCR1_ADDR		0x3C1
#define PENTIUM4_MSR_PMH_ESCR0_IDX		4
#define PENTIUM4_MSR_PMH_ESCR0_ADDR		0x3AC
#define PENTIUM4_MSR_PMH_ESCR1_IDX		4
#define PENTIUM4_MSR_PMH_ESCR1_ADDR		0x3AD
#define PENTIUM4_MSR_RAT_ESCR0_IDX		2
#define PENTIUM4_MSR_RAT_ESCR0_ADDR		0x3BC
#define PENTIUM4_MSR_RAT_ESCR1_IDX		2
#define PENTIUM4_MSR_RAT_ESCR1_ADDR		0x3BD
#define PENTIUM4_MSR_SAAT_ESCR0_IDX		2
#define PENTIUM4_MSR_SAAT_ESCR0_ADDR		0x3AE
#define PENTIUM4_MSR_SAAT_ESCR1_IDX		2
#define PENTIUM4_MSR_SAAT_ESCR1_ADDR		0x3AF
#define PENTIUM4_MSR_SSU_ESCR0_IDX		3
#define PENTIUM4_MSR_SSU_ESCR0_ADDR		0x3BE
#define PENTIUM4_MSR_TBPU_ESCR0_IDX		2
#define PENTIUM4_MSR_TBPU_ESCR0_ADDR		0x3C2
#define PENTIUM4_MSR_TBPU_ESCR1_IDX		2
#define PENTIUM4_MSR_TBPU_ESCR1_ADDR		0x3C3
#define PENTIUM4_MSR_TC_ESCR0_IDX		1
#define PENTIUM4_MSR_TC_ESCR0_ADDR		0x3C4
#define PENTIUM4_MSR_TC_ESCR1_IDX		1
#define PENTIUM4_MSR_TC_ESCR1_ADDR		0x3C5
#define PENTIUM4_MSR_U2L_ESCR0_IDX		3
#define PENTIUM4_MSR_U2L_ESCR0_ADDR		0x3B0
#define PENTIUM4_MSR_U2L_ESCR1_IDX		3
#define PENTIUM4_MSR_U2L_ESCR1_ADDR		0x3B1

#define PENTIUM4_MIN_ESCR_ADDR                  0x3A0
#define PENTIUM4_MAX_ESCR_ADDR                  0x3E1
#define PENTIUM4_NUM_ESCR_ADDRS                 (PENTIUM4_MAX_ESCR_ADDR - PENTIUM4_MIN_ESCR_ADDR + 1)

/* -------- END INTEL DEFINES ------------------------- */

/*
 * nmiNo      -- vmm peer is not attempting to do nmi profiling this run
 * nmiYes     -- vmm peer is doing nmi profiling and nmis are currently enabled
 * nmiStopped -- vmm peer is doing nmi profiling, but nmis are temporarily
 *               disabled for safety reasons.
 */
typedef enum {nmiNo = 0, nmiYes, nmiStopped} NMIStatus;
typedef struct NMIShared { /* shared with vmx and vmkernel */
   NMIStatus vmmStatus;
   int32     nmiErrorCode;
   int32     nmiErrorData;
   int32     nmiErrorData2;
} NMIShared;

/*
 * CrossProf: structures for unified profiling of vmm, vmx, and vmkernel.  Per-vcpu.
 */

#define CALLSTACK_CROSSPROF_PAGES 1

typedef struct {
   /*
    * This structure is per-vcpu.  The raw data is a packed vector
    * of MonitorCallStackSample, a variable-length structure.
    */

   /* raw data - packed vec of MonitorCallStackSample, variable length */
   uint8  crossProfSampleBuffer[PAGES_2_BYTES(CALLSTACK_CROSSPROF_PAGES)];

   uint32 crossProfSampleBytes;
   uint32 crossProfNumDroppedSamples; /* For when buffer fills up */
   Bool   enabled; /* Can be false in stats build if monitor.callstack=FALSE */
   uint8  _pad[3];
} CrossProfShared;

#endif // ifndef _X86PERFCTR_H_ 