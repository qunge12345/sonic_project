#ifndef MODBUS_RTU_SLV_H
#define MODBUS_RTU_SLV_H

#include "mbproto.h"
#include <stdint.h>
#include "CharDev.h"
#include "mbcrc.h"
#include "ringque.h"
#include "fixed_vector.h"
#include "Console.h"

class CModbusRtuSlave
{
	public:
		CModbusRtuSlave();
		void run();
		void irqRun();
	
		enum
		{
			WORK_BUF_LEN = 100
		};
	
#include "ObjectDict.h"	
		
		uint16_t& inputReg(uint16_t idx) {return inputReg_[idx];}
		uint16_t holdingReg(uint16_t idx) {return holdingReg_[idx];}
	
	private:
		void Init();
		bool isFirstIn_;
		uint8_t errCnt_;
		
		int decode(uint8_t& func, uint16_t& addr, uint16_t& data);
		int execute(uint8_t func, uint16_t addr, uint16_t data);
		void executeErr(uint8_t func, uint8_t errcode);
		void reply();
		
		void printWorkBuf()
		{
			for(int i = 0; i < workBuf_.size(); i++)
			{
				Console::Instance()->printf("0x%02X ", workBuf_.at(i));
			}
			Console::Instance()->printf("\r\n");
		}
		
		
	private:
		static uint16_t inputReg_[INPUT_REG_NUM];
		static uint16_t holdingReg_[HOLDING_REG_NUM];
		bool dataflowBreak_;
		static ringque<uint8_t, WORK_BUF_LEN> txQue_;
		static fixed_vector<uint8_t, WORK_BUF_LEN> workBuf_;
//		ringque<uint8_t, 20> rxQue_;
};	

typedef NormalSingleton<CModbusRtuSlave>	ModbusSlave;

#endif
//end of files
