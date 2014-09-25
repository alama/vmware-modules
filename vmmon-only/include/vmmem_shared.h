/*********************************************************
 * Copyright (C) 2000-2012 VMware, Inc. All rights reserved.
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
 * vmmem_shared.h --
 *
 *	This is the header file for machine memory manager.
 */


#ifndef _VMMEM_SHARED_H
#define _VMMEM_SHARED_H

#define INCLUDE_ALLOW_USERLEVEL
#define INCLUDE_ALLOW_VMCORE
#define INCLUDE_ALLOW_VMMON
#define INCLUDE_ALLOW_VMKERNEL
#define INCLUDE_ALLOW_VMX
#include "includeCheck.h"

/*
 * Page remapping definitions.
 */

// Type of Page Operations
typedef enum VmMemRemap_Type {
   VMMEM_REMAP_NONE,
   VMMEM_REMAP_LPAGE,
   VMMEM_REMAP_SPAGE,
   VMMEM_REMAP_NUMA
} VmMemRemap_Type;

#define VMMEM_REMAP_IS_LPAGE(x)     ((x) == VMMEM_REMAP_LPAGE)
#define VMMEM_REMAP_IS_SPAGE(x)     ((x) == VMMEM_REMAP_SPAGE)
#define VMMEM_REMAP_IS_NUMA(x)      ((x) == VMMEM_REMAP_NUMA)
#define VMMEM_REMAP_IS_COLOR(x)     ((x) == VMMEM_REMAP_COLOR)

#define VMMEM_FLAG_BIT(x) (1 << (x))

#define VMMEM_ANON_LOW_MEM        VMMEM_FLAG_BIT(0)
#define VMMEM_ANON_LARGE_PAGE     VMMEM_FLAG_BIT(1)
#define VMMEM_ANON_CAN_FAIL       VMMEM_FLAG_BIT(2)
#define VMMEM_ANON_USE_PREALLOC   VMMEM_FLAG_BIT(3)
#define VMMEM_ANON_IOABLE_PAGE    VMMEM_FLAG_BIT(4)
#define VMMEM_ANON_ALL_FLAGS      MASK(5)

#define VMMEM_GUEST_WRITEABLE     VMMEM_FLAG_BIT(0)
#define VMMEM_GUEST_BREAKCOW      VMMEM_FLAG_BIT(1)
#define VMMEM_GUEST_LARGE_PAGE    VMMEM_FLAG_BIT(2)
#define VMMEM_GUEST_CAN_FAIL      VMMEM_FLAG_BIT(3)
#define VMMEM_GUEST_TRY_ZEROCOW   VMMEM_FLAG_BIT(4)
#define VMMEM_GUEST_TRY_POISONCOW VMMEM_FLAG_BIT(5)
#define VMMEM_GUEST_ALL_FLAGS     MASK(6)
#define VMMEM_GUEST_TRY_COW       (VMMEM_GUEST_TRY_ZEROCOW | \
                                   VMMEM_GUEST_TRY_POISONCOW)

#define VMMEM_PLATFORM_CHECK_OK           VMMEM_FLAG_BIT(0)
#define VMMEM_PLATFORM_KEY_OK             VMMEM_FLAG_BIT(1)
#define VMMEM_PLATFORM_COW                VMMEM_FLAG_BIT(2)
#define VMMEM_PLATFORM_EXPOSED_TO_VMM     VMMEM_FLAG_BIT(3)
#define VMMEM_PLATFORM_P2M_UPDATE_PENDING VMMEM_FLAG_BIT(4)
#define VMMEM_PLATFORM_DIRTY              VMMEM_FLAG_BIT(5)
#define VMMEM_PLATFORM_BACKED_LARGE       VMMEM_FLAG_BIT(6)
#define VMMEM_PLATFORM_ALL_FLAGS          MASK(7)

//TODO: Adjust the value according to the size of new structure BusmemServices_PageInfo
#define MAX_PLATFORM_PAGE_INFO_PAGES  (16) // limited by rpc block size

/*
 * Structure used to query platform about the page state.
 */
typedef struct PlatformPageInfo {
   MPN    hostMPN;                 // mpn for this page in the host
   uint8  flags; 
} PlatformPageInfo;

typedef struct PlatformPageInfoList {
   uint32           numPages;
   BPN              bpn[MAX_PLATFORM_PAGE_INFO_PAGES]; // bpns to check
   PlatformPageInfo info[MAX_PLATFORM_PAGE_INFO_PAGES];
} PlatformPageInfoList;

#define VMMEM_SERVICES_TYPE_2_MASK(type)                  \
           (1 << type)
#define VMMEM_SERVICES_IN_MASK(typeMask, type)            \
           (VMMEM_SERVICES_TYPE_2_MASK(type) & typeMask)
#define VMMEM_SERVICES_CLEAR_MASK(typeMask, type)         \
           (typeMask & ~VMMEM_SERVICES_TYPE_2_MASK(type))

#define VMMEM_SERVICES_DEFS                               \
   MDEF(VMMEM_SERVICES_TYPE_P2M,  P2MUpdate_FilterPages)  \
   MDEF(VMMEM_SERVICES_TYPE_SWAP, BusMemSwap_FilterPages)

#define MDEF(_type, _cb) _type,
typedef enum VmMemServices_Type {
   VMMEM_SERVICES_DEFS
#undef MDEF
   VMMEM_SERVICES_TYPE_MAX
} VmMemServices_Type;
#define VMMEM_SERVICES_TYPE_INVALID (VMMEM_SERVICES_TYPE_MAX)

#ifdef VMX86_SERVER
#define VMMEMPERIODIC_DEFS              \
   MDEF(VMMEMPERIODIC_TYPE_RELIABLEMEM, \
        ReliableMem_PickupVmPages,      \
        BusMemRemap_FilterPages,        \
        ReliableMem_CompleteVmPages)
#else
#define VMMEMPERIODIC_DEFS
   // MDEF(_type, _selectFn, _filterFn, _completeFn)
#endif

#define MDEF(_type, _selectFn, _filterFn, _completeFn) _type,
typedef enum VmMemPeriodic_Type {
   VMMEMPERIODIC_DEFS
   VMMEMPERIODIC_TYPE_MAX,
} VmMemPeriodic_Type;
#undef MDEF

#define VMMEMPERIODIC_TYPE_INVALID (VMMEMPERIODIC_TYPE_MAX)

void VmMem_DisableLargePageAllocations(void);
void VmMem_EnableLargePageAllocations(void);

#endif
