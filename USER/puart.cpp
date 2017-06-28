#include "puart.h"

#define MAXRECVBUF					8
struct
{
	char buf[MAXRECVBUF];
	uint8_t count;
}puart_buf[4];

void puart_init()
{
//	GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	USART1_Tx PA.9		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	USART1_Rx PA.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//	USART1
	USART_Init(USART1,&USART_InitStructure);
//	USART1 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;						//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	USART2_Tx PA.2	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	USART2_Rx PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//	USART2
	USART_Init(USART2,&USART_InitStructure);
//	USART2 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;						//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	USART3_Tx PB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	USART3_Rx PB.11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//	USART3
	USART_Init(USART3,&USART_InitStructure);
//	USART3 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;						//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
//	USART4_Tx PC.10	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);
//	USART4_Rx PC.11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);
//	USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//	UART4
	USART_Init(UART4,&USART_InitStructure);
//	UART4 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;						//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART4,ENABLE);
}

void puart_send_byte(char c)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) != SET) ;
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET) ;
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) != SET) ;
	while(USART_GetFlagStatus(UART4,USART_FLAG_TXE) != SET) ;
	USART_SendData(USART1, c);
	USART_SendData(USART2, c);
	USART_SendData(USART3, c);
	USART_SendData(UART4, c);
}

void puart_send_buf(char *buf,uint8_t length)
{
	for(uint8_t i = 0;i < length;++i)
	{
		puart_send_byte(buf[i]);
	}
}

void puart_read_buf(uint8_t bufnum,char *buf,uint8_t length)
{
	if(bufnum >4) return;
	for(uint8_t i=0;i<length;++i)
	{
		buf[i] = puart_buf[bufnum].buf[i];
	}
	puart_buf[bufnum].count = 0;
}

uint8_t puart_get_buf_count(uint8_t bufnum)
{
	if(bufnum > 4) return 0;
	return puart_buf[bufnum].count;
}

void puart_buf_reset(uint8_t bufnum)
{
	if(bufnum >= 4)																//复位uart1-4 缓冲区计数值
	{
		puart_buf[0].count = 0;
		puart_buf[1].count = 0;
		puart_buf[2].count = 0;
		puart_buf[3].count = 0;
	}
	else{
		puart_buf[bufnum].count = 0;							//复位单个uart 缓冲区计数值
	}
}

#ifdef __cplusplus
 extern "C" {
#endif
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		if(puart_buf[0].count < MAXRECVBUF)
		{
			puart_buf[0].buf[puart_buf[0].count] = USART_ReceiveData(USART1);
		}
		++puart_buf[0].count;
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		if(puart_buf[1].count < MAXRECVBUF)
		{
			puart_buf[1].buf[puart_buf[1].count] = USART_ReceiveData(USART2);
		}
		++puart_buf[1].count;
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		if(puart_buf[2].count < MAXRECVBUF)
		{
			puart_buf[2].buf[puart_buf[2].count] = USART_ReceiveData(USART3);
		}
		++puart_buf[2].count;
	}
}

void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4,USART_IT_RXNE) == SET)
	{
		if(puart_buf[3].count < MAXRECVBUF)
		{
			puart_buf[3].buf[puart_buf[3].count] = USART_ReceiveData(UART4);
		}
		++puart_buf[3].count;
	}
}

#ifdef __cplusplus
}
#endif

