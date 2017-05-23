#include "Distance.h"
#include "delay.h"
#include "My_Tim.h"
#include "led.h"
#include "COM_dbg.h"
#define Test_Timeout 1000
#define Test_nTimes		5

u8 Config_CMD=0x00;
u16 *Distance_Buffer;
u16 *USART1_Buffer,*USART2_Buffer,*USART3_Buffer,*USART4_Buffer;
u8 isData1[4]={0,0,0,0};
enum USART_Status USART1_Status=Complete;
enum USART_Status USART2_Status=Complete;
enum USART_Status USART3_Status=Complete;
enum USART_Status USART4_Status=Complete;
Distance_TypeDef Distance_Structure[6][4]={{{USART1,0xe0,0,0xffff},{USART2,0xe0,0,0xffff},{USART3,0xe0,0,0xffff},{UART4,0xe0,0,0xffff}},
																					 {{USART1,0xe2,0,0xffff},{USART2,0xe2,0,0xffff},{USART3,0xe2,0,0xffff},{UART4,0xe2,0,0xffff}},
																					 {{USART1,0xe4,0,0xffff},{USART2,0xe4,0,0xffff},{USART3,0xe4,0,0xffff},{UART4,0xe4,0,0xffff}},
																					 {{USART1,0xe6,0,0xffff},{USART2,0xe6,0,0xffff},{USART3,0xe6,0,0xffff},{UART4,0xe6,0,0xffff}},
																					 {{USART1,0xe8,0,0xffff},{USART2,0xe8,0,0xffff},{USART3,0xe8,0,0xffff},{UART4,0xe8,0,0xffff}},
																					 {{USART1,0xea,0,0xffff},{USART2,0xea,0,0xffff},{USART3,0xea,0,0xffff},{UART4,0xea,0,0xffff}}};

int Distance_Init(void)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_USART1, ENABLE);	//GPIOA,B,C时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3|RCC_APB1Periph_UART4,ENABLE);//
	
  //USART2_TX   PA.2 			USART1_Tx PA.9		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_9; //
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART2_RX	  PA.3			USART1_Rx PA.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_10;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART3_Tx	PB.10				USART4_Tx PC.10
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//USART3_Rx	PB.11				USART4_Rx PC.11
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//USART1-4 NVIC 
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;//|USART2_IRQn|USART3_IRQn|UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;						//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn;
	NVIC_Init(&NVIC_InitStructure);
  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	//USART1
	USART_Init(USART1,&USART_InitStructure);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1,ENABLE);
	//USART2
  USART_Init(USART2, &USART_InitStructure); //初始化串口
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
  USART_Cmd(USART2, ENABLE);                    //使能串口 
	//USART3
	USART_Init(USART3,&USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3,ENABLE);
	//UART4
	USART_Init(UART4,&USART_InitStructure);
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
	USART_Cmd(UART4,ENABLE);
	USART_GetITStatus(USART1, USART_FLAG_TC);
	USART_GetITStatus(USART2, USART_FLAG_TC);
	USART_GetITStatus(USART3, USART_FLAG_TC);
	USART_GetITStatus(UART4, USART_FLAG_TC);
	return 0;
}

int Distance_Init232(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;				//TX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;					//RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate=115200;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(UART5,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;				//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;						//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
	
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(UART5,ENABLE);
	return 0;
}

void Distance_232SendByte(u8 data)
{
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
	USART_SendData(UART5,data);
}

void Distance_232SendDword(u32 data)
{
	for(u8 i=0;i<4;++i)
	{
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
		USART_SendData(UART5,data);
		data>>=8;
	}
}

int Distance_SendBuffer232(u8 length,u16 *buffer)
{
	while(length)
	{
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
		USART_SendData(UART5,*buffer>>8);
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
		USART_SendData(UART5,*buffer);
		--length;
		++buffer;
	}
	return 0;
}

int Distance_SendByte(USART_TypeDef* USARTx,unsigned char data)
{
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET) ;
	USART_SendData(USARTx,data);
	return 0;
}

