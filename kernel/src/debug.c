/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/src/debug.c
 * @brief  ARTOS kernel debug module.
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
 *                           osDebugPrintChr()
 ****************************************************************************/

void KernelDebugPrintChr(char chr)
{
  /* Print character using kernel's internal ARM UART driver. */
  KernelSerialPut(chr);
}

/*****************************************************************************
 *                           osDebugPrintStr()
 ****************************************************************************/

void KernelDebugPrintStr(char *str)
{
  /* Loop over str and print each character. */
  while (*str)
  {
    KernelDebugPrintChr(*str++);
  }
}

/*****************************************************************************
 *                          osDebugPrintDec()
 ****************************************************************************/

void KernelDebugPrintDec(uint64_t dec)
{
  /* Check which range dec is within. */
  if (dec < 10)
  {
    /* Just print dec (base case). */
    KernelDebugPrintChr('0' + dec);
  }
  else
  {
    /* Recursively print the remaining part. */
    KernelDebugPrintDec(dec/10);
    /* Print the first digit. */
    KernelDebugPrintDec(dec%10);
  }
}

/*****************************************************************************
 *                          osDebugPrintHex()
 ****************************************************************************/

void KernelDebugPrintHex(uint64_t hex)
{
  /* Local variables */
  int64_t i;
  /* Print '0x' prefix. */
  KernelDebugPrintChr('0');
  KernelDebugPrintChr('x');
  /* Loop over hex digits. */
  for (i = 60; i >= 0; i -= 4)
  {
    /* Print hex digit. */
    KernelDebugPrintChr("0123456789ABCDEF"[(hex>>i)&0x0F]);
  }
}

/*****************************************************************************
 *                          osDebugPrintFmt()
 ****************************************************************************/

void KernelDebugPrintFmt(char *fmt, ...)
{
  /* Let pArg point to function arguments. */
  uint64_t *pArg = ((uint64_t *) &fmt)+20;

  /* Loop over fmt. */
  while (*fmt)
  {
    /* Check if current character is %. */
    if (*fmt == '%')
    {
      /* Check next character after %. */
      switch (*++fmt) {
        /* [%c] Character. */
        case 'c':
          KernelDebugPrintChr((char) *pArg++);
          break;
        /* [%s] String. */
        case 's':
          KernelDebugPrintStr((char *) *pArg++);
          break;
        /* [%d] Decimal. */
        case 'u':
        case 'd':
          KernelDebugPrintDec((uint64_t) *pArg++);
          break;
        /* [%x] Hexadecimal. */
        case 'p':
        case 'x':
        case 'X':
          KernelDebugPrintHex((uint64_t) *pArg++);
          break;
        /* [%%] Percentage. */
        case '%':
          KernelDebugPrintChr('%');
          break;
        /* [%?] Unknown. */
        default:
          KernelDebugPrintChr('?');
          break;
      }
    }
    else
    {
      /* Print plain character. */
      KernelDebugPrintChr(*fmt);
    }

    /* Next character. */
    fmt++;
  }
}
