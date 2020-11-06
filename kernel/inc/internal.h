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

/* Port-specific process struct should include process_t at the start. */
typedef struct process
{
  uint8_t        name[KERNEL_CONFIG_NAME_MAX_SIZE];
} __attribute__((packed)) process_t;

/* Port-specific thread struct should include thread_t at the start. */
typedef struct thread
{
  uint8_t        name[KERNEL_CONFIG_NAME_MAX_SIZE];
  struct thread *nextReadyThread;
} __attribute__((packed)) thread_t;

/*****************************************************************************
 *                          FUNCTION PROTOTYPES
 ****************************************************************************/

/* CPU-Specific Serial I/O. */
void KernelPortSerialInitialize(void);
void KernelPortSerialPut(char c);
char KernelPortSerialGet(void);

/* CPU-Specific Address Translation. */
void  KernelPortTranslationInitialize(void);
void *KernelPortTranslationSet(void *virtualAddr, void *physicalAddr);
void *KernelPortTranslationGet(void *virtualAddr);
void *KernelPortTranslationDel(void *virtualAddr);

/* CPU-Specific Process Handling. */
void       KernelPortProcessInitialize(void);
process_t *KernelPortProcessAllocate(void);
void       KernelPortProcessDeallocate(process_t *process);
process_t *KernelPortProcessGet(uint64_t processId);
uint64_t   KernelPortProcessId(process_t *process);

/* CPU-Specific Thread Handling. */
void      KernelPortThreadInitialize(void);
thread_t *KernelPortThreadAllocate(void);
void      KernelPortThreadDeallocate(thread_t *thread);
thread_t *KernelPortThreadGet(uint64_t threadId);
uint64_t  KernelPortThreadId(thread_t *thread);

/*****************************************************************************
 *                            END OF HEADER
 ****************************************************************************/

#endif /* KERNEL_INTERNAL_H */
