#include "timer.h"
#include "puart.h"
#include "task.h"
#include "pwr_ctrl.h"
	
int main(void)
{
	BaseTimer::Instance()->initialize();
	pwr_init();
	puart_init();
	while(1)
	{
		led_do_run();
		sonnic_do_run();
		report_do_run();
		key_scan_do_run();
	}

}


