/*********************************************************
 * Copyright (C) 2003-2014 VMware, Inc. All rights reserved.
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
 * x86types.h --
 *
 *      Type definitions for the x86 architecture.
 */

#ifndef _X86TYPES_H_
#define _X86TYPES_H_

#define INCLUDE_ALLOW_USERLEVEL

#define INCLUDE_ALLOW_MODULE
#define INCLUDE_ALLOW_VMK_MODULE
#define INCLUDE_ALLOW_VMKERNEL
#define INCLUDE_ALLOW_DISTRIBUTE
#define INCLUDE_ALLOW_VMCORE
#define INCLUDE_ALLOW_VMMON
#include "includeCheck.h"

#include "vm_basic_types.h"
#include "vm_basic_defs.h"
#include "x86segdescrs.h"

/*
 * Virtual, physical, machine address and page conversion macros
 *
 * XXX These should either go in vm_basic_types.h, where the types
 * are defined, or those definitions should be moved here.
 */

#define VA_2_VPN(_va)  ((_va) >> PAGE_SHIFT)
#define PTR_2_VPN(_ptr) VA_2_VPN((VA)(_ptr))
#define VPN_2_VA(_vpn) ((_vpn) << PAGE_SHIFT)
#define VPN_2_PTR(_vpn) ((void *)VPN_2_VA(_vpn))

/*
 * Notice that we don't cast PA_2_PPN's argument to an unsigned type, because
 * we would lose compile-time checks for pointer operands and byte-sized
 * operands. If you use a signed quantity for _pa, ones may be shifted into the
 * high bits of your ppn.
 */

#define PA_2_PPN(_pa)     ((_pa) >> PAGE_SHIFT)
#define PPN_2_PA(_ppn)    ((PA)(_ppn) << PAGE_SHIFT)

static INLINE MA    MPN_2_MA(MPN64 mpn)   { return    (MA)mpn << PAGE_SHIFT;  }
static INLINE MPN64 MA_2_MPN(MA ma)       { return (MPN64)(ma >> PAGE_SHIFT); }

static INLINE IOA   IOPN_2_IOA(IOPN iopn) { return (IOA)(iopn << PAGE_SHIFT); }
static INLINE IOPN  IOA_2_IOPN(IOA ioa)   { return (IOPN)(ioa >> PAGE_SHIFT); }

/*
 * Types used for PL4 page table in x86_64
 */

typedef uint64 VM_L4E;
typedef uint64 VM_L3E;
typedef uint64 VM_L2E;
typedef uint64 VM_L1E;


/*
 * 4 Mb pages
 */

#define VM_LARGE_PAGE_SHIFT  22
#define VM_LARGE_PAGE_SIZE   (1 << VM_LARGE_PAGE_SHIFT)
#define VM_LARGE_PAGE_MASK   (VM_LARGE_PAGE_SIZE - 1)


/*
 * Page table
 */

typedef uint32 VM_PDE;
typedef uint32 VM_PTE;
typedef uint64 VM_PAE_PDE;
typedef uint64 VM_PAE_PTE;
typedef uint64 VM_PDPTE;


/*
 * Extended page table
 */

typedef uint64 VM_EPTE;

/*
 * Registers
 */

typedef  int8    Reg8;
typedef  int16   Reg16;
typedef  int32   Reg32;
typedef  int64   Reg64;

typedef uint8   UReg8;
typedef uint16  UReg16;
typedef uint32  UReg32;
typedef uint64  UReg64;

#if defined(VMM) || defined(COREQUERY) || defined (VMKERNEL) \
    || defined (VMKBOOT)
typedef  Reg64  Reg;
typedef UReg64 UReg;
#endif

typedef union SharedReg64 {
   Reg8  reg8[2];
   Reg16 reg16;
   Reg32 reg32;
   Reg64 reg64;
} SharedReg64;

typedef union SharedUReg64 {
   UReg8  ureg8[2];
   UReg16 ureg16;
   UReg32 ureg32;
   UReg32 ureg32Pair[2];
   UReg64 ureg64;
} SharedUReg64;

typedef uint8 Instruction;

