#include <windows.h>
#include <stdio.h>

char key[8] = { 0x9A, 0xF7, 0x13, 0x22, 0xD9, 0x3C, 0x17, 0xEF };

#define swap(x, y, T) do { T swap = x; x = y; y = swap; } while (0)

#define BLOCK_SIZE	8

void * ReadImage(LPSTR szPath, DWORD *dwSize);
void EncryptImage(LPBYTE pImage, DWORD dwImageSize);
void EncryptBlock(LPBYTE pBlock);
void AddResource(LPSTR szPath, LPBYTE lpBuffer, DWORD dwSize, int ResId);

int main(int argc, char *argv[])
{
	LPVOID lpImage = NULL;
	DWORD dwImageSize = 0;

	if (argc > 2)
	{
		lpImage = ReadImage(argv[2], &dwImageSize);
		EncryptImage(lpImage, dwImageSize);
		AddResource(argv[1], lpImage, dwImageSize, 100);
	}
}

void * ReadImage(LPSTR szPath, DWORD *dwSize)
{
	HANDLE hFile;
	DWORD dwFileSize;
	DWORD dwBytesRead;
	LPBYTE lpBuffer;

	hFile = CreateFileA(szPath,
		GENERIC_READ, 
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE != hFile)
	{
		dwFileSize = GetFileSize(hFile, NULL);

		lpBuffer = HeapAlloc(GetProcessHeap(), 0, dwFileSize);
		ReadFile(hFile, lpBuffer, dwFileSize, &dwBytesRead, NULL);
		CloseHandle(hFile);

		*dwSize = dwFileSize;
		return lpBuffer;
	}
	return NULL;
}

void AddResource(LPSTR szPath, LPBYTE lpBuffer, DWORD dwSize, int ResId)
{
	HANDLE hResource;

	hResource = BeginUpdateResource(szPath, FALSE);
	if (NULL != hResource)
	{
		if (UpdateResource(hResource, 
			RT_RCDATA, 
			MAKEINTRESOURCE(ResId), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPVOID)lpBuffer, 
			dwSize) != FALSE)
		{
			EndUpdateResource(hResource, FALSE);
		}
	}
}

void EncryptImage(LPBYTE pImage, DWORD dwImageSize)
{
	DWORD nBlocks = dwImageSize / BLOCK_SIZE;
	for (size_t block = 0; block < nBlocks; block++)
	{
		EncryptBlock(pImage);
		pImage += BLOCK_SIZE;
	}
	/*for (DWORD i = 0; i < dwImageSize; i++)
	{
		pImage[i] ^= key[i % sizeof(key)];
	}*/
}

void EncryptBlock(LPBYTE pBlock)
{
	swap(pBlock[0], pBlock[1], BYTE);	/* two leftmost bytes */
	swap(pBlock[6], pBlock[7], BYTE);	/* two rightmost bytes */
	swap(pBlock[2], pBlock[5], BYTE);	/* bytes around two central bytes */
	swap(pBlock[3], pBlock[4], BYTE);	/* two central bytes */
}
