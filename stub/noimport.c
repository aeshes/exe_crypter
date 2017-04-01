#include "noimport.h"

__declspec(naked) unsigned int GetKernel32(void)
{
	__asm
	{
		mov eax, fs:[0x30];		/* Get pointer to PEB */
		mov eax, [eax + 0xC];	/* Get pointer to PEB_LDR_DATA */
		mov eax, [eax + 0x14];	/* InMemoryOrderModuleList */

		mov eax, [eax];			/* Get the 2-nd entry */
		mov eax, [eax];			/* Get the 3-rd entry */
		mov eax, [eax + 0x10];	/* Get the 3-rd entry base address */

		ret;
	};
}

/*DWORD Hash(char *name)
{
	DWORD h = 0x74D8;
	while (*name++)
		h ^= (h << 7) + *name;
	return h;
}*/

DWORD Hash(char* str)
{
   unsigned int hash = 0x01b63a;

   for(; *str; str++)
   {
      hash = ((hash << 5) + hash) + (*str);
   }
   return hash;	
}

DWORD GetAPI(const DWORD library, const DWORD APIHASH)
{
	if (library)
	{
		PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER)library;
		PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS)(library + dos_hdr->e_lfanew);
		PIMAGE_OPTIONAL_HEADER optional_hdr = &nt_hdr->OptionalHeader;
		PIMAGE_DATA_DIRECTORY data_directory = optional_hdr->DataDirectory;
		PIMAGE_EXPORT_DIRECTORY export = (PIMAGE_EXPORT_DIRECTORY)(library + data_directory[0].VirtualAddress);

		DWORD *names = (DWORD *)(library + export->AddressOfNames);
		WORD *ordinals = (WORD *)(library + export->AddressOfNameOrdinals);
		DWORD *functions = (DWORD *)(library + export->AddressOfFunctions);

		for (int i = 0; i < export->NumberOfNames; ++i)
		{
			char *name = (char *)(library + names[i]);
			if (Hash(name) == APIHASH)
			{
				return functions[ordinals[i]] + (DWORD)library;
			}
		}
	}
	return 0;
}

DWORD CallAPI(DWORD dwPtr, WORD n, ...)
{
	BYTE* asmbuff;
	DWORD ret;
	WORD i;
	va_list ap;


	asmbuff = (BYTE*)VirtualAlloc(NULL, 17 + (5 * n), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!asmbuff)
		return (DWORD)-1;

	*(DWORD*)asmbuff = (DWORD)GetAPI(GetKernel32(), dwPtr);
	if (!(*(DWORD*)asmbuff))
		return (DWORD)-1;

	*(DWORD*)&asmbuff[4] = 0x59595958;
	*(WORD*)&asmbuff[8] = 0x5059;

	va_start(ap, n);

	for (i = n - 1; i != (WORD)-1; i--)
	{
		asmbuff[10 + (5 * i)] = 0x68;
		*(DWORD*)&asmbuff[10 + (5 * i) + 1] = va_arg(ap, DWORD);
	}

	va_end(ap);

	*(WORD*)&asmbuff[10 + (n * 5)] = 0x15FF;
	*(DWORD*)&asmbuff[12 + (n * 5)] = (DWORD)asmbuff;
	asmbuff[16 + (n * 5)] = 0xC3;

	ret = CallWindowProc((WNDPROC)(asmbuff + 4), 0, 0, 0, 0);

	VirtualFree(asmbuff, 0, MEM_RELEASE);

	return ret;
}
