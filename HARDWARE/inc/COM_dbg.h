#ifndef __COM__DBG__H
#define __COM__DBG__H
#include "stm32f10x.h"

//class COM_DBG
//{
//	private:
//		
//	public:
//		COM_DBG();
//		void COM_Init();
//		u8 COM_PutChar(char c);
//		u8 COM_PutString(char const *c);
//		int printf(const char* fmt,...);
//};
u8 COM_PutChar(char c);
u8 COM_PutString(char const *s);
int COM_printf(const char* fmt,...);

#endif
