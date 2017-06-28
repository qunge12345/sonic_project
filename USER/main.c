#include "led.h"
#include "com_dbg.h"
#include "timer.h"
#include "report.h"
#include "puart.h"
#include "task.h"
#include "pwr_ctrl.h"
#include "sonic.h"
#include "io_bitband.h"
	
int main(void)
{
	BaseTimer::Instance()->initialize();
	pwr_init();
	LED_Init();
	puart_init();
	while(1)
	{
		sonnic_do_run();
		led_do_run();
		report_do_run();
	}

}


