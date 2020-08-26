#include <efi.h>
#include <efilib.h>

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
  char                   *types[] = {L"RESV ", L"LDC  ", L"LDD  ", L"BSC  ", L"BSD  ", L"RTSC ", L"RTSD ", L"CONV ", L"UNUSE", L"ACPIR", L"ACPIM", L"MAPIO", L"MAPP ", L"PALC ", L"PERM ", L"MAXM "};
 
  result = uefi_call_wrapper((void *)SystemTable->BootServices->GetMemoryMap,
                             5, 
							 &mapSize, 
							 memoryMap, 
							 NULL, 
							 &descriptorSize, 
							 NULL);

  Print(L"mapSize: %d\n", mapSize);
  Print(L"descriptorSize: %d\n", descriptorSize);
  
  mapSize += 2 * descriptorSize;

  result = uefi_call_wrapper((void *)SystemTable->BootServices->AllocatePool, 
                             3, 
							 EfiLoaderData, 
							 mapSize, 
							 (void **)&memoryMap);

  Print(L"memoryMap: 0x%X\n", memoryMap);
  
  result = uefi_call_wrapper((void *)SystemTable->BootServices->GetMemoryMap, 
                              5, 
							  &mapSize, 
							  memoryMap, 
							  &mapKey, 
							  &descriptorSize, 
							  &descriptorVersion);


  Print(L"mapSize: %d\n", mapSize);
  Print(L"memoryMap: 0x%X\n", memoryMap);
  Print(L"mapKey: %d\n", mapKey);
  Print(L"descriptorSize: %d\n", descriptorSize);
  Print(L"descriptorVersion: %d\n", descriptorVersion);
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
  result = uefi_call_wrapper((void *)SystemTable->BootServices->ExitBootServices, 
                             2, 
							 ImageHandle, 
							 mapKey);
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
  
  /* get efi_memory_map */
  get_memory_map(ImageHandle, SystemTable);
  
  /* Power State Coordination Interface: SYSTEM SHUTDOWN */
  __asm__("LDR W0, =0x84000008");
  __asm__("HVC #0");
  
  /* program should never reach here */
  return EFI_SUCCESS;
}
