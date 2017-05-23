#include "flash.h"

u32 STMFlash_ReadWord(u32 addr)
{
	return *(vu32*)addr+((*(vu32*)(addr+2))<<16);
}

void STMFlash_WriteWord(u32 addr,u32 data)
{
	FLASH_Unlock();
	FLASH_ErasePage(addr);
	FLASH_ProgramWord(addr,data);
	FLASH_Lock();
}
