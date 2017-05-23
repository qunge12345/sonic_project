#include "My_Tim.h"
void Tim6_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	TIM6->ARR=0xffff;
	TIM6->PSC=7199;
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Tim6_Start(void)
{
	TIM6->CNT=0;
	//TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6,ENABLE);
}

u16 Tim6_Stop(void)
{
	TIM_Cmd(TIM6,DISABLE);
	return TIM6->CNT;
}

void Tim7_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	TIM7->ARR=0xffff;
	TIM7->PSC=35999;
}

void Tim7_Start(void)
{
	TIM7->CNT=0;
	//TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM7,ENABLE);
}

u16 Tim7_Stop(void)
{
	TIM_Cmd(TIM7,DISABLE);
	return TIM7->CNT;
}


