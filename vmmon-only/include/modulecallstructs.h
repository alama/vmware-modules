/*********************************************************
 * Copyright (C) 2006-2011 VMware, Inc. All rights reserved.
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
 * modulecallstructs.h --
 *
 *
 *      Data structures that need to be included in modulecall.h
 *      as well as the vmkernel.
 *
 */

#ifndef _MODULECALLSTRUCTS_H_
#define _MODULECALLSTRUCTS_H_

#define INCLUDE_ALLOW_VMMON
#define INCLUDE_ALLOW_VMKERNEL
#define INCLUDE_ALLOW_VMCORE

#include "includeCheck.h"

#include "vm_basic_types.h"
#include "x86types.h"

/*
 *      Flags indicating system call MSR status.
 *
 * PRESERVED - Monitor or user world value is preserved on world switch.
 * SHADOWED  - Live monitor value is always shadowed in the scs structure.
 * CLOBBERED - Host value is clobbered by monitor (hosted only).
 *
 */

#define SCMSR_FLAG_PRESERVED     1
#define SCMSR_FLAG_SHADOWED      2
#define SCMSR_FLAG_CLOBBERED     4

/*
 *      Data structures for dealing with the system call MSRs that need
 *      to be specially handled.  While the MSR definitions themselves
 *      are part of the x86 architecture, our handling of them (and hence
 *      these data structures) are an implementation detail.
 */

typedef enum SystemCallMSR {
   SCMSR_SYSENTERCS,
   SCMSR_SYSENTERRIP,
   SCMSR_SYSENTERRSP,
   SCMSR_STAR,
   SCMSR_LSTAR,
   SCMSR_CSTAR,
   SCMSR_SFMASK,
   NUM_SCMSR_REGS
} SystemCallMSR;

typedef struct SystemCallRegistersStruct {
   /* The order here must match up with the enum above. */
   Selector     sysenterCS;
   uint16       _pad[3];
   uint64       sysenterRIP;
   uint64       sysenterRSP;
   uint64       star;
   uint64       lstar;
   uint64       cstar;
   uint64       sfmask;
} SystemCallRegistersStruct;

/*
 * System Call Information for each VCPU.
 */
typedef union SystemCallRegisters {
   SystemCallRegistersStruct s;
   uint64 a[NUM_SCMSR_REGS];
} SystemCallRegisters;

typedef struct SystemCallState {
   SystemCallRegisters scr;
   uint8               msrFlags[NUM_SCMSR_REGS];
} SystemCallState;

#endif
