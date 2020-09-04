
/* Kernel includes. */
#include "kernel/os.h"
#include "kernel/os_priv.h"

/* PL011 registers */
#define UARTDR            (*((volatile unsigned short *) 0x09000000))
#define UARTRSR           (*((volatile unsigned short *) 0x09000004))
#define UARTECR           (*((volatile unsigned short *) 0x09000004))
#define UARTFR            (*((volatile unsigned short *) 0x09000018))
#define UARTILPR          (*((volatile unsigned short *) 0x09000020))
#define UARTIBRD          (*((volatile unsigned short *) 0x09000024))
#define UARTFBRD          (*((volatile unsigned short *) 0x09000028))
#define UARTFLCR_H        (*((volatile unsigned short *) 0x0900002C))
#define UARTCR            (*((volatile unsigned short *) 0x09000030))

/* UARTFR flags */
#define CTS               (0x0001)
#define DSR               (0x0002)
#define DCD               (0x0004)
#define BUSY              (0x0008)
#define RXFE              (0x0010)
#define TXFF              (0x0020)
#define RXFF              (0x0040)
#define TXFE              (0x0080)
#define RI                (0x0100)

/* UARTCR flags */
#define UARTEN            (0x0001)
#define SIREN             (0x0002)
#define SIRLP             (0x0004)
#define LBE               (0x0080)
#define TXE               (0x0100)
#define RXE               (0x0200)
#define DTR               (0x0400)
#define RTS               (0x0800)
#define Out1              (0x1000)
#define Out2              (0x2000)
#define RTSEn             (0x4000)
#define CTSEn             (0x8000)

void osSerialPut(char c)
{
  /* try to print c */
  while (UARTFR & TXFF);
  UARTDR = c;
  while (!(UARTFR & TXFE));
}
