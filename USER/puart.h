#ifndef __PUART_H__
#define __PUART_H__
#include "stm32f10x.h"

void puart_init();
void puart_send_byte(char c);
void puart_buf_reset();
void puart_read_buf(uint8_t bufnum,char *buf,uint8_t length);
uint8_t puart_get_buf_count(uint8_t bufnum);
void puart_buf_reset(uint8_t bufnum);
void puart_send_buf(char *buf,uint8_t length);

#endif


