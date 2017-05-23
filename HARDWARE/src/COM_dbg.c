#include "COM_dbg.h"
#include "stdarg.h"
#include "stdio.h"


//void COM_Init()
//{
//	USART_InitTypeDef USART_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	
//	
//	//TX
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	//RX
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
//	
//	USART_InitStructure.USART_BaudRate=115200;
//	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
//	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits=USART_StopBits_1;
//	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Parity=USART_Parity_No;
//	USART_Init(USART1,&USART_InitStructure);
//	USART_Cmd(USART1,ENABLE);
//	
//	USART_GetFlagStatus(USART1,USART_FLAG_TC);
//}

u8 COM_PutString(char const *s)
{
	while(*s!='\0')
	{
		COM_PutChar(*s);
		++s;
	}
	return 0;
}

u8 COM_PutChar(char c)
{
	while(USART_GetFlagStatus(UART5,USART_FLAG_TXE)!=SET) ;
	USART_SendData(UART5,c);
	return 0;
}

int COM_printf(const char* fmt,...)
{
	char printf_buff[1024];
	va_list args;
	
	int printed;
	va_start(args,fmt);
	
	printed=vsprintf(printf_buff,fmt,args);
	va_end(args);
	COM_PutString(printf_buff);
	return printed;
}

