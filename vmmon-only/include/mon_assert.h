/*********************************************************
 * Copyright (C) 2007-2013 VMware, Inc. All rights reserved.
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

#ifndef _MON_ASSERT_H_
#define _MON_ASSERT_H_

#define INCLUDE_ALLOW_USERLEVEL

#define INCLUDE_ALLOW_VMMON
#define INCLUDE_ALLOW_VMCORE
#define INCLUDE_ALLOW_VMIROM
#include "includeCheck.h"

#include "vm_assert.h"

/*
 * Monitor source location
 *
 * The monitor encodes source locations -- file name & line number --
 * in just 32 bits; the process is arcane enough that it deserves a
 * little discussion.
 *
 *   o The ASSERT family of macros are expanded in the monitor to take
 *     a 'Assert_MonSrcLoc' rather than the '<file>, <line-number>'
 *     couplet.
 *
 *   o Assert_MonSrcLoc encodes the '<file>, <line-number>' couplet
 *     into an unsigned 32-bit integer.  The upper 16-bits is the line
 *     number, and the lower 16-bits are the offset to the file name
 *     from the start of the table containing all the file names.
 *
 *     This, of course, implies that the size of the table containing
 *     the file names cannot exceed 64K.
 *
 *   o If we use '__FILE__' directly, gcc will coalesce all equivalent
 *     strings into a single occurrence (in '.rodata'), but this is
 *     undesirable because different source trees will frequently have
 *     different path name lengths, and this will causes the
 *     'monitor-modular-size' script to report differences in
 *     '.rodata'.
 *
 *   o To avoid differences in '.rodata', each __FILE__ is put into
 *     its own section. The linker will use the name of the section
 *     to recover the name of the file.
 *
 *   o Prior to loading, when linking the monitor and extensions, all
 *     the file names are extracted from these sections, the
 *     '${VMTREE}' prefix is removed, and the resulting table of
 *     shortened file names are added to '.rodata'.
 *
 *   o Prior to loading, when linking, each relocation to the original
 *     section containing the path name is modified so that the low
 *     16-bits contain an offset from '__vmm_pathnames_start' rather
 *     than the base of the original containing section.
 *
 *     Only three types of relocations to the assertion strings are
 *     supported (32-bit PC-relative and 32-bit/64-bit absolute) because that
 *     is all the compiler has been seen to generate.
 */


typedef uint32 Assert_MonSrcLoc;

#define __VMM__FILE__SECTION __attribute__((section (".assert_pathname_" __FILE__)))
#define __VMM__FILE__ ({                                                \
         static __VMM__FILE__SECTION const char file[] = "";            \
         file;                                                          \
      })

#define ASSERT_MONSRCFILEOFFSET(loc)    LOWORD(loc)
#define ASSERT_MONSRCLINE(loc)          HIWORD(loc)

#define ASSERT_NULL_MONSRCLOC     0             // there is never line 0
#define ASSERT_ILLEGAL_MONSRCLOC  0xffffffff    // and never 4 billion files

#ifdef VMM // {
#ifdef MONITOR_APP // {

#define ASSERT_MONSRCLOC() ASSERT_NULL_MONSRCLOC

#else // } {

#define ASSERT_MONSRCLOC() ({                                           \
   const uintptr_t offset = ((__LINE__ << 16) +                         \
                             (uintptr_t)__VMM__FILE__);                 \
   const Assert_MonSrcLoc loc = offset;                                 \
   loc;                                                                 \
})

extern const char __vmm_pathnames_start;
#define ASSERT_MONSRCFILE(loc) \
   (&__vmm_pathnames_start + ASSERT_MONSRCFILEOFFSET(loc))


#define _ASSERT_PANIC(name)          ((name)(ASSERT_MONSRCLOC()))
#define _ASSERT_PANIC_BUG(bug, name) ((name##Bug)(ASSERT_MONSRCLOC(), bug))
#define _ASSERT_WARNING(name)        ((name)(ASSERT_MONSRCLOC()))

void AssertPanic(Assert_MonSrcLoc loc);
void AssertAssert(Assert_MonSrcLoc loc);
NORETURN void AssertNotImplemented(Assert_MonSrcLoc loc);
void AssertNotReached(Assert_MonSrcLoc loc);
void AssertPanicBug(Assert_MonSrcLoc loc, int bug);
void AssertAssertBug(Assert_MonSrcLoc loc, int bug);
NORETURN void AssertNotImplementedBug(Assert_MonSrcLoc loc, int bug);
void AssertNotReachedBug(Assert_MonSrcLoc loc, int bug);
void AssertNotTested(Assert_MonSrcLoc);

#endif // }
#endif // }

#endif
