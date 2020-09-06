/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   boot/splash.c
 * @brief  Bootloader splash printing code.
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
 *                           STATIC VARIABLES
 ****************************************************************************/

/* Splash message to print out. */
static const UINT16 *bootSplashMsg =
  L"\n"
  L"          ****************************************\n"
  L"          *        ARTOS Operating System        *\n"
  L"          *            Version 21.04             *\n"
  L"          ****************************************\n"
  L"          *      Copyright (C) 2020  ArmKit      *\n"
  L"          ****************************************\n"
  L"\n"
  L"Welcome to ARTOS Operating System, a 64-bit operating system\n"
  L"for armv8 architecture (aarch64).\n"
  L"\n"
  L"This program is free software; you can redistribute it and/or\n"
  L"modify it under the terms of the GNU General Public License\n"
  L"as published by the Free Software Foundation; either version 2\n"
  L"of the License, or (at your option) any later version.\n"
  L"\n"
  L"This program is distributed in the hope that it will be useful,\n"
  L"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
  L"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
  L"GNU General Public License for more details.\n"
  L"\n"
  L"You should have received a copy of the GNU General Public License\n"
  L"along with this program; if not, write to the Free Software\n"
  L"Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA\n"
  L"02110-1301, USA.\n"
  L"\n";

/*****************************************************************************
 *                         bootPrintSplashMsg()
 ****************************************************************************/

void bootPrintSplashMsg(void)
{
  /* Print intro message. */
  Print(bootSplashMsg);
}
