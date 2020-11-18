/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   port/src/translation.c
 * @brief  ARTOS port module: address translation.
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

/* Port includes. */
#include "port/inc/interface.h"
#include "port/inc/internal.h"

/*****************************************************************************
 *                          MEMORY ZONES MACROS
 ****************************************************************************/

/* Private memory zone. */
#define PRIMEM_ZONE_START       (0xFFFF000000000000UL)
#define PRIMEM_ZONE_END         (0xFFFF3FFFFFFFFFFFUL)
#define PRIMEM_ZONE_SLOTS       (64*1024)
#define PRIMEM_ZONE_SLOT_SIZE   (0x40000000UL)

/* Shared memory zone. */
#define SHMEM_ZONE_START        (0xFFFF400000000000UL)
#define SHMEM_ZONE_END          (0xFFFF7FFFFFFFFFFFUL)
#define SHMEM_ZONE_SLOTS        (64*1024)
#define SHMEM_ZONE_SLOT_SIZE    (0x40000000UL)

/* Reserved zone. */
#define RESERVED_ZONE_START     (0xFFFF80000000000UL)
#define RESERVED_ZONE_END       (0xFFFFBFFFFFFFFFFUL)
#define RESERVED_ZONE_SLOTS     (64*1024)
#define RESERVED_ZONE_SLOT_SIZE (0x40000000UL)

/* Stack zone. */
#define STACK_ZONE_START        (0xFFFFC0000000000UL)
#define STACK_ZONE_END          (0xFFFFFFFFFFFFFFFUL)
#define STACK_ZONE_SLOTS        (64*1024)
#define STACK_ZONE_SLOT_SIZE    (0x40000000UL)

/*****************************************************************************
 *                              TYPEDEFS
 ****************************************************************************/

/* Invalid entry descriptor. */
typedef struct port_thread_t
{
  void     *kernelStack;
  void     *userStack;
  uint64_t  isUserMode;
} __attribute__((packed)) port_thread_t;

/*****************************************************************************
 *                           STATIC VARIABLES
 ****************************************************************************/

/* Port-specific thread data. */
static port_thread_t PortThreadList[PORT_THREAD_COUNT];

/*****************************************************************************
 *                         PortThreadAllocate()
 ****************************************************************************/

void PortThreadAllocate (uint64_t threadId)
{
  port_thread_t *portThread = NULL;

  portThread = &PortThreadList[threadId];

  portThread->kernelStack = NULL;
  portThread->userStack   = NULL;
  portThread->isUserMode  = 0;
}

/*****************************************************************************
 *                        PortThreadDeallocate()
 ****************************************************************************/

void PortThreadDeallocate (uint64_t threadId)
{
  port_thread_t *portThread = NULL;

  portThread = &PortThreadList[threadId];

  portThread->kernelStack = NULL;
  portThread->userStack   = NULL;
  portThread->isUserMode  = 0;
}

/*****************************************************************************
 *                          PortThreadRun()
 ****************************************************************************/

void PortThreadRun (uint64_t threadId)
{
  (void) threadId;
}

/*****************************************************************************
 *                          PortThreadStop()
 ****************************************************************************/

void PortThreadStop (uint64_t threadId)
{
  (void) threadId;
}