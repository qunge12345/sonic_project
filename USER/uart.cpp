#include "uart.h"
#include "stdarg.h"
#include "stdio.h"

void uart_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;							//TX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;								//RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(UART5,&USART_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//子优先级2
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级2
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;						//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
//	
//	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(UART5,ENABLE);
}

void uart_put_char(char c)
{
	while(USART_GetFlagStatus(UART5,USART_FLAG_TXE) != SET) ;
	USART_SendData(UART5, c);
}

void uart_put_string(char * s)
{
	while(*s!='\0')
	{
		uart_put_char(*s);
		++s;
	}
}

void uart_printf(const char *fmt, ...)
{
	char printf_buff[256];
	va_list args;

	va_start(args,fmt);

	vsnprintf(printf_buff,256,fmt,args);
	va_end(args);
	uart_put_string(printf_buff);
}


