/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   boot/inc/internal.h
 * @brief  Bootloader internal header file.
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

#ifndef BOOT_INTERNAL_H
#define BOOT_INTERNAL_H

/*****************************************************************************
 *                              INCLUDES
 ****************************************************************************/

/* Bootloader interface header. */
#include "boot/inc/interface.h"

/*****************************************************************************
 *                          GLOBAL VARIABLES
 ****************************************************************************/

extern EFI_HANDLE        BootImageHandle;
extern EFI_SYSTEM_TABLE *BootSystemTable;
extern UINTN             BootMemMapKey;

/*****************************************************************************
 *                          FUNCTION PROTOTYPES
 ****************************************************************************/

void BootInitialize(EFI_HANDLE        ImageHandle,
                    EFI_SYSTEM_TABLE *SystemTable);
void BootPrintSplashMsg(void);
void BootGetMemMap(void);
void BootExitUEFI(void);

/*****************************************************************************
 *                            END OF HEADER
 ****************************************************************************/

#endif /* BOOT_INTERNAL_H */
