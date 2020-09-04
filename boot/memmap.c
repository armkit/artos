
/* UEFI includes. */
#include "efi.h"
#include "efilib.h"

/* Bootloader includes. */
#include "boot/boot.h"
#include "boot/boot_priv.h"

/* Memory map key (used by ExitBootServices). */
UINTN bootMemMapKey;

/* List of EFI memory types (for printing). */
static const unsigned short *bootEFIMemTypes[] = {
  L"RESV ",  /* EfiReservedMemoryType      */
  L"LOADC",  /* EfiLoaderCode              */
  L"LOADD",  /* EfiLoaderData              */
  L"BOOTC",  /* EfiBootServicesCode        */
  L"BOOTD",  /* EfiBootServicesData        */
  L"RUNC ",  /* EfiRuntimeServicesCode     */
  L"RUND ",  /* EfiRuntimeServicesData     */
  L"CONV ",  /* EfiConventionalMemory      */
  L"UNUSE",  /* EfiUnusableMemory          */
  L"ACPIR",  /* EfiACPIReclaimMemory       */
  L"ACPI ",  /* EfiACPIMemoryNVS           */
  L"MAPIO",  /* EfiMemoryMappedIO          */
  L"MAPP ",  /* EfiMemoryMappedIOPortSpace */
  L"PAL  ",  /* EfiPalCode                 */
  L"FLASH",  /* EfiPersistentMemory        */
};

/* bootGetMemMap function */
void bootGetMemMap(void)
{
  /* Local variables. */
  EFI_STATUS             result;
  UINTN                  mapSize = 0;
  EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
  UINTN                  mapKey;
  UINTN                  descriptorSize;
  UINT32                 descriptorVersion;
  UINT32                 i;

  /* Call GetMemoryMap with NULL to get the size of the map. */
  result = (EFI_STATUS) uefi_call_wrapper(
             (void *)bootSystemTable->BootServices->GetMemoryMap,
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
             (void *)bootSystemTable->BootServices->AllocatePool,
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
             (void *)bootSystemTable->BootServices->GetMemoryMap,
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
  Print(L"-------------------------------------------------------\n");
  Print(L"   TYPE         START          END           ATTR      \n");
  Print(L"-------------------------------------------------------\n");

  /* Loop over memory map entries. */
  for(i = 0; i < (mapSize/descriptorSize); i++)
  {
    /* Print leading spaces at the beginning of the line. */
    Print(L"   ");

    /* Print entry type. */
    Print(L"%s", bootEFIMemTypes[memoryMap->Type]);
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

    /* Get next entry in the table. */
    memoryMap = (EFI_MEMORY_DESCRIPTOR*)(((char*)memoryMap)+descriptorSize);
  }

  /* Print extra empty line. */
  Print(L"\n");

  /* Store memory map key in data section. */
  bootMemMapKey = mapKey;
}