int Distance_SetAddr(USART_TypeDef* USARTx,u8 addr_old,unsigned char addr_new)
{
	Distance_SendByte(USARTx,addr_old);
	Distance_SendByte(USARTx,0x02);
	Distance_SendByte(USARTx,0x9A);
	delay_ms(6);
	Distance_SendByte(USARTx,addr_old);
	Distance_SendByte(USARTx,0x02);
	Distance_SendByte(USARTx,0x92);
	delay_ms(6);
	Distance_SendByte(USARTx,addr_old);
	Distance_SendByte(USARTx,0x02);
	Distance_SendByte(USARTx,0x9e);
	delay_ms(6);
	Distance_SendByte(USARTx,addr_old);
	Distance_SendByte(USARTx,0x02);
	Distance_SendByte(USARTx,addr_new);
	delay_ms(100);
	return 0;
}

void Distance_ResetAddr(Distance_TypeDef* Distance_Structure)
{
	u8 addr_temp;
	for(addr_temp=0xd0;addr_temp!=0;addr_temp+=2)
	{
		Distance_SendByte(Distance_Structure->USARTx,addr_temp);
		Distance_SendByte(Distance_Structure->USARTx,0x02);
		Distance_SendByte(Distance_Structure->USARTx,0x9A);
		delay_ms(6);
		Distance_SendByte(Distance_Structure->USARTx,addr_temp);
		Distance_SendByte(Distance_Structure->USARTx,0x02);
		Distance_SendByte(Distance_Structure->USARTx,0x92);
		delay_ms(6);
		Distance_SendByte(Distance_Structure->USARTx,addr_temp);
		Distance_SendByte(Distance_Structure->USARTx,0x02);
		Distance_SendByte(Distance_Structure->USARTx,0x9e);
		delay_ms(6);
		Distance_SendByte(Distance_Structure->USARTx,addr_temp);
		Distance_SendByte(Distance_Structure->USARTx,0x02);
		Distance_SendByte(Distance_Structure->USARTx,Distance_Structure->Addr);
		delay_ms(6);
	}
	delay_ms(100);
}

void Distance_DefaultAddr(u8 row)
{
	u8 addr_temp;
	//Sensor_PwrCtrl(row);
	for(addr_temp=0xd0;addr_temp!=0;addr_temp+=2)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,addr_temp);
		USART_SendData(USART2,addr_temp);
		USART_SendData(USART3,addr_temp);
		USART_SendData(UART4,addr_temp);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,0x02);
		USART_SendData(USART2,0x02);
		USART_SendData(USART3,0x02);
		USART_SendData(UART4,0x02);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,0x9a);
		USART_SendData(USART2,0x9a);
		USART_SendData(USART3,0x9a);
		USART_SendData(UART4,0x9a);
		delay_ms(6);
		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,addr_temp);
		USART_SendData(USART2,addr_temp);
		USART_SendData(USART3,addr_temp);
		USART_SendData(UART4,addr_temp);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,0x02);
		USART_SendData(USART2,0x02);
		USART_SendData(USART3,0x02);
		USART_SendData(UART4,0x02);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,0x92);
		USART_SendData(USART2,0x92);
		USART_SendData(USART3,0x92);
		USART_SendData(UART4,0x92);
		delay_ms(6);
		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,addr_temp);
		USART_SendData(USART2,addr_temp);
		USART_SendData(USART3,addr_temp);
		USART_SendData(UART4,addr_temp);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,0x02);
		USART_SendData(USART2,0x02);
		USART_SendData(USART3,0x02);
		USART_SendData(UART4,0x02);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,0x9e);
		USART_SendData(USART2,0x9e);
		USART_SendData(USART3,0x9e);
		USART_SendData(UART4,0x9e);
		delay_ms(6);
		
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,addr_temp);
		USART_SendData(USART2,addr_temp);
		USART_SendData(USART3,addr_temp);
		USART_SendData(UART4,addr_temp);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,0x02);
		USART_SendData(USART2,0x02);
		USART_SendData(USART3,0x02);
		USART_SendData(UART4,0x02);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
		USART_SendData(USART1,Distance_Structure[row][0].Addr);
		USART_SendData(USART2,Distance_Structure[row][1].Addr);
		USART_SendData(USART3,Distance_Structure[row][2].Addr);
		USART_SendData(UART4,Distance_Structure[row][3].Addr);
		delay_ms(6);
	}
	delay_ms(500);
}

