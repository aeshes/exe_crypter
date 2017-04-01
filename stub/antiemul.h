#ifndef ANTIEMUL_H
#define ANTIEMUL_H

#include <windows.h>
#include <stdbool.h>
#include "peb.h"
#include "noimport.h"
#include "crypt.h"

void BypassAV(void);
void AntiSandbox(void);
void AntiDebug(void);

#endif
