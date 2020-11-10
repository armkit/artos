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

void KernelThreadInitialize (void)
{
  /* Loop counters. */
  uint64_t curCpu      = 0;
  uint64_t curPriority = 0;

  /* Local variables. */
  thread_t *idleThread = NULL;

  /* Initialize Ready Queue. */
  for (curCpu = 0; curCpu < MAX_CPU; curCpu++)
  {
    for (curPriority = 0; curPriority < MAX_PRIORITY; curPriority++)
    {
      KernelThreadsReadyQuHead[curCpu][curPriority] = NULL;
      KernelThreadsReadyQuTail[curCpu][curPriority] = NULL;
    }
    KernelThreadsRunningThread[curCpu] = NULL;
  }

  /* CREATE IDLE THREAD FOR EVERY PROCESSOR. PRIORITY = 0 */
  for(curCpu = 0; curCpu < MAX_CPU; curCpu ++)
  {
    idleThread = KernelPortThreadAllocate();
    idleThread->threadCpu = curCpu;
    idleThread->threadPriority = 0;
    idleThread->nextReadyThread = NULL;
    KernelThreadsReadyQuHead[curCpu][0] = idleThread;
    KernelThreadsReadyQuTail[curCpu][0] = idleThread;
  }

  /* Calling Admit. */
  KernelThreadAdmit(idleThread);

}

/*****************************************************************************
 *                        KernelThreadAdmit()
 ****************************************************************************/

void KernelThreadAdmit (thread_t *thread)
{
  /* Simplifying variables. */
  uint64_t threadCpu      = thread->threadCpu;
  uint64_t threadPriority = thread->threadPriority;
  thread_t *threadTail    = KernelThreadsReadyQuTail[threadCpu][threadPriority];
  thread_t *threadHead    = KernelThreadsReadyQuHead[threadCpu][threadPriority];

  /* Enqueue */
  if(threadTail == NULL)
  {
    threadTail = thread;
    threadHead = thread;
  }
  else
  {
    threadTail->nextReadyThread = thread;
    threadTail = thread;
  }
  threadHead = threadHead;
}

/*****************************************************************************
 *                       KernelThreadDispatch()
 ****************************************************************************/

uint64_t KernelThreadDispatch (thread_t *thread)
{
  /* Simplifying variables. */
  uint64_t threadCpu       = thread->threadCpu;
  uint64_t threadPriority  = thread->threadPriority;
  thread_t *threadTail     = KernelThreadsReadyQuTail[threadCpu][threadPriority];
  thread_t *threadHead     = KernelThreadsReadyQuHead[threadCpu][threadPriority];

  /* Dequeue */
  threadHead = threadHead->nextReadyThread;

  if(threadHead == NULL)
  {
    threadTail = NULL;
  }

  threadTail = threadTail;

  return 0;
}

/*****************************************************************************
 *                         KernelThreadRun()
 ****************************************************************************/

void KernelThreadRun (uint64_t threadId)
{
  /* Put thread on the CPU and RESTORE its context. */

  /* Update KernelThreadsRunningThread */

  (void) threadId;
}

/*****************************************************************************
 *                        KernelThreadPause()
 ****************************************************************************/

uint64_t KernelThreadPause (void)
{
  /* Read KernelThreadsRunningThread */

  /* Remove thread from the CPU and STORE its context. */

  return 0;
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
