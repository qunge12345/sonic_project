#include "led.h"
#include "com_dbg.h"
#include "timer.h"
#include "uart.h"
#include "puart.h"
#include "task.h"
#include "pwr_ctrl.h"
#include "sonic.h"
	
int main(void)
{
	Timer HeartBeat(500,500);
	BaseTimer::Instance()->initialize();
	pwr_init();
	uart_init();
	LED_Init();
	puart_init();
	while(1)
	{
		sonnic_do_run();
		led_do_run();
		report_do_run();
		if(HeartBeat.isAbsoluteTimeUp())
		{
			uart_printf("data:%d\r\n",Sonic[0].get_data(3));
		}
	}

}


