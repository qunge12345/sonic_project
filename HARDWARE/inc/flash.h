#ifndef __FLASH__H
#define __FLASH__H
#include "stm32f10x.h"

#define FLASH_SAVEADDR 0x0803F800

u32 STMFlash_ReadWord(u32 addr);
void STMFlash_WriteWord(u32 addr,u32 data);
#endif

