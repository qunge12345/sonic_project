#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"
#include "io_bitband.h"

#define LED 											PA06out
#define LED_On										1		
#define LED_Off										0	

#define LED_PORT									GPIOC
#define LED_ALL										0x3C0			//pin6-9
#define LED_ROW0									PC06out
#define LED_ROW1									PC07out
#define LED_ROW2									PC08out
#define LED_ROW3									PC09out

void LED_Init(void);
void LED_Toggle(void);
void LED_ON(u8 num);
void LED_OFF(u8 num);
void led_write(uint16_t data);

#endif