u8 Distance_ShowConnectionStatus(u8 row)
{
	u8 temp=0;
	Sensor_PwrCtrl(6);
	delay_ms(500);
	Distance_Structure[row][0].Data=0xffff;
	Distance_Structure[row][1].Data=0xffff;
	Distance_Structure[row][2].Data=0xffff;
	Distance_Structure[row][3].Data=0xffff;
	USART1_Buffer=&(Distance_Structure[row][0].Data);
	USART2_Buffer=&(Distance_Structure[row][1].Data);
	USART3_Buffer=&(Distance_Structure[row][2].Data);
	USART4_Buffer=&(Distance_Structure[row][3].Data);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
	USART_SendData(USART1,Distance_Structure[row][0].Addr);
	USART_SendData(USART2,Distance_Structure[row][1].Addr);
	USART_SendData(USART3,Distance_Structure[row][2].Addr);
	USART_SendData(UART4,Distance_Structure[row][3].Addr);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
	USART_SendData(USART1,0x02);
	USART_SendData(USART2,0x02);
	USART_SendData(USART3,0x02);
	USART_SendData(UART4,0x02);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
	isData1[0]=0;
	isData1[1]=0;
	isData1[2]=0;
	isData1[3]=0;
	USART_SendData(USART1,0xa0);
	USART_SendData(USART2,0xa0);
	USART_SendData(USART3,0xa0);
	USART_SendData(UART4,0xa0);
	delay_ms(5);
//	if(Distance_Structure[row][0].Data<0x400&Distance_Structure[row][0].Data>0) //兼容非定制版探头
//	{
//		LED_ON(0);
//		temp|=0x01;
//	}
//	else
//		LED_OFF(0);
//	if(Distance_Structure[row][1].Data<0x400&Distance_Structure[row][1].Data>0)
//	{
//		LED_ON(1);
//		temp|=0x02;
//	}
//	else
//		LED_OFF(1);
//	if(Distance_Structure[row][2].Data<0x400&Distance_Structure[row][2].Data>0)
//	{
//		LED_ON(2);
//		temp|=0x04;
//	}
//	else
//		LED_OFF(2);
//	if(Distance_Structure[row][3].Data<0x400&Distance_Structure[row][3].Data>0)
//	{
//		LED_ON(3);
//		temp|=0x08;
//	}
//	else
//		LED_OFF(3);
	if(Distance_Structure[row][0].Data==0x0211)																//兼容定制版探头
	{
		LED_ON(0);
		temp|=0x01;
	}
	else
		LED_OFF(0);
	COM_printf("USART1:0x%X  ",Distance_Structure[row][0].Data);
	if(Distance_Structure[row][1].Data==0x0211)
	{
		LED_ON(1);
		temp|=0x02;
	}
	else
		LED_OFF(1);
	COM_printf("USART2:0x%X  ",Distance_Structure[row][1].Data);
	if(Distance_Structure[row][2].Data==0x0211)
	{
		LED_ON(2);
		temp|=0x04;
	}
	else
		LED_OFF(2);
	COM_printf("USART3:0x%X  ",Distance_Structure[row][2].Data);
	if(Distance_Structure[row][3].Data==0x0211)
	{
		LED_ON(3);
		temp|=0x08;
	}
	else
		LED_OFF(3);
	COM_printf("USART4:0x%X\r\n",Distance_Structure[row][3].Data);
	return temp;
}

