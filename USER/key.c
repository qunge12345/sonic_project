#include "key.h"
#include "timer.h"

struct
{
	bool _is_released;
	uint32_t _press_period;
	uint32_t _press_time;
	uint32_t _release_time;
}key_struct[2];

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStructure);
}

bool key_is_pressed(uint8_t key_num)
{
	if(key_struct[key_num]._is_released == false)
		return false;
	if((key_struct[key_num]._release_time - key_struct[key_num]._press_time) >20)
	{
		return true;
	}
	return false;
}

#ifdef __cplusplus
 extern "C" {
#endif

void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==RESET)
		{
			key_struct[1]._press_time = BaseTimer::Instance()->getTime();
			key_struct[1]._is_released = false;
		}
		else
		{
			key_struct[1]._release_time = BaseTimer::Instance()->getTime();
			key_struct[1]._is_released = true;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET)
		{
			key_struct[0]._press_time = BaseTimer::Instance()->getTime();
			key_struct[0]._is_released = false;
		}
		else
		{
			key_struct[0]._release_time = BaseTimer::Instance()->getTime();
			key_struct[0]._is_released = true;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

#ifdef __cplusplus
}
#endif

