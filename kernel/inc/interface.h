/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/inc/interface.h
 * @brief  Kernel interface header file.
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

#ifndef KERNEL_INTERFACE_H
#define KERNEL_INTERFACE_H

/*****************************************************************************
 *                              INCLUDES
 ****************************************************************************/

/* Kernel config header. */
#include "kernel/cfg/config.h"

/*****************************************************************************
 *                              DEFINES
 ****************************************************************************/

/* Memory defs. */
#define PAGE_SIZE             (4096u)

/* Null Pointer */
#if defined(NULL)
#undef  NULL
#endif
#define NULL                  ((void *) -1)

/* Error codes */
#define KERNEL_SUCCESS        (0)
#define KERNEL_ERR_RESOURCE   (-1)
#define KERNEL_ERR_PARAMETER  (-2)

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
 *                             EXTERNS
 ****************************************************************************/

/* RAM start and end variables */
extern uint64_t KernelMemoryRamStart;
extern uint64_t KernelMemoryRamEnd;

/*****************************************************************************
 *                          FUNCTION PROTOTYPES
 ****************************************************************************/

void     KernelDebugPrintChr(char      chr);
void     KernelDebugPrintStr(char     *str);
void     KernelDebugPrintDec(uint64_t  dec);
void     KernelDebugPrintHex(uint64_t  hex);
void     KernelDebugPrintFmt(char     *fmt, ...);

void     KernelMemoryInitialize(void);
void    *KernelMemoryPageAllocate(void);
void     KernelMemoryPageDeallocate(void *pageBaseAddr);

void     KernelCoreInitialize(void);
void     KernelCoreStart(void);

void     KernelPowerOff(void);

/*****************************************************************************
 *                            END OF HEADER
 ****************************************************************************/

#endif /* KERNEL_INTERFACE_H */