u32 Distance_Test(void)
{
	u32 temp=0;
	for(signed char i=5;i>=0;--i)
	{
		temp<<=4;
		LED_OFF(1);
		LED_OFF(2);
		LED_OFF(3);
		LED_OFF(4);
		LED_Toggle();
		for(u16 n=0;n<Test_nTimes;++n)
		{
			temp|=Distance_ShowConnectionStatus(i);
		}
	}
	return temp;
}

void Distance_Update(u8 row)
{
	//Sensor_PwrCtrl(row);
	Distance_Structure[row][0].Data=0xffff;
	Distance_Structure[row][1].Data=0xffff;
	Distance_Structure[row][2].Data=0xffff;
	Distance_Structure[row][3].Data=0xffff;
	USART1_Buffer=&(Distance_Structure[row][0].Data);
	USART2_Buffer=&(Distance_Structure[row][1].Data);
	USART3_Buffer=&(Distance_Structure[row][2].Data);
	USART4_Buffer=&(Distance_Structure[row][3].Data);
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
	USART_SendData(USART1,Distance_Structure[row][0].Addr);
	USART_SendData(USART2,Distance_Structure[row][1].Addr);
	USART_SendData(USART3,Distance_Structure[row][2].Addr);
	USART_SendData(UART4,Distance_Structure[row][3].Addr);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
	USART_SendData(USART1,0x02);
	USART_SendData(USART2,0x02);
	USART_SendData(USART3,0x02);
	USART_SendData(UART4,0x02);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) ;
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET) ;
	USART_SendData(USART1,0xb0);
	USART_SendData(USART2,0xb0);
	USART_SendData(USART3,0xb0);
	USART_SendData(UART4,0xb0);
	isData1[0]=0;
	isData1[1]=0;
	isData1[2]=0;
	isData1[3]=0;
	delay_ms(50);
	if(Distance_Structure[row][0].Data>5000)
		Distance_Structure[row][0].Data=0xFFFF;
	if(Distance_Structure[row][1].Data>5000)
		Distance_Structure[row][1].Data=0xFFFF;
	if(Distance_Structure[row][2].Data>5000)
		Distance_Structure[row][2].Data=0xFFFF;
	if(Distance_Structure[row][3].Data>5000)
		Distance_Structure[row][3].Data=0xFFFF;
}

void Distance_Upload(u8 row,u32 config)
{
	u8 check_sum=0;
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;	//
	USART_SendData(UART5,0xA5);
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;	//包头
	USART_SendData(UART5,0x5A);
	config>>=(4*row);
	config&=0x0f;
	u8 temp=config;
	u8 Sensor_Num=0;
	for(u8 i=0;i<4;++i)
	{
		if(temp&0x1)
			++Sensor_Num;
		temp>>=1;
	}
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
	USART_SendData(UART5,Sensor_Num);													//检测传感器数量
	check_sum+=Sensor_Num;
	temp=config;
	for(u8 i=0;i<4;++i)
	{
		if(temp&0x1)
		{
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
			USART_SendData(UART5,4*row);											//
			check_sum+=(4*row);
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
			USART_SendData(UART5,Distance_Structure[row][i].Data&0x00ff);//
			check_sum+=((u8)(Distance_Structure[row][i].Data&0x00ff));
			while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
			USART_SendData(UART5,Distance_Structure[row][i].Data>>8);		//
			check_sum+=((u8)(Distance_Structure[row][i].Data>>8));
		}
		temp>>=1;
	}
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
	USART_SendData(UART5,0x00);
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
	USART_SendData(UART5,check_sum);
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
	USART_SendData(UART5,0x0D);
}

