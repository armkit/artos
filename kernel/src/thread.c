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

/* Thread count (from the port). */
#define THREAD_COUNT     (0x10000U)

/* Maximum CPU count and priorities. */
#define MAX_CPU          (KERNEL_CONFIG_MAX_CPU_COUNT)
#define MAX_PRIORITY     (KERNEL_CONFIG_MAX_PRIOIRTY )

/*****************************************************************************
 *                           STATIC VARIABLES
 ****************************************************************************/

static thread_t  KernelThreadList[THREAD_COUNT];

static thread_t *KernelThreadFreeHead;
static thread_t *KernelThreadFreeTail;

static thread_t *KernelThreadReadyQuHead[MAX_CPU][MAX_PRIORITY];
static thread_t *KernelThreadReadyQuTail[MAX_CPU][MAX_PRIORITY];

static thread_t *KernelThreadRunning[MAX_CPU];

/*****************************************************************************
 *                       KernelThreadInitialize()
 ****************************************************************************/

void KernelThreadInitialize (void)
{
  /* Loop counters. */
  uint64_t curThread      = 0;
  uint64_t curCpu         = 0;
  uint64_t curPriority    = 0;

  /* Local variables. */
  thread_t *idleThread     = NULL;
  thread_t *nextFreeThread = NULL;

  /* Initialize head and tail for thread list. */
  KernelThreadFreeHead = &KernelThreadList[0];
  KernelThreadFreeTail = &KernelThreadList[THREAD_COUNT - 1];

  /* Initialize thread list. */
  for (curThread = 0; curThread < THREAD_COUNT; curThread++)
  {
    /* Compute pointer to next free thread. */
    if(curThread == THREAD_COUNT - 1)
    {
      nextFreeThread = NULL;
    }
    else
    {
      nextFreeThread = &KernelThreadList[curThread + 1];
    }

    /* Initialize thread structure. */
    KernelThreadList[curThread].isUsed          = 0;
    KernelThreadList[curThread].threadId        = curThread;
    KernelThreadList[curThread].threadCpu       = 0;
    KernelThreadList[curThread].threadPriority  = 0;
    KernelThreadList[curThread].nextReadyThread = NULL;
    KernelThreadList[curThread].nextFreeThread  = nextFreeThread;
  }

  /* Initialize Ready Queue. */
  for (curCpu = 0; curCpu < MAX_CPU; curCpu++)
  {
    for (curPriority = 0; curPriority < MAX_PRIORITY; curPriority++)
    {
      KernelThreadReadyQuHead[curCpu][curPriority] = NULL;
      KernelThreadReadyQuTail[curCpu][curPriority] = NULL;
    }
    KernelThreadRunning[curCpu] = NULL;
  }

  /* CREATE IDLE THREAD FOR EVERY PROCESSOR. PRIORITY = 0 */
  for(curCpu = 0; curCpu < MAX_CPU; curCpu ++)
  {
    /* Allocate new thread. */
    idleThread = KernelThreadAllocate(curCpu, 0);

    /* Admit the idle thread into the ready queue. */
    KernelThreadReadyQuHead[curCpu][0] = idleThread;
    KernelThreadReadyQuTail[curCpu][0] = idleThread;
  }
}

/*****************************************************************************
 *                         KernelThreadAllocate()
 ****************************************************************************/

thread_t *KernelThreadAllocate (uint64_t threadCpu,
                                uint64_t threadPriority)
{
  /* Thread to be allocated. */
  thread_t *thread = NULL;

  /* Check if there is no free thread. */
  if (KernelThreadFreeHead == NULL)
  {
    return NULL;
  }

  /* Allocate new thread from head. */
  thread = KernelThreadFreeHead;
  KernelThreadFreeHead = thread->nextFreeThread;

  /* Initialize the new thread. */
  thread->isUsed          = 1;
  thread->threadCpu       = threadCpu;
  thread->threadPriority  = threadPriority;
  thread->nextFreeThread  = NULL;
  thread->nextReadyThread = NULL;

  /* Done. */
  return thread;
}

