#ifndef OS_H
#define OS_H

void osDebugPrintStr(char *str);

void osKernelInitialize(void);
void osKernelStart(void);

void osPowerOff(void);

void osSerialPut(char c);

#endif /* OS_H */
