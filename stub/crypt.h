#ifndef CRYPT_H
#define CRYPT_H

#include <windows.h>
#include "antiemul.h"

#define BLOCK_SIZE	8

void DecryptBlock(LPBYTE pBlock);
void DecryptImage(LPBYTE pImage, DWORD dwImageSize);
void XorBlock(LPBYTE pBlock, DWORD dwSize);

#endif
