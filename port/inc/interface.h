/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   port/inc/interface.h
 * @brief  Port interface header file.
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

#ifndef PORT_INTERFACE_H
#define PORT_INTERFACE_H

/*****************************************************************************
 *                              INCLUDES
 ****************************************************************************/

/* Port config header. */
#include "port/cfg/config.h"

/*****************************************************************************
 *                              DEFINES
 ****************************************************************************/

/* Null Pointer */
#if defined(NULL)
#undef  NULL
#endif
#define NULL                  ((void *) -1)

/* Address translation unit size. */
#define PAGE_SIZE             (4096u)

/* Process/thread count. */
#define PORT_PROCESS_COUNT    (0x10000U)
#define PORT_THREAD_COUNT     (0x10000U)

/*****************************************************************************
 *                              TYPEDEFS
 ****************************************************************************/

/* Signed types. */
typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long        int64_t;

/* Unsigned types. */
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long      uint64_t;

/* Error type. */
typedef int64_t            error_t;

/*****************************************************************************
 *                          FUNCTION PROTOTYPES
 ****************************************************************************/

/* CPU-Specific Serial I/O. */
void PortSerialInitialize (void);
void PortSerialPut        (char c);
char PortSerialGet        (void);

/* CPU-Specific Address Translation. */
void  PortTranslationInitialize (void);
void *PortTranslationSet        (void *virtualAddr, void *physicalAddr);
void *PortTranslationGet        (void *virtualAddr);
void *PortTranslationDel        (void *virtualAddr);

/* CPU-Specific Thread Routines. */
void PortThreadAllocate   (uint64_t threadId);
void PortThreadDeallocate (uint64_t threadId);
void PortThreadRun        (uint64_t threadId);
void PortThreadStop       (uint64_t threadId);

/*****************************************************************************
 *                            END OF HEADER
 ****************************************************************************/

#endif /* PORT_INTERFACE_H */
