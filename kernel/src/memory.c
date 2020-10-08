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
 *                                MACROS
 ****************************************************************************/

#define MSR(sys_reg, var) __asm__("MSR " #sys_reg " , %0"::"r"(var))
#define MRS(var, sys_reg) __asm__("MRS %0, " #sys_reg : "=r"(var));
#define ISB()             __asm__("ISB")

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
uint64_t *KernelMemoryMainTable; 

/* Linkedlist for free pages. */
node_t *KernelMemoryFreeHead = NULL;
node_t *KernelMemoryFreeTail = NULL;

/*****************************************************************************
 *                       KernelMemoryInitialize()
 ****************************************************************************/

void KernelMemoryInitialize(void)
{
  /* Local variables. */
  uint64_t sysCtrl = 0;

  /* Create linkedlist of free RAM pages. */
  KernelMemoryFreeHead = (node_t *) KernelMemoryRamStart;
  KernelMemoryFreeTail = (node_t *) KernelMemoryRamStart;

  /* Initialize linkedlist. */
  KernelMemoryFreeHead->next = NULL;
  KernelMemoryFreeHead->size = KernelMemoryRamEnd - KernelMemoryRamStart;

  /* Allocate one page. */
  KernelMemoryMainTable = (uint64_t *) KernelMemoryPageAllocate();

  /* Update TTBR register with address to L1 page table. */
  MSR(TTBR1_EL1, KernelMemoryMainTable);
  MSR(TCR_EL1, 0x0000000500100000);
  ISB();

  /* Enable MMU. */
  MRS(sysCtrl, SCTLR_EL1);
  sysCtrl |= 1;
  MSR(SCTLR_EL1,  sysCtrl);
  ISB();
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
