#include "sonic.h"
#include "pwr_ctrl.h"
#include "timer.h"
#include "puart.h"
#include "ObjectDict.h"

uint8_t valid_addr[]={0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
	0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf8,0xfa,0xfc,0xfe};

CSonic::CSonic(uint8_t addr)
{
	_addr = addr;
	_timestamp = 0;
	for(int i = 0; i < 4; ++i)
	{
		_data[i] = KS103_SONIC_OFF;
		_timeoutCount[i] = 0;
	}
//	reset_id();
}

CSonic::~CSonic()
{
}

void CSonic::reset_id()
{
	pwr_off(PWRALL);
	BaseTimer::Instance()->delay_ms(100);
	if(0xea == _addr){
		pwr_on(PWR0);
	}
	if(0xec == _addr){
		pwr_on(PWR1);
	}
	if(0xee == _addr){
		pwr_on(PWR2);
	}
	if(0xf8 == _addr){
		pwr_on(PWR3);
	}
	if(0xfa == _addr){
		pwr_on(PWR4);
	}
	if(0xfc == _addr){
		pwr_on(PWR5);
	}
	BaseTimer::Instance()->delay_ms(100);
	for(uint8_t i=0;i<20;++i)
	{
		puart_send_byte(valid_addr[i]);
		puart_send_byte(2);
		puart_send_byte(0x9a);
		BaseTimer::Instance()->delay_ms(5);						//延时，避免超声模块发出的回复数据干扰总线
		puart_send_byte(valid_addr[i]);
		puart_send_byte(2);
		puart_send_byte(0x92);
		BaseTimer::Instance()->delay_ms(5);
		puart_send_byte(valid_addr[i]);
		puart_send_byte(2);
		puart_send_byte(0x9e);
		BaseTimer::Instance()->delay_ms(5);
		puart_send_byte(valid_addr[i]);
		puart_send_byte(2);
		puart_send_byte(_addr);
		BaseTimer::Instance()->delay_ms(5);
	}
	BaseTimer::Instance()->delay_ms(100);
	pwr_off(PWRALL);
}

void CSonic::send_cmd()
{
	puart_send_byte(_addr);
	puart_send_byte(0x02);
	puart_send_byte(0xb0);
	_is_data_received = 0;
	puart_buf_reset(4);
}

void CSonic::read_data()
{
	if(puart_get_buf_count(0) > 1)
	{
		char tmp[2];
		puart_read_buf(0,tmp,2);
		_data[0] = ((uint16_t)tmp[0]<<8) + tmp[1];
		if(_data[0] > SONIC_MAX_LENGTH)
			_data[0] = KS103_SONIC_TIMEOUT;
		_is_data_received |= SONIC_ROW_0;
	}
	if(puart_get_buf_count(1) > 1)
	{
		char tmp[2];
		puart_read_buf(1,tmp,2);
		_data[1] = ((uint16_t)tmp[0]<<8) + tmp[1];
		if(_data[1] > SONIC_MAX_LENGTH)
			_data[1] = KS103_SONIC_TIMEOUT;
		_is_data_received |= SONIC_ROW_1;
	}
	if(puart_get_buf_count(2) > 1)
	{
		char tmp[2];
		puart_read_buf(2,tmp,2);
		_data[2] = ((uint16_t)tmp[0]<<8) + tmp[1];
		if(_data[2] > SONIC_MAX_LENGTH)
			_data[2] = KS103_SONIC_TIMEOUT;
		_is_data_received |= SONIC_ROW_2;
	}
	if(puart_get_buf_count(3) > 1)
	{
		char tmp[2];
		puart_read_buf(3,tmp,2);
		_data[3] = ((uint16_t)tmp[0]<<8) + tmp[1];
		if(_data[3] > SONIC_MAX_LENGTH)
			_data[3] = KS103_SONIC_TIMEOUT;
		_is_data_received |= SONIC_ROW_3;
	}
}

void CSonic::detect()
{
	puart_send_byte(_addr);
	puart_send_byte(0x02);
	puart_send_byte(0xa0);									//光强检测命令
	puart_buf_reset(4);
	BaseTimer::Instance()->delay_ms(10);
	read_data();
	
	if(SONIC_DETECT_RECV == _data[0]){			//数据校验
		_is_valid |= SONIC_ROW_0;
	}else{
		_is_valid &= (~SONIC_ROW_0);
	}
	if(SONIC_DETECT_RECV == _data[1]){
		_is_valid |= SONIC_ROW_1;
	}else{
		_is_valid &= (~SONIC_ROW_1);
	}
	if(SONIC_DETECT_RECV == _data[2]){
		_is_valid |= SONIC_ROW_2;
	}else{
		_is_valid &= (~SONIC_ROW_2);
	}
	if(SONIC_DETECT_RECV == _data[3]){
		_is_valid |= SONIC_ROW_3;
	}else{
		_is_valid &= (~SONIC_ROW_3);
	}
}

void CSonic::check_offline()
{
	for(int i = 0; i < 4; ++i)
	{
		uint8_t mask = 1u << i;
		if(is_data_received_by_mask(mask) || (false == is_single_valid(mask)))
		{
			_timeoutCount[i] = 0;
		}
		else
		{
			if(_timeoutCount[i]++ >= 20)
			{
				_timeoutCount[i] = 20;
				_data[i] = KS103_SONIC_OFFLINE;
			}
		}
	}
}

CSonic Sonic[6] = {CSonic(0xea),CSonic(0xec),CSonic(0xee),CSonic(0xf8),CSonic(0xfa),CSonic(0xfc)};




