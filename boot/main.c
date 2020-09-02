#include <efi.h>
#include <efilib.h>

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

void UartPutChar(char c)
{
  /* try to print c */
  while (UARTFR & TXFF);
  UARTDR = c;
  while (!(UARTFR & TXFE));
}

void UartPutStr(char *str)
{
  /* loop over str */	
  while (*str)
  {
    UartPutChar(*str++);
  } 
}

void uart(void)
{
  /* printing registers and their address */
  Print(L"\n");
  Print(L"UARTFR:    0x%X\n", UARTFR);
  Print(L"UARTIBRD:  0x%X\n", UARTIBRD);
  Print(L"UARTFBRD:  0x%X\n", UARTFBRD);
  Print(L"UARTCR:    0x%X\n", UARTCR);

  /* print something using our uart code */
  UartPutStr("Hello world!\n");
}

void get_memory_map(EFI_HANDLE ImageHandle, 
                    EFI_SYSTEM_TABLE *SystemTable)
{	
  EFI_STATUS             result;
  UINTN                  mapSize = 0;
  EFI_MEMORY_DESCRIPTOR *memoryMap = NULL;
  UINTN                  mapKey; 
  UINTN                  descriptorSize;
  UINT32                 descriptorVersion;
  UINT32                 i;
  unsigned short         *types[] = {L"RESV ", L"LDC  ", L"LDD  ", L"BSC  ", L"BSD  ", L"RTSC ", L"RTSD ", L"CONV ", L"UNUSE", L"ACPIR", L"ACPIM", L"MAPIO", L"MAPP ", L"PALC ", L"PERM ", L"MAXM "};
 
  Print(L"\n");

  result = (EFI_STATUS) uefi_call_wrapper((void *)SystemTable->BootServices->GetMemoryMap,
                             5, 
							 &mapSize, 
							 memoryMap, 
							 NULL, 
							 &descriptorSize, 
							 NULL);

  /*Print(L"mapSize: %d\n", mapSize); */
  /*Print(L"descriptorSize: %d\n", descriptorSize); */
  
  mapSize += 2 * descriptorSize;

  result = (EFI_STATUS) uefi_call_wrapper((void *)SystemTable->BootServices->AllocatePool, 
                             3, 
							 EfiLoaderData, 
							 mapSize, 
							 (void **)&memoryMap);

  /*Print(L"memoryMap: 0x%X\n", memoryMap);*/
  
  result = (EFI_STATUS) uefi_call_wrapper((void *)SystemTable->BootServices->GetMemoryMap, 
                              5, 
							  &mapSize, 
							  memoryMap, 
							  &mapKey, 
							  &descriptorSize, 
							  &descriptorVersion);

  /*Print(L"mapSize: %d\n", mapSize);*/
  /*Print(L"memoryMap: 0x%X\n", memoryMap);*/
  /*Print(L"mapKey: %d\n", mapKey);*/
  /*Print(L"descriptorSize: %d\n", descriptorSize);*/
  /*Print(L"descriptorVersion: %d\n", descriptorVersion);*/

  Print(L"-------------------------------------------------------\n");
  Print(L"   Type         SADS           EADS          ATRB\n");
  Print(L"-------------------------------------------------------\n");
  for(i = 0; i < (mapSize/descriptorSize); i++)
  {
	  Print(L"   ");
	  Print(L"%s", types[memoryMap->Type]);
	  Print(L"     ");
	  Print(L"0x%X", memoryMap->PhysicalStart);
	  Print(L"     ");
	  Print(L"0x%X", memoryMap->PhysicalStart + memoryMap->NumberOfPages*4096-1);
	  Print(L"     ");	  
	  Print(L"0x%X", memoryMap->Attribute); 
	  Print(L"   ");
	  Print(L"\n");
	  
	  memoryMap = (EFI_MEMORY_DESCRIPTOR *)(((char *)memoryMap) + descriptorSize);
  }
  result = (EFI_STATUS) uefi_call_wrapper((void *)SystemTable->BootServices->ExitBootServices, 
                             2, 
							 ImageHandle, 
							 mapKey);
  (void) result;
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle,
                           EFI_SYSTEM_TABLE *SystemTable)
{
  /* initialize EFI library */
  InitializeLib(ImageHandle, SystemTable);

  /* print empty line*/
  Print(L"\n");

  /* print splash */
  Print(L"Hello world!\n");
  
  /* temporary function */
  uart();
  
  /* get efi_memory_map */
  get_memory_map(ImageHandle, SystemTable);
  
  /* Power State Coordination Interface: SYSTEM SHUTDOWN */
  __asm__("LDR W0, =0x84000008");
  __asm__("HVC #0");
  
  /* program should never reach here */
  return EFI_SUCCESS;
}
