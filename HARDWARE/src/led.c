#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//LED驱动代码	   
//修改日期:2015/5/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PF6~10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED-->PB7 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);						 //

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;				 //LED-->PB7 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
}

void Sensor_PwrInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);
}

void Sensor_PwrCtrl(u8 num)
{
	switch(num)
	{
		case 0: PCout(0)=1;PCout(1)=0;PCout(2)=0;PCout(3)=0;PCout(4)=0;PCout(5)=0;break;
		case 1: PCout(0)=0;PCout(1)=1;PCout(2)=0;PCout(3)=0;PCout(4)=0;PCout(5)=0;break;
		case 2: PCout(0)=0;PCout(1)=0;PCout(2)=1;PCout(3)=0;PCout(4)=0;PCout(5)=0;break;
		case 3: PCout(0)=0;PCout(1)=0;PCout(2)=0;PCout(3)=1;PCout(4)=0;PCout(5)=0;break;
		case 4: PCout(0)=0;PCout(1)=0;PCout(2)=0;PCout(3)=0;PCout(4)=1;PCout(5)=0;break;
		case 5: PCout(0)=0;PCout(1)=0;PCout(2)=0;PCout(3)=0;PCout(4)=0;PCout(5)=1;break;
		case 6: PCout(0)=1;PCout(1)=1;PCout(2)=1;PCout(3)=1;PCout(4)=1;PCout(5)=1;break;
		case 7: PCout(0)=0;PCout(1)=0;PCout(2)=0;PCout(3)=0;PCout(4)=0;PCout(5)=0;break;
		default:break;
	}
}
 
void LED_Toggle(void)
{
	if(PAout(8))
		PAout(8)=0;
	else
		PAout(8)=1;
}

void LED_ON(u8 num)
{
	switch(num)
	{
		case 0: PCout(6)=0;break;
		case 1: PCout(7)=0;break;
		case 2: PCout(8)=0;break;
		case 3: PCout(9)=0;break;
		case 4: PAout(8)=0;break;
		default:break;
	}
}

void LED_OFF(u8 num)
{
	switch(num)
	{
		case 0: PCout(6)=1;break;
		case 1: PCout(7)=1;break;
		case 2: PCout(8)=1;break;
		case 3: PCout(9)=1;break;
		case 4: PAout(8)=1;break;
		default:break;
	}
}

