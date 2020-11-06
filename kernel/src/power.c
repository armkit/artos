/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/src/power.c
 * @brief  ARTOS kernel power module.
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
 *                        KernelPowerInitialize()
 ****************************************************************************/

void KernelPowerInitialize(void)
{
  /* Do nothing. */
}

/*****************************************************************************
 *                           KernelPowerOff()
 ****************************************************************************/

void KernelPowerOff(void)
{
  /* Power State Coordination Interface: SYSTEM SHUTDOWN */
  __asm__("LDR W0, =0x84000008");
  __asm__("HVC #0");
}
