
/* UEFI includes (for efi_main prototype). */
#include "efi.h"

/* Boot loader includes. */
#include "boot/boot.h"

/* Kernel includes. */
#include "kernel/os.h"

/* efi_main function. */
EFI_STATUS EFIAPI efi_main(EFI_HANDLE        ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable)
{
  /* Initialize the boot loader. */
  bootInitialize(ImageHandle, SystemTable);

  /* Initialize ARTOS kernel. */
  osKernelInitialize();

  /* Create first thread to be executed by the kernel. */
  /* osThreadNew(someThread, NULL, NULL); */

  /* Start the kernel. */
  osKernelStart();

  /* Should never reach this line. */
  return EFI_SUCCESS;
}
