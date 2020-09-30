/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   boot/src/exit.c
 * @brief  Bootloader UEFI termination code.
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
#include "boot/inc/interface.h"
#include "boot/inc/internal.h"

/*****************************************************************************
 *                             bootExitUEFI()
 ****************************************************************************/

void BootExitUEFI(void)
{
  /* Local variables. */
  EFI_STATUS             result;

  /* Simply call ExitBootServices to exit UEFI. */
  result = (EFI_STATUS) uefi_call_wrapper(
             (void *)BootSystemTable->BootServices->ExitBootServices,
             2,
             BootImageHandle,
             BootMemMapKey);

  /* Make sure ExitBootServices is done successfully. */
  if (result != EFI_SUCCESS)
  {
    Print(L"BOOTLOADER: Failed to exit UEFI boot services.\n");
    while(1);
  }
}
