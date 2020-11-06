/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/src/thread.c
 * @brief  ARTOS kernel thread-management module.
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
 *                               MACROS
 ****************************************************************************/

/* Maximum CPU count and priorities. */
#define MAX_CPU          (KERNEL_CONFIG_MAX_CPU_COUNT)
#define MAX_PRIORITY     (KERNEL_CONFIG_MAX_PRIOIRTY )

/*****************************************************************************
 *                           STATIC VARIABLES
 ****************************************************************************/

static thread_t *KernelThreadsReadyQueue[MAX_CPU][MAX_PRIORITY];

/*****************************************************************************
 *                       KernelThreadInitialize()
 ****************************************************************************/

void KernelThreadInitialize(void)
{
  /* Loop counters. */
  uint64_t curCPU      = 0;
  uint64_t curPriority = 0;

  /* Initialize Ready Queue. */
  for (curCPU = 0; curCPU < MAX_CPU; curCPU++)
  {
    for (curPriority = 0; curPriority < MAX_PRIORITY; curPriority++)
    {
      KernelThreadsReadyQueue[curCPU][curPriority] = NULL;
    }
  }
}
