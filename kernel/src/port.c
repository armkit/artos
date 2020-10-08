/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/src/port.c
 * @brief  ARTOS kernel port module.
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
 *                           GLOBAL VARIABLES
 ****************************************************************************/

/* Pointer to L1 page table. */
uint64_t *KernelPortPageTable = NULL;

/*****************************************************************************
 *                       KernelPortMemoryInitialize()
 ****************************************************************************/

void KernelPortMemoryInitialize(void)
{
  /* Local variables. */
  uint64_t sysCtrl = 0;

  /* Allocate one page. */
  KernelPortPageTable = (uint64_t *) KernelPortMemoryPageAllocate();

  /* Update TTBR register with address to L1 page table. */
  MSR(TTBR1_EL1, KernelPortPageTable);
  ISB();

  /* Update TCR register with PIN values. */ 
  MSR(TCR_EL1, 0x0000000500100000);
  ISB();

  /* Enable MMU. */
  MRS(sysCtrl, SCTLR_EL1);
  sysCtrl |= 1;
  MSR(SCTLR_EL1,  sysCtrl);
  ISB();
}
