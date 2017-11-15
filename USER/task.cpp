#include "task.h"
#include "led.h"
#include "timer.h"
#include "sonic.h"
#include "pwr_ctrl.h"
#include "report.h"
#include "key.h"
#include "ModbusRtuSlave.h"
#include "CommonConfig.h"

#define COLUME_NUM 2

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

		// initialize watch dog
//		InitWatchDog(2000);

		// sonic num
		uint16_t sensor_num = 0;
		for(uint16_t i = 0;i < 6;++i)
		{
			uint16_t row_status = 0;
			row_status = Sonic[i].get_status();
			if(row_status > 0)
			{
				for(uint16_t m = 0;m < COLUME_NUM;++m)
				{
					if(row_status & 0x01)
					{
						++sensor_num;
					}
					row_status >>= 1;
				}
			}
		}

		// initialize modbus slave
//		ModbusSlave::Instance()->run();
//		ModbusSlave::Instance()->inputReg(CModbusRtuSlave::VERSION_MAJOR) = 2;
//		ModbusSlave::Instance()->inputReg(CModbusRtuSlave::VERSION_MINOR) = 1;
//		ModbusSlave::Instance()->inputReg(CModbusRtuSlave::VERSION_FIX) = 0;
//		ModbusSlave::Instance()->inputReg(CModbusRtuSlave::TERMINAL_NUM) = sensor_num;
	}

	// read sonic data
	static Timer _is_cmd_time_up(50,100);
	static uint8_t current_col = 0;

	if(_is_cmd_time_up.isAbsoluteTimeUp())
	{
		++task_freq;

		// check timeout
		_valid_sonic[current_col-1]->check_offline();

		if(current_col >= _valid_col)
		{
				current_col = 0;
		}

		led_write(_valid_sonic[current_col]->get_status());
		_valid_sonic[current_col++]->send_cmd();

		for (size_t i = 0; i < 6; ++i)
		{
			for (size_t j = 0; j < COLUME_NUM; j++)
			{
				ModbusSlave::Instance()->inputReg(i * 2  + j) = Sonic[i].get_data(j);
			}
		}
	}

	// read every frame
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