void Distance_UploadAll(u32 config)
{
	u8 check_sum=0;
	u8 sensor_num=0;
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;	//
	USART_SendData(UART5,0xA5);
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;	//包头
	USART_SendData(UART5,0x5A);
	for(u32 i=0;i<24;++i)																			//检查数量
	{
		if((config>>i)&0x01)
			++sensor_num;
	}
	check_sum+=sensor_num;
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;	//
	USART_SendData(UART5,sensor_num);
	for(u8 m=0;m<6;++m)
	{
		for(u8 n=0;n<4;++n)
		{
			if((config>>(m*4+n))&0x1)
			{
				while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
				USART_SendData(UART5,4*m+n);											//
				check_sum+=(4*m+n);
				while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
				USART_SendData(UART5,Distance_Structure[m][n].Data&0x00ff);//
				check_sum+=((u8)(Distance_Structure[m][n].Data&0x00ff));
				while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
				USART_SendData(UART5,Distance_Structure[m][n].Data>>8);		//
				check_sum+=((u8)(Distance_Structure[m][n].Data>>8));
			}
		}
	}
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
	USART_SendData(UART5,0x00);
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
	USART_SendData(UART5,check_sum);
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET) ;
	USART_SendData(UART5,0x0D);
}

//
//mode: 	0x70 第一级降噪，出厂默认设置，适用于电池供电
//				0x71 第二级降噪，适用于 USB 供电
//				0x72 第三级降噪，适用于较长距离 USB 供电
//				0x73 第四级降噪，适用于开关电源供电
//				0x74 第五级降噪，适用于噪音较大的开关电源供电
//				0x75 第六级降噪，适用于高噪音电源供电
int Distance_SetPowerMode(u8 addr,u8 Mode)
{
//	Distance_SendByte(addr);
//	Distance_SendByte(0x02);
//	Distance_SendByte(Mode);
	return 0;
}

//
//mode:	0xa0 光强探测指令
//			0xb0 0-5m 范围，普通距离(不带温度补偿)，返回 mm,探测最大耗时约 33ms
//			0xb2 0-5m 范围，普通距离(不带温度补偿)，返回μs，探测最大耗时约 32ms
//			0xb4 0-5m 范围，普通距离(带温度补偿)，返回 mm,探测最大耗时约 87ms(KS103S 不支持此命令)
//			0xb8 0-11m 范围，普通距离(不带温度补偿)，返回 mm,探测最大耗时约 68ms
//			0xba 0-11m 范围，普通距离(不带温度补偿)，返回μs,探测最大耗时约 66ms
//			0xbc 0-11m 范围，普通距离(带温度补偿)，返回 mm,探测最大耗时约 87ms(KS103S 不支持此命令)
//			0xbd 12cm-11m 范围，0-10 米量程专用指令。普通距离(不带温度补偿)，返回 mm,探测最大耗时约 68ms
//			0xbe 12cm-11m 范围，0-10 米量程专用指令。普通距离(不带温度补偿)，返回μs,探测最大耗时约 66ms
//			0xbf 12cm-11m 范围，0-10 米量程专用指令。普通距离(带温度补偿)，返回 mm,探测最大耗时约87ms(KS103S 不支持此命令)
int COM1_GetData(u8 addr,u8 mode,u16 *buffer)
{
	Sensor_PwrCtrl(4);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
	USART_SendData(USART1,addr);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
	USART_SendData(USART1,2);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET) ;
	USART_SendData(USART1,mode);
	USART1_Buffer=buffer;
	isData1[0]=0;
	return 0;
}

