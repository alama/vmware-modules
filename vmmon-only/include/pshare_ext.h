/*********************************************************
 * Copyright (C) 2001 VMware, Inc. All rights reserved.
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
 * pshare_ext.h --
 *
 *      VMKernel/VMMon <-> VMM transparent page sharing info.
 */

#ifndef _PSHARE_EXT_H
#define _PSHARE_EXT_H

#define INCLUDE_ALLOW_USERLEVEL

#define INCLUDE_ALLOW_VMKERNEL
#define INCLUDE_ALLOW_VMMON
#define INCLUDE_ALLOW_VMCORE
#define INCLUDE_ALLOW_MODULE
#include "includeCheck.h"

#include "pagelist.h"
#include "vm_basic_types.h"
#include "vm_assert.h"

EXTERN MPN shareMPN;

/*
 * constants
 */

#define PSHARE_DEFAULT_SCAN_RATE        (32)

#define PSHARE_PAGELIST_MAX             (PAGELIST_MAX)
#define PSHARE_PAGELIST_MIN             (8)
#define PSHARE_P2M_BUFFER_MPNS_MAX      (8)
#define PSHARE_P2M_BUFFER_MPNS_DEFAULT  (2)
#define PSHARE_HINT_UPDATES_MAX         (PSHARE_PAGELIST_MAX)
#define PSHARE_HINT_PAGELIST_MAX        (PSHARE_PAGELIST_MAX)
#define PSHARE_P2M_BUFFER_SLOTS_PER_MPN (PAGE_SIZE / sizeof(PShare_P2MUpdate))
#define PSHARE_BACKDOOR_HINTS_MAX       (PAGE_SIZE / sizeof(BPN))

#define PSHARE_POISON_MARKER            (CONST64U(0xAAAAAAAAAAAAAAAA))

MY_ASSERTS(PSHARE_EXT,
           ASSERT_ON_COMPILE(PSHARE_HINT_UPDATES_MAX <= PSHARE_PAGELIST_MAX &&
                             PSHARE_PAGELIST_MAX <= PAGELIST_MAX);)

/*
 * types
 */

typedef struct PShare_P2MUpdate {
   BPN     bpn;
   MPN     mpn;
} PShare_P2MUpdate;

typedef uint8 PShareFlags;

/*
 * Config information that is used by the platform to implement
 * dynamic scan rate distribution across multiple running VMs.
 */

typedef struct PShare_MgmtInfo {
   uint16      minScanRate;
   uint16      maxScanRate;
   uint16      curScanRate;
   PShareFlags flags;
   uint8       _pad[1];
} PShare_MgmtInfo;

void BusMemPShare_HandleBackdoor(void);
#endif
