#include "task.h"

#include "led.h"
#include "timer.h"
#include "sonic.h"
#include "pwr_ctrl.h"
#include "uart.h"

void sonnic_do_run()
{
	static bool _is_first_in = true;
	static uint16_t _valid_col = 0;
	static CSonic* _valid_sonic[6];
	
	if(true == _is_first_in)
	{
		_is_first_in = false;
		
		for(uint16_t i = 0;i<6;++i)									//复位ID
		{
			Sonic[i].reset_id();
		}
		
		pwr_on(PWRALL);
		BaseTimer::Instance()->delay_ms(100);				//开启所有超声电源
		
		for(uint16_t i = 0;i<6;++i)									//检测超声
		{
			Sonic[i].detect();
			if(Sonic[i].get_status() > 0)
			{
				_valid_sonic[_valid_col] = &Sonic[i];
				++_valid_col;
			}
		}
	}
	
	static Timer _is_cmd_time_up(50,50);
	static uint8_t current_col = 0;
	if(_is_cmd_time_up.isAbsoluteTimeUp())
	{
		if(current_col < _valid_col)
		{
			_valid_sonic[current_col]->send_cmd();
			++current_col;
		}else{
			current_col =0;
		}
	}
	_valid_sonic[current_col-1]->read_data();
}

void led_do_run()
{
	static bool _is_first_in = true;
	if(true == _is_first_in)
	{
		LED_Init();
		_is_first_in = false;
	}
	static Timer heart_beat(500,500);
	if(heart_beat.isAbsoluteTimeUp())
	{
		LED_Toggle();
	}
}

void report_do_run()
{
	static Timer _is_time_up(50,50);
	static bool _is_first_in = true;
		
	if(_is_first_in == true)
	{
//		uart_init();
		_is_first_in = false;
	}
	
	if(_is_time_up.isAbsoluteTimeUp())
	{
		
	}
}


