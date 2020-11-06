/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/src/print.c
 * @brief  ARTOS kernel print (stdout) module.
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
 *                       KernelPrintInitialize()
 ****************************************************************************/

void KernelPrintInitialize(void)
{
  /* Do nothing. */
}

/*****************************************************************************
 *                          KernelPrintChr()
 ****************************************************************************/

void KernelPrintChr(char chr)
{
  /* Print character using CPU-specific UART driver. */
  KernelPortSerialPut(chr);
}

/*****************************************************************************
 *                          KernelPrintStr()
 ****************************************************************************/

void KernelPrintStr(char *str)
{
  /* Loop over str and print each character. */
  while (*str)
  {
    KernelPrintChr(*str++);
  }
}

/*****************************************************************************
 *                          KernelPrintDec()
 ****************************************************************************/

void KernelPrintDec(uint64_t dec)
{
  /* Check which range dec is within. */
  if (dec < 10)
  {
    /* Just print dec (base case). */
    KernelPrintChr('0' + dec);
  }
  else
  {
    /* Recursively print the remaining part. */
    KernelPrintDec(dec/10);

    /* Print the first digit. */
    KernelPrintDec(dec%10);
  }
}

/*****************************************************************************
 *                          KernelPrintHex()
 ****************************************************************************/

void KernelPrintHex(uint64_t hex)
{
  /* Loop counter. */
  int64_t i;

  /* Print '0x' prefix. */
  KernelPrintChr('0');
  KernelPrintChr('x');

  /* Loop over hex digits. */
  for (i = 60; i >= 0; i -= 4)
  {
    /* Print hex digit. */
    KernelPrintChr("0123456789ABCDEF"[(hex>>i)&0x0F]);
  }
}

/*****************************************************************************
 *                          KernelPrintFmt()
 ****************************************************************************/

void KernelPrintFmt(char *fmt, ...)
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
          KernelPrintChr((char) *pArg++);
          break;
        /* [%s] String. */
        case 's':
          KernelPrintStr((char *) *pArg++);
          break;
        /* [%d] Decimal. */
        case 'u':
        case 'd':
          KernelPrintDec((uint64_t) *pArg++);
          break;
        /* [%x] Hexadecimal. */
        case 'p':
        case 'x':
        case 'X':
          KernelPrintHex((uint64_t) *pArg++);
          break;
        /* [%%] Percentage. */
        case '%':
          KernelPrintChr('%');
          break;
        /* [%?] Unknown. */
        default:
          KernelPrintChr('?');
          break;
      }
    }
    else
    {
      /* Print plain character. */
      KernelPrintChr(*fmt);
    }

    /* Next character. */
    fmt++;
  }
}
