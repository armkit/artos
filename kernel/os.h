/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/os.h
 * @brief  Kernel header file.
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

#ifndef OS_H
#define OS_H

/*****************************************************************************
 *                              INCLUDES
 ****************************************************************************/

/* Kernel header files. */
#include "kernel/types.h"

/*****************************************************************************
 *                          FUNCTION PROTOTYPES
 ****************************************************************************/

void osDebugPrintChr(char      chr);
void osDebugPrintStr(char     *str);
void osDebugPrintDec(uint64_t  dec);
void osDebugPrintHex(uint64_t  hex);
void osDebugPrintFmt(char     *fmt, ...);

void osKernelInitialize(void);
void osKernelStart(void);

void osPowerOff(void);

/*****************************************************************************
 *                            END OF HEADER
 ****************************************************************************/

#endif /* OS_H */