int Distance_TestConection(Distance_TypeDef* Distance_Structure)
{
	if(Distance_Structure->USARTx==USART1)
	{
		//while(USART1_Status==Busy) ;
		Distance_Structure->Data=0xffff;
		USART1_Buffer=&Distance_Structure->Data;
		Distance_SendByte(USART1,Distance_Structure->Addr);
		Distance_SendByte(USART1,0x02);
		Distance_SendByte(USART1,0xa0);
		delay_ms(10);
		if(Distance_Structure->Data>0x400)
		{
			Distance_Structure->Data=0xffff;
			return 1;
		}
	}else if(Distance_Structure->USARTx==USART2)
	{
		//while(USART2_Status==Busy) ;
		Distance_Structure->Data=0xffff;
		USART2_Buffer=&Distance_Structure->Data;
		Distance_SendByte(USART2,Distance_Structure->Addr);
		Distance_SendByte(USART2,0x02);
		Distance_SendByte(USART2,0xa0);
		delay_ms(10);
		if(Distance_Structure->Data>0x400)
		{
			Distance_Structure->Data=0xffff;
			return 1;
		}
	}else if(Distance_Structure->USARTx==USART3)
	{
		//while(USART3_Status==Busy) ;
		Distance_Structure->Data=0xffff;
		USART3_Buffer=&Distance_Structure->Data;
		Distance_SendByte(USART3,Distance_Structure->Addr);
		Distance_SendByte(USART3,0x02);
		Distance_SendByte(USART3,0xa0);
		delay_ms(10);
		if(Distance_Structure->Data>0x400)
		{
			Distance_Structure->Data=0xffff;
			return 1;
		}
	}else if(Distance_Structure->USARTx==UART4)
	{
		//while(USART4_Status==Busy) ;
		Distance_Structure->Data=0xffff;
		USART4_Buffer=&Distance_Structure->Data;
		Distance_SendByte(UART4,Distance_Structure->Addr);
		Distance_SendByte(UART4,0x02);
		Distance_SendByte(UART4,0xa0);
		delay_ms(10);
		if(Distance_Structure->Data>0x400)
		{
			Distance_Structure->Data=0xffff;
			return 1;
		}
	}
	return 0;
}

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		if(isData1[0]==0)
		{
			(*USART1_Buffer)&=0x00FF;
			(*USART1_Buffer)|=(USART_ReceiveData(USART1)<<8);
			++isData1[0];
		}
		else
		{
			(*USART1_Buffer)&=0xFF00;
			(*USART1_Buffer)|=(USART_ReceiveData(USART1));
			isData1[0]=0;
			USART1_Status=Complete;
		}
	}
}

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE)==SET)
	{
		if(isData1[1]==0)
		{
			(*USART2_Buffer)&=0x00FF;
			(*USART2_Buffer)|=(USART_ReceiveData(USART2)<<8);
			++isData1[1];
		}
		else
		{
			(*USART2_Buffer)&=0xFF00;
			(*USART2_Buffer)|=USART_ReceiveData(USART2);
			isData1[1]=0;
			USART2_Status=Complete;
		}
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
	{
		if(isData1[2]==0)
		{
			(*USART3_Buffer)&=0x00FF;
			(*USART3_Buffer)|=(USART_ReceiveData(USART3)<<8);
			++isData1[2];
		}
		else
		{
			(*USART3_Buffer)&=0xFF00;
			(*USART3_Buffer)|=(USART_ReceiveData(USART3));
			isData1[2]=0;
			USART3_Status=Complete;
		}
	}
}

void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
	{
		if(isData1[3]==0)
		{
			(*USART4_Buffer)&=0x00FF;
			(*USART4_Buffer)|=(USART_ReceiveData(UART4)<<8);
			++isData1[3];
		}
		else
		{
			(*USART4_Buffer)&=0xFF00;
			(*USART4_Buffer)|=(USART_ReceiveData(UART4));
			isData1[3]=0;
			USART4_Status=Complete;
		}
	}
}

void UART5_IRQHandler(void)
{
	static u8 step=0;
	static u8 Check_Sum=0;
	static u8 CMD=0;
	if(USART_GetITStatus(UART5,USART_IT_RXNE)==SET)
	{
		switch(step)
		{
			case 0:
				if(USART_ReceiveData(UART5)==0xA5)
					++step;break;
			case 1:
				if(USART_ReceiveData(UART5)==0x5A)
					++step;
				else
					step=0;
				break;
			case 2:
				if(USART_ReceiveData(UART5)==0x01)
				{
					Check_Sum+=1;
					++step;
				}
				else
				{
					step=0;
				}
				break;
			case 3:
				CMD=USART_ReceiveData(UART5);
				Check_Sum+=CMD;
				++step;
				break;
			case 4:
				if(USART_ReceiveData(UART5)==Check_Sum)
				{
					Config_CMD=CMD;
				}
				CMD=0;
				step=0;
				Check_Sum=0;
				break;
			default:break;
		}
	}
}

