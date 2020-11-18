/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/inc/internal.h
 * @brief  Kernel internal header file.
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
 *                             SAFE GUARD
 ****************************************************************************/

#ifndef KERNEL_INTERNAL_H
#define KERNEL_INTERNAL_H

/*****************************************************************************
 *                              INCLUDES
 ****************************************************************************/

/* Kernel interface header. */
#include "kernel/inc/interface.h"

/*****************************************************************************
 *                              TYPEDEFS
 ****************************************************************************/

/* Structure to hold process information. */
typedef struct process
{
  uint64_t             isUsed;
  uint8_t              processName[KERNEL_CONFIG_NAME_MAX_SIZE];
  uint64_t             processId;
  struct process      *nextFreeProcess;
} __attribute__((packed)) process_t;

/* Structure to hold thread information. */
typedef struct thread
{
  uint64_t            isUsed;
  uint8_t             threadName[KERNEL_CONFIG_NAME_MAX_SIZE];
  uint64_t            threadId;
  uint64_t            threadCpu;
  uint64_t            threadPriority;
  struct thread      *nextReadyThread;
  struct thread      *nextFreeThread;
} __attribute__((packed)) thread_t;

/*****************************************************************************
 *                          FUNCTION PROTOTYPES
 ****************************************************************************/

/* Memory module. */
void        KernelMemoryInitialize     (void);
void       *KernelMemoryPageAllocate   (void);
void        KernelMemoryPageDeallocate (void *pageBaseAddr);

/* Process module. */
void        KernelProcessInitialize    (void);
process_t  *KernelProcessAllocate      (void);
void        KernelProcessDeallocate    (process_t *process);
process_t  *KernelProcessGet           (uint64_t processId);

/* Thread module. */
void        KernelThreadInitialize     (void);
thread_t   *KernelThreadAllocate       (uint64_t threadCpu,
                                        uint64_t threadPriority);
void        KernelThreadDeallocate     (thread_t *thread);
thread_t   *KernelThreadGet            (uint64_t threadId);
void        KernelThreadAdmit          (thread_t *thread);
thread_t   *KernelThreadDispatch       (uint64_t threadCpu,
                                        uint64_t threadPriority);
void        KernelThreadRun            (uint64_t threadId);
uint64_t    KernelThreadPause          (void);

/*****************************************************************************
 *                            END OF HEADER
 ****************************************************************************/

#endif /* KERNEL_INTERNAL_H */
