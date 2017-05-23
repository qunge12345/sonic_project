#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//LED驱动代码	   
//修改日期:2016/2/19
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED PAout(6)// PB6
#define LED_On	1		//LED打开
#define LED_Off	0		//LED熄灭

void LED_Init(void);//初始化
void LED_Toggle(void);
void LED_ON(u8 num);
void LED_OFF(u8 num);
void Sensor_PwrInit(void);
void Sensor_PwrCtrl(u8 num);

#endif
