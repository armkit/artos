
/* Kernel includes. */
#include "kernel/os.h"
#include "kernel/os_priv.h"

void osPowerOff(void)
{
  /* Power State Coordination Interface: SYSTEM SHUTDOWN */
  __asm__("LDR W0, =0x84000008");
  __asm__("HVC #0");
}
