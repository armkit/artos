/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   kernel/src/port.c
 * @brief  ARTOS kernel port module.
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
 *                           ASSEMNBLY MACROS
 ****************************************************************************/

#define MSR(sys_reg, var) __asm__("MSR " #sys_reg " , %0"::"r"(var))
#define MRS(var, sys_reg) __asm__("MRS %0, " #sys_reg : "=r"(var));
#define ISB()             __asm__("ISB")

/*****************************************************************************
 *                              TCR MACROS
 ****************************************************************************/

/* TCR.EPD field specification. */
#define EPD_WALK_ON_TLB_MISS    0
#define EPD_EXCP_ON_TLB_MISS    1

/* TCR.IRGN field specification. */
#define IRGN_NON_CASHEABLE      0
#define IRGN_WB_RA_WA           1
#define IRGN_WT_RA_NWA          2
#define IRGN_WB_RA_NWA          1

/* TCR.ORGN field specification. */
#define ORGN_NON_CASHEABLE      0
#define ORGN_WB_RA_WA           1
#define ORGN_WT_RA_NWA          2
#define ORGN_WB_RA_NWA          1

/* TCR.SH field specification. */
#define SH_NON_SHAREABLE        0
#define SH_OUTER_SHAREABLE      2
#define SH_INNER_SHAREABLE      3

/* TCR.TG field specification. */
#define TG_4KB                  0
#define TG_64KB                 1
#define TG_16KB                 2

/* TCR.A field specification. */
#define A_TTBR0_DEFINES_ASID    0
#define A_TTBR1_DEFINES_ASID    1

/* TCR.IPS field specification. */
#define IPS_32_BITS             0
#define IPS_36_BITS             1
#define IPS_40_BITS             2
#define IPS_42_BITS             3
#define IPS_44_BITS             4
#define IPS_48_BITS             5
#define IPS_52_BITS             6

/* TCR.AS field specification. */
#define AS_ASID_SIZE_8_BITS     0
#define AS_ASID_SIZE_16_BITS    1

/* TCR.TBI field specification. */
#define TBI_T0P_BYTE_USED       0
#define TBI_T0P_BYTE_IGNORED    1

/*****************************************************************************
 *                              TYPEDEFS
 ****************************************************************************/

/* Invalid entry descriptor. */
typedef struct INVENTRY
{
  unsigned long VALID          :1;
  unsigned long IGNORED        :63;
} __attribute__((packed)) INVENTRY_t;

/* Block entry descriptor (1GB). */
typedef struct BLKENTRY
{
  unsigned long VALID          :1;
  unsigned long TYPE           :1;
  unsigned long ATTRIDX        :3;
  unsigned long NS             :1;
  unsigned long AP             :2;
  unsigned long SH             :2;
  unsigned long AF             :1;
  unsigned long NG             :1;
  unsigned long RESV0          :18;
  unsigned long ADDR           :18;
  unsigned long RESV1          :4;
  unsigned long CONT           :1;
  unsigned long PXN            :1;
  unsigned long XN             :1;
  unsigned long IGNORED        :9;
} __attribute__((packed)) BLKENTRY_t;

/* Table entry descriptor. */
typedef struct TBLENTRY
{
  unsigned long VALID          :1;
  unsigned long TYPE           :1;
  unsigned long IGNORED0       :10;
  unsigned long ADDR           :36;
  unsigned long RESV           :4;
  unsigned long IGNORED1       :7;
  unsigned long PXN            :1;
  unsigned long UXN            :1;
  unsigned long AP             :2;
  unsigned long NS             :1; 
} __attribute__((packed)) TBLENTRY_t;

/* Page entry descriptor. */
typedef struct PAGENTRY
{
  unsigned long VALID          :1;
  unsigned long TYPE           :1;
  unsigned long ATTRIDX        :3;
  unsigned long NS             :1;
  unsigned long AP             :2;
  unsigned long SH             :2;
  unsigned long AF             :1;
  unsigned long NG             :1;
  unsigned long ADDR           :36;
  unsigned long RESV0          :4;
  unsigned long CONT           :1;
  unsigned long PXN            :1;
  unsigned long UXN            :1;
  unsigned long IGNORED        :9;
} __attribute__((packed)) PAGENTRY_t;

/* TTBR0 and TTBR1 register format. */
typedef struct TTBR 
{
  unsigned long RESV           :1;
  unsigned long BASE_ADDRESS   :47;
  unsigned long ASID           :16;
} __attribute__((packed)) TTBR_t;

