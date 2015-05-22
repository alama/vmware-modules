/*********************************************************
 * Copyright (C) 2010-2013 VMware, Inc. All rights reserved.
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
 *      Definitions of operations on BPNs used in communicating page info
 *	between VMKernel/VMX and VMM.
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
 * Sets of pages are passed between the monitor and the platform to be 
 * shared, invalidated, remapped, or swapped.
 *
 * The upper bit of each (tagged) BPN is used to encode specific state 
 * about each page:
 *
 *     bit 31: the entry has been voided/rejected
 *
 * A set is sized so that it fits in a 4KB page.
 */

#define PAGELIST_MAX     512
#define PAGELIST_VOID    (1 << 31)

MY_ASSERTS(PAGELISTDEFS, 
           ASSERT_ON_COMPILE(PAGELIST_MAX * sizeof(BPN) <= PAGE_SIZE);)

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

/*
 * This function inspects the set of BPN between entry [0,i) in the page list
 * and returns TRUE if any of them matches the provided BPN.
 */
static INLINE Bool
PageList_IsBPNDup(const BPN *bpnList, unsigned i, BPN bpn)
{
   unsigned k;
   ASSERT(!PageList_IsVoid(bpn));
   for (k = 0; k < i; k++) {
      if (PageList_BPN(bpnList[k]) == bpn) {
         return TRUE;
      }
   }
   return FALSE;
}

#endif
