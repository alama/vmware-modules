/*********************************************************
 * Copyright (C) 2015 VMware, Inc. All rights reserved.
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

#include "compat_version.h"
#include "compat_autoconf.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 19, 0)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
#include <linux/dcache.h>
#include <linux/list.h>

/*
 * After 3.19.0, the dentry d_alias field was moved. Fedora
 * backported this behavior into a 3.18.0 kernel.
 *
 * This test will fail on a kernel with such a patch.
 */
void test(void)
{
   struct dentry aliasDentry;

   INIT_HLIST_NODE(&aliasDentry.d_alias);
}

#else
/* Intentionally passes for earlier than 3.9.0 kernels as d_alias is valid. */
#endif
#else
#error "This test intentionally fails on 3.19.0 or newer kernels."
#endif
