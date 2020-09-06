/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   boot/init.c
 * @brief  Bootloader UEFI initialization code.
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

/* UEFI includes. */
#include "efi.h"
#include "efilib.h"

/* Bootloader includes. */
#include "boot/boot.h"
#include "boot/boot_priv.h"

/*****************************************************************************
 *                            GLOBAL VARIABLES
 ****************************************************************************/

/* UEFI boot data */
EFI_HANDLE bootImageHandle;
EFI_SYSTEM_TABLE *bootSystemTable;

/*****************************************************************************
 *                            bootInitialize()
 ****************************************************************************/

void bootInitialize(EFI_HANDLE        ImageHandle,
                    EFI_SYSTEM_TABLE *SystemTable)
{
  /* Store UEFI ImageHandle in data section. */
  bootImageHandle = ImageHandle;

  /* Store UEFI SystemTable in data section. */
  bootSystemTable = SystemTable;

  /* Initialize EFI library. */
  InitializeLib(ImageHandle, SystemTable);

  /* Print intro message. */
  bootPrintSplashMsg();

  /* Get memory map. */
  bootGetMemMap();

  /* Exit boot services. */
  bootExitUEFI();
}
