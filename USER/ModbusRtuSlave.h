#ifndef MODBUS_RTU_SLV_H
#define MODBUS_RTU_SLV_H

#include "mbproto.h"
#include <stdint.h>
#include "CharDev.h"
#include "mbcrc.h"
#include "ringque.h"

class CModbusCharDev
	:public CCharDev
{
	public:
		CModbusCharDev(uint32_t break_period);
		virtual int open();
		virtual int close() {return 0;}
	
		virtual int write(const uint8_t*, uint32_t);
		virtual int read(uint8_t*, uint32_t);
	
		virtual uint32_t data_in_write_buf();
		virtual uint32_t freesize_in_write_buf();
		virtual uint32_t data_in_read_buf();
		virtual void clear_read_buf();
		
		virtual void runTransmitter(){};
		virtual void runReceiver(){};
			
		ringque<uint8_t, 40> txQue_;
		ringque<uint8_t, 20> rxQue_;
		
};

class CModbusRtuSlave
{
	public:
		CModbusRtuSlave();
		void run();
	
#include "ObjectDict.h"	
		
		uint16_t& inputReg(uint16_t idx) {return inputReg_[idx];}
		uint16_t holdingReg(uint16_t idx) {return holdingReg_[idx];}
	
	private:
		bool isFirstIn_;
		uint8_t errCnt_;
		CModbusCharDev charDev_;
		
		int decode(uint8_t& func, uint16_t& addr, uint16_t& data);
		int execute(uint8_t func, uint16_t addr, uint16_t data);
		void reply();
		
		void printWorkBuf()
		{
		}
		enum
		{
			WORK_BUF_LEN = 25
		};
		
	private:
		static uint16_t inputReg_[INPUT_REG_NUM];
		static uint16_t holdingReg_[HOLDING_REG_NUM];
		static uint8_t workBuf_[WORK_BUF_LEN];
		uint16_t workBufMsgLen_;
};	

typedef NormalSingleton<CModbusRtuSlave>	ModbusSlave;

#endif
//end of files
