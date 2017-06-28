#ifndef __UART_H__
#define __UART_H__
#include "stm32f10x.h"
#include "stdint.h"

void uart_init();
void uart_printf(const char *fmt, ...);


#endif


