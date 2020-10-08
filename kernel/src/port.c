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

typedef struct TCR
{
  unsigned int  T0SZ  :6;
  unsigned int  RESV0 :1;
  unsigned int  EPD0  :1;
  unsigned int  IRGN0 :2;
  unsigned int  ORGNO :2;
  unsigned int  SH0   :2;
  unsigned int  TG0   :2;
  unsigned int  T1SZ  :6;
  unsigned int  A1    :1;
  unsigned int  EPD1  :1;
  unsigned int  IRGN1 :2;
  unsigned int  ORGN1 :2;
  unsigned int  SH1   :2;
  unsigned int  TG1   :2;
  unsigned int  IPS   :3;
  unsigned int  RESV1 :1;
  unsigned int  AS    :1;
  unsigned int  TBI0  :1;
  unsigned int  TBI1  :1;
  unsigned int  RESV2 :25;
} __attribute__((packed)) TCR_t;

/*****************************************************************************
 *                           GLOBAL VARIABLES
 ****************************************************************************/

/* Pointer to L1 page table. */
uint64_t *KernelPortPageTable = NULL;

/*****************************************************************************
 *                           STATIC VARIABLES
 ****************************************************************************/

/* Initial value for TCR register. */
static const TCR_t KernelPortTcrValue =
{
  .T0SZ  = 0,
  .RESV0 = 0,
  .EPD0  = EPD_WALK_ON_TLB_MISS,
  .IRGN0 = IRGN_NON_CASHEABLE,
  .ORGNO = ORGN_NON_CASHEABLE,
  .SH0   = SH_NON_SHAREABLE,
  .TG0   = TG_4KB,
  .T1SZ  = 16,
  .A1    = A_TTBR1_DEFINES_ASID,
  .EPD1  = EPD_WALK_ON_TLB_MISS,
  .IRGN1 = IRGN_WB_RA_WA,
  .ORGN1 = ORGN_WB_RA_WA,
  .SH1   = SH_OUTER_SHAREABLE,
  .TG1   = TG_4KB,
  .IPS   = IPS_48_BITS,
  .RESV1 = 0,
  .AS    = AS_ASID_SIZE_16_BITS,
  .TBI0  = TBI_T0P_BYTE_IGNORED,
  .TBI1  = TBI_T0P_BYTE_USED,
  .RESV2 = 0,
};

/*****************************************************************************
 *                       KernelPortMemoryInitialize()
 ****************************************************************************/

void KernelPortMemoryInitialize(void)
{
  /* Local variables. */
  uint64_t sysCtrl         = 0;
  uint64_t tcrInitValue    = 0;

  /* Allocate one page. */
  KernelPortPageTable = (uint64_t *) KernelMemoryPageAllocate();

  /* Update TTBR register with address to L1 page table. */
  MSR(TTBR1_EL1, KernelPortPageTable);
  ISB();

  /* Update TCR register with PIN values. */
  tcrInitValue = *((uint64_t *) &KernelPortTcrValue);
  MSR(TCR_EL1, tcrInitValue);
  ISB();

  /* Enable MMU. */
  MRS(sysCtrl, SCTLR_EL1);
  sysCtrl |= 1;
  MSR(SCTLR_EL1,  sysCtrl);
  ISB();
}
