
/* UEFI includes. */
#include "efi.h"
#include "efilib.h"

/* Bootloader includes. */
#include "boot/boot.h"
#include "boot/boot_priv.h"

void bootExitUEFI(void)
{
  /* Local variables. */
  EFI_STATUS             result;

  /* Simply call ExitBootServices to exit UEFI. */
  result = (EFI_STATUS) uefi_call_wrapper(
             (void *)bootSystemTable->BootServices->ExitBootServices,
             2,
             bootImageHandle,
             bootMemMapKey);

  /* Make sure ExitBootServices is done successfully. */
  if (result != EFI_SUCCESS)
  {
    Print(L"BOOTLOADER: Failed to exit UEFI boot services.\n");
    while(1);
  }
}
