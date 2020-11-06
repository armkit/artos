/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/cfg/config.h
 * @brief  Kernel config header file.
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

#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

/*****************************************************************************
 *                              DEFINES
 ****************************************************************************/

/* RAM start address (if not overwritten by bootloader). */
#define KERNEL_CONFIG_RAM_START           0

/* RAM end address (if not overwritten by bootloader). */
#define KERNEL_CONFIG_RAM_END             0

/* Maximum number of CPUs to support */
#define KERNEL_CONFIG_MAX_CPU_COUNT       16

/* Maximum prioirty (e.g. 64 means 1..63 are valid priorities). */
#define KERNEL_CONFIG_MAX_PRIOIRTY        64

/* Thread/process name maximum size. */
#define KERNEL_CONFIG_NAME_MAX_SIZE       32

/* Stack default size. */
#define KERNEL_CONFIG_DEFAULT_STACK_SIZE  0x2000

/*****************************************************************************
 *                            END OF HEADER
 ****************************************************************************/

#endif /* KERNEL_CONFIG_H */