/*****************************************************************************
 *                        KernelThreadDeallocate()
 ****************************************************************************/

void KernelThreadDeallocate (thread_t *thread)
{
  /* Free up the thread. */
  thread->isUsed         = 0;
  thread->nextFreeThread = NULL;

  /* Insert into the free thread list. */
  KernelThreadFreeTail->nextFreeThread = thread;
  KernelThreadFreeTail                 = thread;
}

/*****************************************************************************
 *                            KernelThreadGet()
 ****************************************************************************/

thread_t *KernelThreadGet (uint64_t threadId)
{
  /* Pointer to thread structure to be returned. */
  thread_t *thread = NULL;

  /* Make sure threadId is within limits. */
  if (threadId >= THREAD_COUNT)
  {
    return NULL;
  }

  /* Obtain the thread structure by Id. */
  thread = &KernelThreadList[threadId];

  /* Check whether the thread is already allocated or not. */
  if (thread->isUsed == 0)
  {
    return NULL;
  }

  /* Done. */
  return thread;
}

/*****************************************************************************
 *                        KernelThreadAdmit()
 ****************************************************************************/

void KernelThreadAdmit (thread_t *thread)
{
  /* Simplifying variables. */
  uint64_t threadCpu      = 0;
  uint64_t threadPriority = 0;
  thread_t *threadHead    = NULL;
  thread_t *threadTail    = NULL;

  /* Obtain thread CPU and priority. */
  threadCpu = thread->threadCpu;
  threadPriority = thread->threadPriority;

  /* Obtain ready queue head and tail. */
  threadHead = KernelThreadReadyQuHead[threadCpu][threadPriority];
  threadTail = KernelThreadReadyQuTail[threadCpu][threadPriority];

  /* Enqueue */
  if(threadTail == NULL)
  {
    threadHead = thread;
    threadTail = thread;
  }
  else
  {
    threadTail->nextReadyThread = thread;
    threadTail = thread;
  }

  /* Store the new values of ready queue head and tail. */
  KernelThreadReadyQuTail[threadCpu][threadPriority] = threadTail;
  KernelThreadReadyQuHead[threadCpu][threadPriority] = threadHead;
}

/*****************************************************************************
 *                       KernelThreadDispatch()
 ****************************************************************************/

thread_t *KernelThreadDispatch (uint64_t threadCpu,
                                uint64_t threadPriority)
{
  /* Thread to be returned. */
  thread_t *thread     = NULL;

  /* Simplifying variables. */
  thread_t *threadHead = NULL;
  thread_t *threadTail = NULL;

  /* Obtain ready queue head and tail. */
  threadHead = KernelThreadReadyQuHead[threadCpu][threadPriority];
  threadTail = KernelThreadReadyQuTail[threadCpu][threadPriority];

  /* Check whether the ready queue is empty or not. */
  if (threadHead == NULL)
  {
    return NULL;
  }

  /* Dequeue */
  threadHead = threadHead->nextReadyThread;
  if (threadHead == NULL)
  {
    threadTail = NULL;
  }

  /* Clean up nextReadyThread pointer. */
  thread->nextReadyThread = NULL;

  /* Store the new values of ready queue head and tail. */
  KernelThreadReadyQuHead[threadCpu][threadPriority] = threadHead;
  KernelThreadReadyQuTail[threadCpu][threadPriority] = threadTail;

  /* Done. */
  return thread;
}

/*****************************************************************************
 *                         KernelThreadRun()
 ****************************************************************************/

void KernelThreadRun (uint64_t threadId)
{
  /* Put thread on the CPU and RESTORE its context. */

  /* Update KernelThreadRunning */

  (void) threadId;
}

/*****************************************************************************
 *                        KernelThreadPause()
 ****************************************************************************/

uint64_t KernelThreadPause (void)
{
  /* Read KernelThreadRunning */

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
