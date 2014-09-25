/*********************************************************
 * Copyright (C) 2010 VMware, Inc. All rights reserved.
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
 * pagelist.h -- 
 *
 *      Definitions of constants/structs used in communicating 
 *	page info. between VMKernel/VMMon and VMM.
 */

#ifndef	_PAGELIST_H
#define	_PAGELIST_H

#define INCLUDE_ALLOW_USERLEVEL
#define INCLUDE_ALLOW_VMKERNEL
#define INCLUDE_ALLOW_VMMON
#define INCLUDE_ALLOW_VMCORE
#define INCLUDE_ALLOW_MODULE
#include "includeCheck.h"

#include "vm_assert.h"

/*
 * PageList is used for communicating sets of BPNs/MPNs between the monitor
 * and platform.  The most common use is for passing sets of pages to be 
 * shared, remapped, or swapped.
 *
 * The upper bit of each (tagged) BPN is used to encode specific state 
 * about each page:
 *
 *     bit 31: the entry has been voided/rejected
 *
 * The page-list structure is sized so that it fits in a 4KB page.
 */

#define PAGELIST_MAX     (PAGE_SIZE / (sizeof(BPN) + sizeof(MPN)))
#define PAGELIST_VOID    (1 << 31)

typedef struct PageList {
   BPN bpnList[PAGELIST_MAX];
   MPN mpnList[PAGELIST_MAX];
} PageList;

MY_ASSERTS(PAGELISTDEFS, 
           ASSERT_ON_COMPILE(sizeof(PageList) <= PAGE_SIZE);)

static INLINE BPN
PageList_VoidBPN(BPN bpn) 
{
   return bpn | PAGELIST_VOID;
}

static INLINE BPN
PageList_BPN(BPN taggedBPN)
{
   return taggedBPN & ~PAGELIST_VOID;
}

static INLINE Bool
PageList_IsVoid(BPN taggedBPN)
{
   return (taggedBPN & PAGELIST_VOID) != 0;
}

#endif
