#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//LED��������	   
//�޸�����:2016/2/19
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) 
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED PAout(6)// PB6
#define LED_On	1		//LED��
#define LED_Off	0		//LEDϨ��

void LED_Init(void);//��ʼ��
void LED_Toggle(void);
void LED_ON(u8 num);
void LED_OFF(u8 num);
void Sensor_PwrInit(void);
void Sensor_PwrCtrl(u8 num);

#endif
