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

static thread_t *KernelThreadsReadyQuHead[MAX_CPU][MAX_PRIORITY];
static thread_t *KernelThreadsReadyQuTail[MAX_CPU][MAX_PRIORITY];

static thread_t *KernelThreadsRunningThread[MAX_CPU];

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
      KernelThreadsReadyQuHead[curCPU][curPriority] = NULL;
      KernelThreadsReadyQuTail[curCPU][curPriority] = NULL;
    }
    KernelThreadsRunningThread[curCPU] = NULL;
  }

  /* CREATE IDLE THREAD FOR EVERY PROCESSOR. PRIORITY = 0 */

  /* After creation for each idle thread, call:
   * KernelThreadAdmit(idleThreadId)
   */
}

/*****************************************************************************
 *                        KernelThreadAdmit()
 ****************************************************************************/

void KernelThreadAdmit(uint64_t threadId)
{
  /* Enqueue */

  /* Deals with KernelThreadsReadyQuHead and KernelThreadsReadyQuTail */
}

/*****************************************************************************
 *                       KernelThreadDispatch()
 ****************************************************************************/

uint64_t KernelThreadDispatch (void)
{
  /* Dequeue */

  /* Deals with KernelThreadsReadyQuHead and KernelThreadsReadyQuTail */
}

/*****************************************************************************
 *                         KernelThreadRun()
 ****************************************************************************/

void KernelThreadRun (uint64_t threadId)
{
  /* Put thread on the CPU and RESTORE its context. */

  /* Update KernelThreadsRunningThread */
}

/*****************************************************************************
 *                        KernelThreadPause()
 ****************************************************************************/

uint64_t KernelThreadPause (void)
{
  /* Read KernelThreadsRunningThread */

  /* Remove thread from the CPU and STORE its context. */
}

/*****************************************************************************
 *                        KernelThreadCreate()
 ****************************************************************************/

void KernelThreadCreate (void)
{
}

/*****************************************************************************
 *                       KernelThreadYield()
 ****************************************************************************/

void KernelThreadYield (void)
{
}

/*****************************************************************************
 *                        KernelThreadBlock()
 ****************************************************************************/

void KernelThreadBlock (void)
{
}

/*****************************************************************************
 *                       KernelThreadUnblock()
 ****************************************************************************/

void KernelThreadUnblock (void)
{
}

/*****************************************************************************
 *                      KernelThreadTerminate()
 ****************************************************************************/

void KernelThreadTerminate (void)
{
}

/*****************************************************************************
 *                       KernelThreadJoin()
 ****************************************************************************/

void KernelThreadJoin (void)
{
}
