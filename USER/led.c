#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
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

void led_write(uint16_t data)
{
	data = (~data)&0x0f;								//1:灭 0:亮
	LED_PORT->ODR &= (~LED_ALL);
	LED_PORT->ODR |= (data << 6);
}

