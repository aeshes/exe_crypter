#ifndef INTRINSIC_H
#define INTRINSIC_H

#include <windows.h>

void __stdcall MemCpy(LPVOID lpMem, LPVOID lpMem2, DWORD dwLen);
void __stdcall MemSet(void *szBuffer, DWORD dwLen, DWORD dwSym);

#endif
