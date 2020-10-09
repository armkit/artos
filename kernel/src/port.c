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
 *                              ASSEMNBLY MACROS
 ****************************************************************************/

#define MSR(sys_reg, var) __asm__("MSR " #sys_reg " , %0"::"r"(var))
#define MRS(var, sys_reg) __asm__("MRS %0, " #sys_reg : "=r"(var));
#define ISB()             __asm__("ISB")

/*****************************************************************************
 *                              TCR DEFINES
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

typedef struct TTBR 
{
  unsigned int  RESV           :1;
  unsigned long BASE_ADDRESS   :47;
  unsigned int  ASID           :16;
} __attribute__((packed)) TTBR_t;

typedef struct TCR
{
  unsigned int  T0SZ           :6;
  unsigned int  RESV0          :1;
  unsigned int  EPD0           :1;
  unsigned int  IRGN0          :2;
  unsigned int  ORGNO          :2;
  unsigned int  SH0            :2;
  unsigned int  TG0            :2;
  unsigned int  T1SZ           :6;
  unsigned int  A1             :1;
  unsigned int  EPD1           :1;
  unsigned int  IRGN1          :2;
  unsigned int  ORGN1          :2;
  unsigned int  SH1            :2;
  unsigned int  TG1            :2;
  unsigned int  IPS            :3;
  unsigned int  RESV1          :1;
  unsigned int  AS             :1;
  unsigned int  TBI0           :1;
  unsigned int  TBI1           :1;
  unsigned int  RESV2          :25;
} __attribute__((packed)) TCR_t;

typedef struct SCTLR
{
  unsigned int  MMU            :1;
  unsigned long RESV           :63;
} __attribute__((packed)) SCTLR_t;

/*****************************************************************************
 *                       KernelPortMemoryInitialize()
 ****************************************************************************/

void KernelPortMemoryInitialize(void)
{
  /* Local variables. */
  //uint64_t ttbr0Value   = 0;
  uint64_t ttbr1Value   = 0;
  uint64_t tcrValue     = 0;
  uint64_t sctlrValue   = 0;
  //TTBR_t *ttbr0Ptr      = NULL;
  TTBR_t *ttbr1Ptr      = NULL;
  TCR_t *tcrPtr         = NULL;
  SCTLR_t *sctlrPtr     = NULL;

  /* setup TTBR1_EL1 register. */
  KernelDebugPrintFmt("TTBR1_EL1:  ");
  ttbr1Ptr = (TTBR_t *) &ttbr1Value;
  MRS(ttbr1Value, TTBR1_EL1);
  KernelDebugPrintFmt("%x  ", ttbr1Value);
  ttbr1Ptr->BASE_ADDRESS = (long )KernelMemoryPageAllocate();
  KernelDebugPrintFmt("%x  ", ttbr1Value);
  MSR(TTBR1_EL1, ttbr1Value);
  ISB();
  KernelDebugPrintFmt("\n");

  /* Setup TCR_EL1 register. */
  KernelDebugPrintFmt("TCR_EL1:    ");
  tcrPtr = (TCR_t *) &tcrValue;
  MRS(tcrValue, TCR_EL1);
  KernelDebugPrintFmt("%x  ", tcrValue);
  tcrPtr->T1SZ  = 16,
  tcrPtr->A1    = A_TTBR1_DEFINES_ASID,
  tcrPtr->EPD1  = EPD_WALK_ON_TLB_MISS,
  tcrPtr->IRGN1 = IRGN_WB_RA_WA,
  tcrPtr->ORGN1 = ORGN_WB_RA_WA,
  tcrPtr->SH1   = SH_OUTER_SHAREABLE,
  tcrPtr->TG1   = TG_4KB,
  tcrPtr->IPS   = IPS_48_BITS,
  tcrPtr->RESV1 = 0,
  tcrPtr->AS    = AS_ASID_SIZE_16_BITS,
  tcrPtr->TBI0  = TBI_T0P_BYTE_IGNORED,
  tcrPtr->TBI1  = TBI_T0P_BYTE_USED,
  tcrPtr->RESV2 = 0,
  KernelDebugPrintFmt("%x  ", tcrValue);
  MSR(TCR_EL1, tcrValue);
  ISB();
  KernelDebugPrintFmt("\n");

  /* Setup SCTlR_EL1 register. */
  KernelDebugPrintFmt("SCTLR_EL1:  ");
  sctlrPtr = (SCTLR_t *) &sctlrValue;
  MRS(sctlrValue, SCTLR_EL1);
  KernelDebugPrintFmt("%x  ", sctlrValue);
  sctlrPtr->MMU = 1;
  KernelDebugPrintFmt("%x  ", sctlrValue);
  MSR(SCTLR_EL1, sctlrValue);
  ISB();
}
