#ifndef BOOT_PRIV_H
#define BOOT_PRIV_H

extern EFI_HANDLE bootImageHandle;
extern EFI_SYSTEM_TABLE *bootSystemTable;

extern UINTN bootMemMapKey;

void bootPrintSplashMsg(void);

void bootGetMemMap(void);

void bootExitUEFI(void);

#endif /* BOOT_PRIV_H */
