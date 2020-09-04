
/* Kernel includes. */
#include "kernel/os.h"
#include "kernel/os_priv.h"

void UartPutStr(char *str);

void osKernelInitialize(void)
{
  /* Print something. */
  osDebugPrintStr("Hello from kernel!\n");
}

void osKernelStart(void)
{
  /* Just shutdown for now. */
  osPowerOff();
}
