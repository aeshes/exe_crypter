#include "intrinsic.h"

void __stdcall MemCpy(LPVOID lpMem, LPVOID lpMem2, DWORD dwLen)
{
    _asm
    {
        mov        edi,[lpMem]
        mov        esi,[lpMem2]
        mov        ecx,[dwLen]
        rep        movsb
    }
}

void __stdcall MemSet(void *szBuffer, DWORD dwLen, DWORD dwSym)
{
    _asm
    {
        pushad
        mov        edi,[szBuffer]
        mov        ecx,[dwLen]
        mov        eax,[dwSym]
        rep        stosb
        popad
    }
}