typedef uint16 Selector;

/*
 *   tasks
 */


#define IST_NUM_ENTRIES 8
#pragma pack(push, 1)
typedef struct Task64 {
   uint32     reserved0;
   uint64     rsp[3];   // Stacks for CPL 0-2.
   uint64     ist[IST_NUM_ENTRIES];   // ist[0] is reserved.
   uint64     reserved1;
   uint16     reserved2;
   uint16     IOMapBase;
} Task64;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct Task32 {
   uint32     prevTask;
   uint32     esp0;
   uint32     ss0;
   uint32     esp1;
   uint32     ss1;
   uint32     esp2;
   uint32     ss2;
   uint32     cr3;
   uint32     eip;
   uint32     eflags;
   uint32     eax;
   uint32     ecx;
   uint32     edx;
   uint32     ebx;
   uint32     esp;
   uint32     ebp;
   uint32     esi;
   uint32     edi;
   uint32     es;
   uint32     cs;
   uint32     ss;
   uint32     ds;
   uint32     fs;
   uint32     gs;
   uint32     ldt;
   uint16     trap;
   uint16     IOMapBase;
} Task32;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
   uint16     prevTask;
   uint16     sp0;  // static.  Unmarked fields are dynamic
   uint16     ss0;  // static
   uint16     sp1;  // static
   uint16     ss1;  // static
   uint16     sp2;  // static
   uint16     ss2;  // static 
   uint16     ip;
   uint16     flags;
   uint16     ax;
   uint16     cx;
   uint16     dx;
   uint16     bx;
   uint16     sp;
   uint16     bp;
   uint16     si;
   uint16     di;
   uint16     es;
   uint16     cs;
   uint16     ss;
   uint16     ds;
   uint16     ldt;  // static
} Task16;
#pragma pack(pop)

// Task defaults to Task32 for everyone except vmkernel. Task64 is used where
// needed by these products.
#if defined VMX86_SERVER && defined VMKERNEL
#ifdef VM_X86_64
typedef Task64 Task;
#else
typedef Task32 Task;
#endif
#else /* VMX86_SERVER && defined VMKERNEL */
typedef Task32 Task;
#endif


/*
 *   far pointers
 */

#pragma pack(push, 1)
typedef struct {
#if defined(VMM) || defined(COREQUERY)
   uint64 va;
#else
   uint32 va;
#endif
   Selector seg;
} FarPtr;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FarPtr16 {
   uint16   offset;
   uint16   selector;
} FarPtr16;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FarPtr32 {
   uint32   offset;
   uint16   selector;
} FarPtr32;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct FarPtr64 {
   uint64   offset;
   uint16   selector;
} FarPtr64;
#pragma pack(pop)

/*
 * X86-defined stack layouts for interrupts, exceptions, irets, calls, etc.
 */

/*
 * Layout of the 64-bit stack frame on exception.
 */
#pragma pack(push, 1)
typedef struct x86ExcFrame64 {
   uint64       rip;
   uint16       cs, __sel[3];
   uint64       rflags;
   uint64       rsp;
   uint16       ss, __ssel[3];
} x86ExcFrame64;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct x86ExcFrame64WithErrorCode {
   uint32       errorCode, __errorCode;
   uint64       rip;
   uint16       cs, __sel[3];
   uint64       rflags;
   uint64       rsp;
   uint16       ss, __ssel[3];
} x86ExcFrame64WithErrorCode;
#pragma pack(pop)

/*
 * Layout of the 32-bit stack frame on exception.
 */
#pragma pack(push, 1)
typedef struct x86ExcFrame32 {
   uint32         eip;
   union {
      struct {
         uint16   sel, __sel;
      }           cs16;
      uint32      cs32;
   } u;
   uint32         eflags;
} x86ExcFrame32;
#pragma pack(pop)

/*
 * Layout of the 32-bit stack frame with ss:esp and no error code.
 */
#pragma pack(push, 1)
typedef struct x86ExcFrame32WithStack {
   uint32      eip;
   uint16      cs, __csu;
   uint32      eflags;
   uint32      esp;
   uint16      ss, __ssu;
} x86ExcFrame32WithStack;
#pragma pack(pop)

