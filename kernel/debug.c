
/* Kernel includes. */
#include "kernel/os.h"
#include "kernel/os_priv.h"

void osDebugPrintStr(char *str)
{
  /* loop over str */
  while (*str)
  {
    /* Print character using kernel's internal UART driver. */
    osSerialPut(*str++);
  }
}