/* TCR register format. */
typedef struct TCR
{
  unsigned long T0SZ           :6;
  unsigned long RESV0          :1;
  unsigned long EPD0           :1;
  unsigned long IRGN0          :2;
  unsigned long ORGNO          :2;
  unsigned long SH0            :2;
  unsigned long TG0            :2;
  unsigned long T1SZ           :6;
  unsigned long A1             :1;
  unsigned long EPD1           :1;
  unsigned long IRGN1          :2;
  unsigned long ORGN1          :2;
  unsigned long SH1            :2;
  unsigned long TG1            :2;
  unsigned long IPS            :3;
  unsigned long RESV1          :1;
  unsigned long AS             :1;
  unsigned long TBI0           :1;
  unsigned long TBI1           :1;
  unsigned long RESV2          :25;
} __attribute__((packed)) TCR_t;

/* SCTLR register format. */
typedef struct SCTLR
{
  unsigned long MMU            :1;
  unsigned long RESV           :63;
} __attribute__((packed)) SCTLR_t;

/*****************************************************************************
 *                       KernelPortMemoryInitialize()
 ****************************************************************************/

void KernelPortMemoryInitialize(void)
{
  /* Registers as integers. */
  uint64_t ttbr0Value   = 0;
  uint64_t ttbr1Value   = 0;
  uint64_t tcrValue     = 0;
  uint64_t sctlrValue   = 0;
  
  /* Registers as structs. */
  TTBR_t  *ttbr0Ptr     = NULL;
  TTBR_t  *ttbr1Ptr     = NULL;
  TCR_t   *tcrPtr       = NULL;
  SCTLR_t *sctlrPtr     = NULL;
  
  /* Setup pointers. */
  ttbr0Ptr = (TTBR_t  *) &ttbr0Value;
  ttbr1Ptr = (TTBR_t  *) &ttbr1Value;
  tcrPtr   = (TCR_t   *) &tcrValue;
  sctlrPtr = (SCTLR_t *) &sctlrValue;
  
  /* Setup TTBR0_EL1 register. */
  MRS(ttbr0Value, TTBR0_EL1);
  KernelDebugPrintFmt("TTBR0_EL1:  %x", ttbr0Value);
  (void) ttbr0Ptr;
  KernelDebugPrintFmt(" -> %x\n", ttbr0Value);
  MSR(TTBR0_EL1, ttbr0Value);
  ISB();

  /* Setup TTBR1_EL1 register. */
  MRS(ttbr1Value, TTBR1_EL1);
  KernelDebugPrintFmt("TTBR1_EL1:  %x", ttbr1Value);
  (void) ttbr1Ptr;
  KernelDebugPrintFmt(" -> %x\n", ttbr1Value);
  MSR(TTBR1_EL1, ttbr1Value);
  ISB();

  /* Setup TCR_EL1 register. */
  MRS(tcrValue, TCR_EL1);
  KernelDebugPrintFmt("TCR_EL1:    %x", tcrValue);
  tcrPtr->T1SZ  = 16,
  tcrPtr->A1    = A_TTBR1_DEFINES_ASID,
  tcrPtr->EPD1  = EPD_WALK_ON_TLB_MISS,
  tcrPtr->IRGN1 = IRGN_WB_RA_WA,
  tcrPtr->ORGN1 = ORGN_WB_RA_WA,
  tcrPtr->SH1   = SH_INNER_SHAREABLE,
  tcrPtr->TG1   = TG_4KB,
  tcrPtr->IPS   = IPS_48_BITS,
  tcrPtr->RESV1 = 0,
  tcrPtr->AS    = AS_ASID_SIZE_16_BITS,
  tcrPtr->TBI0  = TBI_T0P_BYTE_IGNORED,
  tcrPtr->TBI1  = TBI_T0P_BYTE_USED,
  tcrPtr->RESV2 = 0,
  KernelDebugPrintFmt(" -> %x\n", tcrValue);
  MSR(TCR_EL1, tcrValue);
  ISB();

  /* Setup SCTlR_EL1 register. */
  MRS(sctlrValue, SCTLR_EL1);
  KernelDebugPrintFmt("SCTLR_EL1:  %x", sctlrValue);
  sctlrPtr->MMU = 1;
  KernelDebugPrintFmt(" -> %x\n", sctlrValue);
  MSR(SCTLR_EL1, sctlrValue);
  ISB();
}
