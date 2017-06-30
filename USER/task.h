#ifndef __TASK_H__
#define __TASK_H__
#include "stdint.h"

extern uint32_t task_freq;

void led_do_run();
void sonnic_do_run();
void report_do_run();
void key_scan_do_run();


#endif

