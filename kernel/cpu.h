/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/cpu.h
 * @brief  ARM-specific defines.
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

#ifndef CPU_H
#define CPU_H

/*****************************************************************************
 *                              DEFINES
 ****************************************************************************/

/* PL011 registers */
#define UARTDR            (*((volatile unsigned short *) 0x09000000))
#define UARTRSR           (*((volatile unsigned short *) 0x09000004))
#define UARTECR           (*((volatile unsigned short *) 0x09000004))
#define UARTFR            (*((volatile unsigned short *) 0x09000018))
#define UARTILPR          (*((volatile unsigned short *) 0x09000020))
#define UARTIBRD          (*((volatile unsigned short *) 0x09000024))
#define UARTFBRD          (*((volatile unsigned short *) 0x09000028))
#define UARTFLCR_H        (*((volatile unsigned short *) 0x0900002C))
#define UARTCR            (*((volatile unsigned short *) 0x09000030))

/* UARTFR flags */
#define CTS               (0x0001)
#define DSR               (0x0002)
#define DCD               (0x0004)
#define BUSY              (0x0008)
#define RXFE              (0x0010)
#define TXFF              (0x0020)
#define RXFF              (0x0040)
#define TXFE              (0x0080)
#define RI                (0x0100)

/* UARTCR flags */
#define UARTEN            (0x0001)
#define SIREN             (0x0002)
#define SIRLP             (0x0004)
#define LBE               (0x0080)
#define TXE               (0x0100)
#define RXE               (0x0200)
#define DTR               (0x0400)
#define RTS               (0x0800)
#define Out1              (0x1000)
#define Out2              (0x2000)
#define RTSEn             (0x4000)
#define CTSEn             (0x8000)

/*****************************************************************************
 *                         INLINE FUNCTIONS
 ****************************************************************************/

inline static void osCpuSerialOut(char c)
{
  /* Wait until there is space in PL011 UART TX buffer. */
  while (UARTFR & TXFF);

  /* Put 'c' parameter in the TX buffer. */
  UARTDR = c;

  /* Wait until 'c' is dispatched. */
  while (!(UARTFR & TXFE));
}

/*****************************************************************************
 *                            END OF HEADER
 ****************************************************************************/

#endif /* CPU_H */
