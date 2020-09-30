/***************************************************************************
 *
 *                   ARTOS Operating System.
 *                 Copyright (C) 2020  ARMKit.
 *
 ***************************************************************************
 * @file   boot/src/memmap.c
 * @brief  Bootloader UEFI Memory Map handling code.
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

/* UEFI includes. */
#include "efi.h"
#include "efilib.h"

/* Bootloader includes. */
#include "boot/inc/interface.h"
#include "boot/inc/internal.h"

/* Other modules. */
#include "kernel/inc/interface.h"

/*****************************************************************************
 *                           GLOBAL VARIABLES
 ****************************************************************************/

/* Memory map key (used by ExitBootServices). */
UINTN BootMemMapKey;

/*****************************************************************************
 *                           STATIC VARIABLES
 ****************************************************************************/

/* List of EFI memory types (for printing). */
static const UINT16 *BootEFIMemTypes[] = {
  L"RESERVED",  /* EfiReservedMemoryType      */
  L"LOADCODE",  /* EfiLoaderCode              */
  L"LOADDATA",  /* EfiLoaderData              */
  L"BOOTCODE",  /* EfiBootServicesCode        */
  L"BOOTDATA",  /* EfiBootServicesData        */
  L"RUNTCODE",  /* EfiRuntimeServicesCode     */
  L"RUNTDATA",  /* EfiRuntimeServicesData     */
  L"RAMSPACE",  /* EfiConventionalMemory      */
  L"UNUSABLE",  /* EfiUnusableMemory          */
  L"ACPIRECL",  /* EfiACPIReclaimMemory       */
  L"ACPIMEMR",  /* EfiACPIMemoryNVS           */
  L"MEMMAPIO",  /* EfiMemoryMappedIO          */
  L"MEMMAPPS",  /* EfiMemoryMappedIOPortSpace */
  L"PALSPACE",  /* EfiPalCode                 */
  L"FLASHMEM",  /* EfiPersistentMemory        */
};

/*****************************************************************************
 *                            bootGetMemMap()
 ****************************************************************************/

void BootGetMemMap(void)
{
  /* Local variables. */
  EFI_STATUS             result             = EFI_SUCCESS;
  UINTN                  mapSize            = 0;
  EFI_MEMORY_DESCRIPTOR *memoryMap          = NULL;
  UINTN                  mapKey             = 0;
  UINTN                  descriptorSize     = 0;
  UINT32                 descriptorVersion  = 0;
  UINTN                  ramBase            = 0;
  UINTN                  ramPages           = 0;
  UINT32                 i                  = 0;

  /* Call GetMemoryMap with NULL to get the size of the map. */
  result = (EFI_STATUS) uefi_call_wrapper(
             (void *)BootSystemTable->BootServices->GetMemoryMap,
             5,
             &mapSize,
             memoryMap,
             NULL,
             &descriptorSize,
             NULL);

  /* Make sure GetMemoryMap returned EFI_BUFFER_TOO_SMALL. */
  if (result != EFI_BUFFER_TOO_SMALL)
  {
    Print(L"BOOTLOADER: Failed to retrieve memory map info from UEFI.\n");
    while(1);
  }

  /* Compensate the effect of the AllocatePool() call below. */
  mapSize += 2 * descriptorSize;

  /* Allocate pool for a new memory map */
  result = (EFI_STATUS) uefi_call_wrapper(
             (void *)BootSystemTable->BootServices->AllocatePool,
             3,
             EfiLoaderData,
             mapSize,
             (void **)&memoryMap);

  /* Make sure AllocatePool returned EFI_SUCCESS. */
  if (result != EFI_SUCCESS)
  {
    Print(L"BOOTLOADER: Failed to allocate pool for UEFI memory map.\n");
    while(1);
  }

  /* Retrieve full memory map. */
  result = (EFI_STATUS) uefi_call_wrapper(
             (void *)BootSystemTable->BootServices->GetMemoryMap,
             5,
             &mapSize,
             memoryMap,
             &mapKey,
             &descriptorSize,
             &descriptorVersion);

  /* Make sure GetMemoryMap returned EFI_SUCCESS. */
  if (result != EFI_SUCCESS)
  {
    Print(L"BOOTLOADER: Failed to retieve UEFI memory map.\n");
    while(1);
  }

  /* Print good-looking header for the table. */
  Print(L"-----------------------------------------------------------\n");
  Print(L"                SYSTEM MEMORY MAP TABLE                    \n");
  Print(L"-----------------------------------------------------------\n");
  Print(L"     TYPE          START          END            ATTR      \n");
  Print(L"-----------------------------------------------------------\n");

  /* Loop over memory map entries. */
  for(i = 0; i < (mapSize/descriptorSize); i++)
  {
    /* Print leading spaces at the beginning of the line. */
    Print(L"   ");

    /* Print entry type. */
    Print(L"%s", BootEFIMemTypes[memoryMap->Type]);
    Print(L"     ");

    /* Print entry start address. */
    Print(L"0x%X", memoryMap->PhysicalStart);
    Print(L"     ");

    /* Print entry end address. */
    Print(L"0x%X", memoryMap->PhysicalStart +
                   memoryMap->NumberOfPages*4096 - 1);
    Print(L"     ");

    /* Print attributes. */
    Print(L"0x%X", memoryMap->Attribute);
    Print(L"   ");

    /* Newline terminator. */
    Print(L"\n");

    /* RAM space? */
    if (memoryMap->Type == EfiConventionalMemory)
    {
      /* Choose the biggest continuous ram region. */
      if (memoryMap->NumberOfPages > ramPages) {
        ramBase  = memoryMap->PhysicalStart;
        ramPages = memoryMap->NumberOfPages;
      }
    }

    /* Get next entry in the table. */
    memoryMap = (EFI_MEMORY_DESCRIPTOR*)(((char*)memoryMap)+descriptorSize);
  }

  /* Print table footer. */
  Print(L"-----------------------------------------------------------\n");

  /* Calculate ram information. */
  KernelMemoryRamStart = ramBase;
  KernelMemoryRamEnd   = ramBase + ramPages*4096;

  /* Print ram information. */
  Print(L"   RAM START: 0x%X\n", KernelMemoryRamStart);
  Print(L"   RAM END:   0x%X\n", KernelMemoryRamEnd);
  Print(L"   RAM SIZE:  %dMB\n", ramPages*4/1024);

  /* Print another table footer. */
  Print(L"-----------------------------------------------------------\n");

  /* Print newline to keep space. */
  Print(L"\n");

  /* Spawn an error if RAM couldn't be detected. */

  /* Make sure GetMemoryMap returned EFI_BUFFER_TOO_SMALL. */
  if (ramPages == 0)
  {
    Print(L"BOOTLOADER: Failed to detect RAM information from EFI.\n");
    while(1);
  }

  /* Store memory map key in data section. */
  BootMemMapKey = mapKey;
}
