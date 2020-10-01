/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/src/memory.c
 * @brief  ARTOS kernel memory module.
 ***************************************************************************
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 ****************************************************************************/

/*****************************************************************************
 *                              INCLUDES
 ****************************************************************************/

/* Kernel includes. */
#include "kernel/inc/interface.h"
#include "kernel/inc/internal.h"

/*****************************************************************************
 *                           GLOBAL VARIABLES
 ****************************************************************************/

/* RAM information. */
uint64_t KernelMemoryRamStart = KERNEL_CONFIG_RAM_START;
uint64_t KernelMemoryRamEnd   = KERNEL_CONFIG_RAM_END;

/* Linkedlist for free pages. */
uint8_t *KernelMemoryFreeHead = NULL;
uint8_t *KernelMemoryFreeTail = NULL;

/*****************************************************************************
 *                       KernelMemoryInitialize()
 ****************************************************************************/

void KernelMemoryInitialize(void)
{
  /* Local variables. */
  uint8_t *firstPage = (uint8_t *) (KernelMemoryRamStart);
  uint8_t *lastPage  = (uint8_t *) (KernelMemoryRamEnd - PAGE_SIZE);
  uint8_t *curPage   = NULL;

  /* Create linkedlist of free RAM pages. */
  KernelMemoryFreeHead = firstPage;
  KernelMemoryFreeTail = lastPage;

  /* Loop over RAM pages and add them to linkedlist. */
  for (curPage = firstPage; curPage < lastPage; curPage += PAGE_SIZE)
  {
    *((uint64_t *) curPage) = ((uint64_t) curPage) + PAGE_SIZE;
  }

  /* Last page should be a null pointer. */
  *((uint64_t *) curPage) = (uint64_t) NULL;
}

/*****************************************************************************
 *                       KernelMemoryPageAllocate()
 ****************************************************************************/

void *KernelMemoryPageAllocate(void)
{
  /* Local variables */

  //if (KernelMemoryFreeHead == NULL) {

  //}

  return NULL;
}

/*****************************************************************************
 *                      KernelMemoryPageDeallocate()
 ****************************************************************************/

void KernelMemoryPageDeallocate(void *pageBaseAddr)
{
  (void) pageBaseAddr;
}
