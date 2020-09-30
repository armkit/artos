/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   boot/src/main.c
 * @brief  Bootloader main file.
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

/* UEFI includes (for efi_main prototype). */
#include "efi.h"

/* Boot loader includes. */
#include "boot/inc/interface.h"
#include "boot/inc/internal.h"

/* Other modules. */
#include "kernel/inc/interface.h"

/*****************************************************************************
 *                              efi_main()
 ****************************************************************************/

EFI_STATUS EFIAPI efi_main(EFI_HANDLE        ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable)
{
  /* Initialize the boot loader. */
  BootInitialize(ImageHandle, SystemTable);

  /* Initialize ARTOS kernel. */
  KernelCoreInitialize();

  /* Create first thread to be executed by the kernel. */
  /* KernelThreadNew(someThread, NULL, NULL); */

  /* Start the kernel. */
  KernelCoreStart();

  /* Should never reach this line. */
  return EFI_SUCCESS;
}
