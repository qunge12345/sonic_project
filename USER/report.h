#ifndef __REPORT_H__
#define __REPORT_H__
#include "stm32f10x.h"
#include "stdint.h"

void uart_init();
void uart_put_char(char c);
void uart_send_buf(char * s,uint16_t length);
void uart_printf(const char *fmt, ...);
void uart_clear_buf();
uint16_t uart_get_buf_size();
void uart_read_buf(char * s,uint16_t length);

#endif

