#ifndef __PWR_CTRL_H__
#define __PWR_CTRL_H__
#include "stm32f10x.h"
#include "stdint.h"

#define PWR0					GPIO_Pin_0
#define PWR1					GPIO_Pin_1
#define PWR2					GPIO_Pin_2
#define PWR3					GPIO_Pin_3
#define PWR4					GPIO_Pin_4
#define PWR5					GPIO_Pin_5
#define PWRALL				(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5)

void pwr_init();
void pwr_on(uint16_t num);
void pwr_off(uint16_t num);

#endif


