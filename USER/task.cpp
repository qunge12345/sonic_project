#include "task.h"
#include "led.h"
#include "timer.h"
#include "sonic.h"
#include "pwr_ctrl.h"
#include "report.h"
#include "key.h"

const char reset_cmd[] = {0xa5,0x5a,0x01,0x01,0x02,0xa5,0x5a};

uint32_t task_freq = 0;

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
		//显示有效超声
		pwr_off(PWRALL);
		for(uint16_t i = 0;i<6;++i)
		{
			pwr_on(1<<i);
			led_write(Sonic[i].get_status());
			BaseTimer::Instance()->delay_ms(1000);
			pwr_off(PWRALL);
		}
		pwr_on(PWRALL);
	}
	
	static Timer _is_cmd_time_up(50,50);
	static uint8_t current_col = 0;
	static bool _is_data_received = false;
	if((_is_cmd_time_up.isAbsoluteTimeUp()) | (_is_data_received == true))
	{
		++task_freq;
		_is_cmd_time_up.reset();
		if(current_col < _valid_col)
		{
			led_write(_valid_sonic[current_col]->get_status());
			_valid_sonic[current_col]->send_cmd();
			++current_col;
		}else{
			current_col =0;
		}
	}
	_valid_sonic[current_col-1]->read_data();
	_is_data_received = _valid_sonic[current_col-1]->is_data_received();
	
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
	static Timer _is_1s_time_up(1000,1000);
	static bool _is_first_in = true;
		
	if(_is_first_in == true)
	{
		uart_init();
		_is_first_in = false;
	}
	
	if(_is_time_up.isAbsoluteTimeUp())
	{
		uint16_t sensor_num = 0;
		char data_buf[72];
		uint16_t buf_count = 0;
		for(uint16_t i = 0;i < 6;++i)
		{
			uint16_t row_status = 0;
			row_status = Sonic[i].get_status();
			if(row_status >0)
			{
				for(uint16_t m = 0;m < 4;++m)
				{
					if(row_status & 0x01)
					{
						++sensor_num;
						data_buf[buf_count++] = i*4 + m;											//探头编号
						data_buf[buf_count++] = Sonic[i].get_data(m);					//数据低八位
						data_buf[buf_count++] = (Sonic[i].get_data(m)>>8);		//数据高八位
					}
					row_status >>= 1;
				}
			}
		}
		//包头
		uart_put_char(0xa5);
		uart_put_char(0x5a);
		
		uint8_t check_sum = sensor_num;
		for(uint16_t i = 0;i < buf_count;++i)
		{
			check_sum += data_buf[i];
		}
		//有效传感器数量
		uart_put_char(sensor_num);
		//数据包
		uart_send_buf(data_buf, buf_count);
		//数据尾
		uart_put_char(0x00);
		//校验值
		uart_put_char(check_sum);
		//包尾
		uart_put_char(0x0D);
	}
	
	if(_is_1s_time_up.isAbsoluteTimeUp())
	{
//		uart_printf("\r\ntask freq:%d \r\n",task_freq);
		task_freq = 0;
	}
	
	if(uart_get_buf_size() > 6)
	{
		char cmd[7];
		uart_read_buf(cmd,7);
		uart_clear_buf();
		for(uint16_t i = 0;i<7;++i)
		{
			if(cmd[i] != reset_cmd[i])
				return;
		}
		NVIC_SystemReset();
	}
}

void key_scan_do_run()
{
	static bool _is_first_in = true;
	if(_is_first_in == true)
	{
		_is_first_in = false;
		Key_Init();
	}
	if(key_is_pressed(0) == true)
	{
		NVIC_SystemReset();
	}
}


