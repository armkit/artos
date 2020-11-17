/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/src/process.c
 * @brief  ARTOS kernel process-management module.
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

/* Process count (from the port). */
#define PROCESS_COUNT    (0x10000U)

/*****************************************************************************
 *                           STATIC VARIABLES
 ****************************************************************************/

static process_t KernelProcessList[PROCESS_COUNT];

static process_t *KernelProcessFreeHead;
static process_t *KernelProcessFreeTail;

/*****************************************************************************
 *                       KernelProcessInitialize()
 ****************************************************************************/

void KernelProcessInitialize (void)
{
  /* Pointer to next free process. */
  process_t *nextFreeProcess = NULL;

  /* Loop counter. */
  uint64_t   curProcess      = 0;

  /* Initialize head and tail for process list. */
  KernelProcessFreeHead = &KernelProcessList[0];
  KernelProcessFreeTail = &KernelProcessList[PROCESS_COUNT - 1];

  /* Initialize process list. */
  for (curProcess = 0; curProcess < PROCESS_COUNT; curProcess++)
  {
    /* Compute pointer to next process. */
    if (curProcess == PROCESS_COUNT - 1)
    {
      nextFreeProcess = NULL;
    }
    else
    {
      nextFreeProcess = &KernelProcessList[curProcess + 1];
    }

    /* Initialize process structure. */
    KernelProcessList[curProcess].isUsed          = 0;
    KernelProcessList[curProcess].processId       = curProcess;
    KernelProcessList[curProcess].nextFreeProcess = nextFreeProcess;
  }
}

/*****************************************************************************
 *                        KernelProcessAllocate()
 ****************************************************************************/

process_t *KernelProcessAllocate (void)
{
  /* Pointer to process structure to be returned. */
  process_t *process = NULL;

  /* Check if there is no free process. */
  if (KernelProcessFreeHead == NULL)
  {
    return NULL;
  }

  /* Allocate new process from head. */
  process = KernelProcessFreeHead;
  KernelProcessFreeHead = process->nextFreeProcess;

  /* Initialize the new process. */
  process->isUsed          = 1;
  process->nextFreeProcess = NULL;

  /* Done. */
  return process;
}

/*****************************************************************************
 *                       KernelProcessDeallocate()
 ****************************************************************************/

void KernelProcessDeallocate (process_t *process)
{
  /* Free up the process. */
  process->isUsed          = 0;
  process->nextFreeProcess = NULL;

  /* Update the tail of the process list. */
  KernelProcessFreeTail->nextFreeProcess = process;
  KernelProcessFreeTail                  = process;
}

/*****************************************************************************
 *                          KernelProcessGet()
 ****************************************************************************/

process_t *KernelProcessGet (uint64_t processId)
{
  /* Pointer to process structure to be returned. */
  process_t *process = NULL;

  /* Make sure processId is within limits. */
  if (processId >= PROCESS_COUNT)
  {
    return NULL;
  }

  /* Obtain process structure by id. */
  process = &KernelProcessList[processId];

  /* Check whether the process is used or not. */
  if (process->isUsed == 0)
  {
    return NULL;
  }

  /* Done. */
  return process;
}