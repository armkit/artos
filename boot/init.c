/* UEFI includes. */
#include "efi.h"
#include "efilib.h"

/* Bootloader includes. */
#include "boot/boot.h"
#include "boot/boot_priv.h"

/* UEFI boot data */
EFI_HANDLE bootImageHandle;
EFI_SYSTEM_TABLE *bootSystemTable;

void bootInitialize(EFI_HANDLE        ImageHandle,
                    EFI_SYSTEM_TABLE *SystemTable)
{
  /* Store UEFI ImageHandle in data section. */
  bootImageHandle = ImageHandle;

  /* Store UEFI SystemTable in data section. */
  bootSystemTable = SystemTable;

  /* Initialize EFI library. */
  InitializeLib(ImageHandle, SystemTable);

  /* Print intro message. */
  bootPrintSplashMsg();

  /* Get memory map. */
  bootGetMemMap();

  /* Exit boot services. */
  bootExitUEFI();
}
