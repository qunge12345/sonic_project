#ifndef __KEY__H
#define __KEY__H

#include "stm32f10x.h"
#include "stdint.h"

void Key_Init(void);
bool key_is_pressed(uint8_t key_num);

#endif