/*
 * Layout of the 32-bit stack frame on inter-level transfer.
 */
#pragma pack(push, 1)
typedef struct x86ExcFrame32IL {
   uint32      errorCode;
   uint32      eip;
   uint16      cs, __csu;
   uint32      eflags;
   uint32      esp;
   uint16      ss, __ssu;
} x86ExcFrame32IL;
#pragma pack(pop)


/*
 * Layout of the 16-bit stack frame on exception.
 */
#pragma pack(push, 1)
typedef struct x86ExcFrame16 {
   uint16   eip;
   uint16   cs;
   uint16   eflags;
} x86ExcFrame16;
#pragma pack(pop)

/*
 * Layout of the 16-bit stack frame which incudes ss:sp.
 */
#pragma pack(push, 1)
typedef struct x86ExcFrame16WithStack {
   uint16   ip;
   uint16   cs;
   uint16   flags;
   uint16   sp;
   uint16   ss;
} x86ExcFrame16WithStack;
#pragma pack(pop)

/*
 * Layout of the 32-bit stack frame on exception
 * from V8086 mode. It is also a superset
 * of inter-level exception stack frame, which
 * in turn is superset of intra-level exception
 * stack frame.
 */
#pragma pack(push, 1)
typedef struct x86ExcFrameV8086 {
   uint32         eip;
   union {
      struct {
         uint16   sel, __sel;
      }           cs16;
      uint32      cs32;
   } u;
   uint32         eflags;
   uint32         esp;
   uint16         ss, __ss;
   uint16         es, __es;
   uint16         ds, __ds;
   uint16         fs, __fs;
   uint16         gs, __gs;
} x86ExcFrameV8086;
#pragma pack(pop)

/*
 * Layout of the 32-bit stack frame on exception
 * from V8086 mode with errorCode. It is
 * superset of SegmentExcFrameV8086.
 */
#pragma pack(push, 1)
typedef struct x86ExcFrameV8086WithErrorCode {
   uint32         errorCode;
   uint32         eip;
   union {
      struct {
         uint16   sel, __sel;
      }           cs16;
      uint32      cs32;
   } u;
   uint32         eflags;
   uint32         esp;
   uint16         ss, __ss;
   uint16         es, __es;
   uint16         ds, __ds;
   uint16         fs, __fs;
   uint16         gs, __gs;
} x86ExcFrameV8086WithErrorCode;
#pragma pack(pop)

/*
 * Layout of the stack on a 32 bit far call.
 */
#pragma pack(push, 1)
typedef struct x86CallStack32 {
   uint32   eip;
   uint16   cs, __cs;
} x86CallStack32;
#pragma pack(pop)

/*
 * Layout of the stack on a 16 bit far call.
 */
#pragma pack(push, 1)
typedef struct x86CallStack16 {
   uint16   ip;
   uint16   cs;
} x86CallStack16;
#pragma pack(pop)

/*
 * Layout of the stack on a 32 bit far call.
 */
#pragma pack(push, 1)
typedef struct x86CallGateStack32 {
   uint32   eip;
   uint16   cs, __cs;
   uint32   esp;
   uint16   ss, __ss;
} x86CallGateStack32;
#pragma pack(pop)

/*
 * Layout of the stack on a 16 bit far call.
 */
#pragma pack(push, 1)
typedef struct x86CallGateStack16 {
   uint16   ip;
   uint16   cs;
   uint16   sp;
   uint16   ss;
} x86CallGateStack16;
#pragma pack(pop)

typedef struct DebugControlRegister {

   int l0:1;
   int g0:1;
   int l1:1;
   int g1:1;
   int l2:1;
   int g2:1;
   int l3:1;
   int g3:1;
   
   int le:1;
   int ge:1;
   int oo1:3;
   
   int gd:1;
   int oo:2;
   
   int rw0:2;
   int len0:2;
   int rw1:2;
   int len1:2;
   int rw2:2;
   int len2:2;
   int rw3:2;
   int len3:2;
   
} DebugControlRegister;

#endif // ifndef _X86TYPES_H_
