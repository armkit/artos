/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/src/core.c
 * @brief  ARTOS kernel core module.
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

/* Kernel includes. */
#include "kernel/inc/interface.h"
#include "kernel/inc/internal.h"

/*****************************************************************************
 *                       KernelCoreInitialize()
 ****************************************************************************/

void KernelCoreInitialize(void)
{
  /* Initialize CPU-specific port. */
  KernelPortSerialInitialize();
  KernelPortTranslationInitialize();
  KernelPortProcessInitialize();
  KernelPortThreadInitialize();

  /* Initialize kernel components. */
  KernelPrintInitialize();
  KernelMemoryInitialize();
  KernelThreadInitialize();
  KernelPowerInitialize();
}

/*****************************************************************************
 *                         KernelCoreStart()
 ****************************************************************************/

void KernelCoreStart(void)
{
  /* Just shutdown for now. */
  KernelPowerOff();
}
