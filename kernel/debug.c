/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/debug.c
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
#include "kernel/os.h"
#include "kernel/os_priv.h"

/*****************************************************************************
 *                           osDebugPrintChr()
 ****************************************************************************/

void osDebugPrintChr(char chr)
{
  /* Print character using kernel's internal ARM UART driver. */
  osCpuSerialOut(chr);
}

/*****************************************************************************
 *                           osDebugPrintStr()
 ****************************************************************************/

void osDebugPrintStr(char *str)
{
  /* Loop over str and print each character. */
  while (*str)
  {
    osDebugPrintChr(*str++);
  }
}

/*****************************************************************************
 *                          osDebugPrintDec()
 ****************************************************************************/

void osDebugPrintDec(uint64_t dec)
{
  /* Check which range dec is within. */
  if (dec < 10)
  {
    /* Just print dec (base case). */
    osDebugPrintChr('0' + dec);
  }
  else
  {
    /* Recursively print the remaining part. */
    osDebugPrintDec(dec/10);
    /* Print the first digit. */
    osDebugPrintDec(dec%10);
  }
}

/*****************************************************************************
 *                          osDebugPrintHex()
 ****************************************************************************/

void osDebugPrintHex(uint64_t hex)
{
  /* Local variables */
  int64_t i;
  /* Print '0x' prefix. */
  osDebugPrintChr('0');
  osDebugPrintChr('x');
  /* Loop over hex digits. */
  for (i = 60; i >= 0; i -= 4)
  {
    /* Print hex digit. */
    osDebugPrintChr("0123456789ABCDEF"[(hex>>i)&0x0F]);
  }
}

/*****************************************************************************
 *                          osDebugPrintFmt()
 ****************************************************************************/

void osDebugPrintFmt(char *fmt, ...)
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
          osDebugPrintChr((char) *pArg++);
          break;
        /* [%s] String. */
        case 's':
          osDebugPrintStr((char *) *pArg++);
          break;
        /* [%d] Decimal. */
        case 'u':
        case 'd':
          osDebugPrintDec((uint64_t) *pArg++);
          break;
        /* [%x] Hexadecimal. */
        case 'p':
        case 'x':
        case 'X':
          osDebugPrintHex((uint64_t) *pArg++);
          break;
        /* [%%] Percentage. */
        case '%':
          osDebugPrintChr('%');
          break;
        /* [%?] Unknown. */
        default:
          osDebugPrintChr('?');
          break;
      }
    }
    else
    {
      /* Print plain character. */
      osDebugPrintChr(*fmt);
    }

    /* Next character. */
    fmt++;
  }
}
