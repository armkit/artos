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
 *                              TYPEDEFS
 ****************************************************************************/

typedef struct node {
  struct node *next;
  uint64_t     size;
} node_t;

/*****************************************************************************
 *                           GLOBAL VARIABLES
 ****************************************************************************/

/* RAM information. */
uint64_t KernelMemoryRamStart = KERNEL_CONFIG_RAM_START;
uint64_t KernelMemoryRamEnd   = KERNEL_CONFIG_RAM_END;

/* Linkedlist for free pages. */
node_t *KernelMemoryFreeHead = NULL;
node_t *KernelMemoryFreeTail = NULL;

/*****************************************************************************
 *                       KernelMemoryInitialize()
 ****************************************************************************/

void KernelMemoryInitialize(void)
{
  /* Create linkedlist of free RAM pages. */
  KernelMemoryFreeHead = (node_t *) KernelMemoryRamStart;
  KernelMemoryFreeTail = (node_t *) KernelMemoryRamStart;

  /* Initialize linkedlist. */
  KernelMemoryFreeHead->next = NULL;
  KernelMemoryFreeHead->size = KernelMemoryRamEnd - KernelMemoryRamStart;

  /* Let's assume memory consists of only 3 pages: */
  KernelMemoryFreeHead->size = 3*PAGE_SIZE;

  /* Allocate some pages. */
  KernelDebugPrintFmt("\n");
  KernelDebugPrintFmt("Page #1: %p\n", KernelMemoryPageAllocate());
  KernelDebugPrintFmt("Page #2: %p\n", KernelMemoryPageAllocate());
  KernelDebugPrintFmt("Page #3: %p\n", KernelMemoryPageAllocate());
  KernelDebugPrintFmt("Page #4: %p\n", KernelMemoryPageAllocate());

  /* Print linkedlist info. */
  KernelDebugPrintFmt("\n");
  KernelDebugPrintFmt("Head: %p\n", KernelMemoryFreeHead);
  KernelDebugPrintFmt("Tail: %p\n", KernelMemoryFreeTail);

  /* Free up a page */
  KernelMemoryPageDeallocate((void *) 0x0000000040002000);

  /* Print linkedlist info. */
  KernelDebugPrintFmt("\n");
  KernelDebugPrintFmt("Head: %p\n", KernelMemoryFreeHead);
  KernelDebugPrintFmt("Tail: %p\n", KernelMemoryFreeTail);

  /* Free up a page */
  KernelMemoryPageDeallocate((void *) 0x0000000040000000);

  /* Print linkedlist info. */
  KernelDebugPrintFmt("\n");
  KernelDebugPrintFmt("Head: %p\n", KernelMemoryFreeHead);
  KernelDebugPrintFmt("Tail: %p\n", KernelMemoryFreeTail);

  /* Allocate some pages. */
  KernelDebugPrintFmt("\n");
  KernelDebugPrintFmt("Page #1: %p\n", KernelMemoryPageAllocate());
  KernelDebugPrintFmt("Page #2: %p\n", KernelMemoryPageAllocate());
  KernelDebugPrintFmt("Page #3: %p\n", KernelMemoryPageAllocate());
  KernelDebugPrintFmt("Page #4: %p\n", KernelMemoryPageAllocate());
}

/*****************************************************************************
 *                       KernelMemoryPageAllocate()
 ****************************************************************************/

void *KernelMemoryPageAllocate(void)
{
  /* Local variables. */
  node_t *freePage = NULL;
  node_t *newHead  = NULL;

  /* Linkedlist is not empty? */
  if (KernelMemoryFreeHead != NULL) {
    /* Allocate the page at the head. */
    freePage = (node_t *) KernelMemoryFreeHead;

    /* Big page? */
    if (KernelMemoryFreeHead->size > PAGE_SIZE) {
      /* Split the page. */
      newHead = (node_t *) (((uint8_t *) freePage) + PAGE_SIZE);
      newHead->next = KernelMemoryFreeHead->next;
      newHead->size = KernelMemoryFreeHead->size - PAGE_SIZE;
      /* Update head. */
      KernelMemoryFreeHead = newHead;
    } else {
      /* Use the whole page. */
      newHead = KernelMemoryFreeHead->next;
      /* Update head. */
      KernelMemoryFreeHead = newHead;
    }

    /* Update tail if necessary */
    if (KernelMemoryFreeTail == freePage) {
      KernelMemoryFreeTail = KernelMemoryFreeHead;
    }
  }

  /* Return allocated page. */
  return freePage;
}

/*****************************************************************************
 *                      KernelMemoryPageDeallocate()
 ****************************************************************************/

void KernelMemoryPageDeallocate(void *pageBaseAddr)
{
  /* Local variables. */
  node_t *freePage = NULL;

  /* Read parameter. */
  freePage = (node_t *)(((uint64_t)pageBaseAddr)&~(((uint64_t)PAGE_SIZE)-1));

  /* Insert page into linkedlist. */
  if (KernelMemoryFreeHead == NULL) {
    KernelMemoryFreeHead  = freePage;
    KernelMemoryFreeTail  = freePage;
  } else {
    KernelMemoryFreeTail->next = freePage;
    KernelMemoryFreeTail       = freePage;
  }

  /* Re-initialize page info. */
  freePage->next = NULL;
  freePage->size = PAGE_SIZE;
}
