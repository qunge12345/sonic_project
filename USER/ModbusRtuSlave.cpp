#include "ModbusRtuSlave.h"
#include <stdint.h>
#include "mbcrc.h"
#include "ringque.h"
#include "Console.h"
#include "powerupOption.h"
#include "stmflash.h"

#define DEFAULT_SLAVE_ID	1

uint16_t CModbusRtuSlave::inputReg_[INPUT_REG_NUM];
uint16_t  CModbusRtuSlave::holdingReg_[HOLDING_REG_NUM];
ringque<uint8_t, 100> CModbusRtuSlave::txQue_;
fixed_vector<uint8_t, CModbusRtuSlave::WORK_BUF_LEN> CModbusRtuSlave::workBuf_;

#define MODBUS_USART UART5

/**
	* @brief  open device
	* @param  None
	* @retval 1: success
	*/
void CModbusRtuSlave::Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	uint32_t RCC_APB2Periph_GPIOx;
	uint8_t GPIO_PinSource_Tx;
	uint8_t GPIO_PinSource_Rx;
//	uint8_t GPIO_AF_USARTx;
	GPIO_TypeDef *GPIOx_Tx;
	GPIO_TypeDef *GPIOx_Rx;

	if(MODBUS_USART == UART5)
	{
		RCC_APB2Periph_GPIOx = RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC;
		GPIOx_Tx = GPIOC;
		GPIOx_Rx = GPIOD;
		GPIO_PinSource_Tx = GPIO_PinSource12;
		GPIO_PinSource_Rx = GPIO_PinSource2;

		/* open clock of GPIO */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);

		/* Config Pin: TXD RXD*/
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0<< GPIO_PinSource_Tx;
		GPIO_Init(GPIOx_Tx, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0<< GPIO_PinSource_Rx;
		GPIO_Init(GPIOx_Rx, &GPIO_InitStructure);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	}
	else
	{
		while(1);
	}
	/* Deinitializes the USARTx */
	USART_DeInit(MODBUS_USART);

	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	NVIC_InitTypeDef NVIC_InitStructure;
	if(MODBUS_USART == UART5)
		NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	else
		while(1);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(MODBUS_USART,USART_IT_TC,ENABLE);
	USART_ITConfig(MODBUS_USART,USART_IT_RXNE,ENABLE);
	USART_ITConfig(MODBUS_USART,USART_IT_IDLE,ENABLE);

	USART_Cmd(MODBUS_USART, DISABLE);
	USART_Init(MODBUS_USART, &USART_InitStructure);
	USART_Cmd(MODBUS_USART, ENABLE);
}


extern "C" {

	void UART5_IRQHandler(void)
	{
		ModbusSlave::Instance()->irqRun();
	}

}

/**
	* @brief  Constructor of ModbusSlave
	* @param  None
	*/
CModbusRtuSlave::CModbusRtuSlave()
	:isFirstIn_(true),
	errCnt_(0)
{
	inputReg_[1] = 0x5544;
}

/**
	* @brief  decode modbus rtu message to function code, address and data
	* @param  funCode:	function code of rtu message
	* @param  addr: access address of message
	* @param  data: write data or read data number
	* @retval 0: success
	* @retval -1: faild
	*/
extern Timer is_cmd_time_up;
int CModbusRtuSlave::decode(uint8_t& funCode, uint16_t& addr, uint16_t& data)
{
	int ret = -1;
	do
	{
		if(workBuf_.at(0)!=  CModbusRtuSlave::SLAVE_ID)
		{
			Console::Instance()->printf("slaver id%d unmatch\r\n", workBuf_.at(0));
			break;
		}

		if(workBuf_.size() < 3)
		{
			Console::Instance()->printf("Message less than 3 bytes\r\n");
			break;
		}

		uint16_t crcResult = ((uint16_t)workBuf_.at(workBuf_.size() - 1) << 8) + workBuf_.at(workBuf_.size() - 2);
		if(usMBCRC16(workBuf_.begin(), workBuf_.size() - 2) != crcResult)
		{
			Console::Instance()->printf("CRC failed,should be 0x%X, get 0x%X\r\n", 
				usMBCRC16(workBuf_.begin(), workBuf_.size() - 2), 
				crcResult);
			
			Console::Instance()->printf("buff size = %d\r\n", workBuf_.size());
			
			break;
		}
		ret = 0;
		
	}while(0);
	
	if(ret < 0)
	{
		workBuf_.clear();
		return ret;
	}

	funCode = workBuf_.at(1);
	addr = ((uint16_t)workBuf_.at(2) << 8) + workBuf_.at(3);
	data = ((uint16_t)workBuf_.at(4) << 8) + workBuf_.at(5);
	return 0;
}

/**
	* @brief  execute command from modbus host
	* @param  funCode: function code of rtu message
	* @param  addr: access address of message
	* @param  data: write data or read data number
	* @retval 0: success
	* @retval -1: faild
	*/
int CModbusRtuSlave::execute(uint8_t funCode, uint16_t addr, uint16_t data)
{
	if(MB_FUNC_READ_INPUT_REGISTER == funCode  || MB_FUNC_READ_HOLDING_REGISTER == funCode)
	{
		uint16_t* pReg;
		uint16_t dataNum = data;
		uint16_t dataLimit = 0;
		(MB_FUNC_READ_INPUT_REGISTER == funCode) ? 
			(pReg = (uint16_t*)&inputReg_, dataLimit = INPUT_REG_NUM) : 
			(pReg = (uint16_t*)&holdingReg_, dataLimit = HOLDING_REG_NUM);
		
		if(addr + dataNum > dataLimit)
		{
			Console::Instance()->printf("access invalid address 0x%X, address limit = 0x%X\r\n", addr + dataNum, dataLimit);
			executeErr(funCode, MB_EX_ILLEGAL_DATA_ADDRESS);
			return -1;
		}

		workBuf_.at(0) = CModbusRtuSlave::SLAVE_ID;
		workBuf_.at(1) = funCode;
		workBuf_.at(2) = 2 * dataNum;//2bytes per register

		workBuf_.resize(3 + 2 * dataNum + 2);

		for(int i = 0; i < dataNum; i++)
		{
			workBuf_.at(3 + 2*i) = pReg[i + addr] >> 8;
			workBuf_.at(3 + 2*i + 1) = pReg[i + addr] & 0xFF;
		}
		uint16_t crcResult = usMBCRC16(workBuf_.begin() , workBuf_.size() - 2);

		workBuf_.at(workBuf_.size() - 2) = crcResult & 0xFF;
		workBuf_.at(workBuf_.size() - 1) = crcResult >> 8;

		return 0;
	}
	else if(MB_FUNC_WRITE_REGISTER == funCode)
	{
		if(addr >= HOLDING_REG_NUM)
		{
			Console::Instance()->printf("invalid address\r\n");
			executeErr(funCode, MB_EX_ILLEGAL_DATA_ADDRESS);
			return -1;
		}

		holdingReg_[addr] = data;

		// do command logic
		if (COMMAND_RESET == addr)
		{
			Console::Instance()->printf("Get reset command\r\n");
			txQue_.push_array(workBuf_.begin(), workBuf_.size());
			reply();
			while(txQue_.elemsInQue() > 0 || SET != USART_GetFlagStatus(MODBUS_USART, USART_FLAG_TXE));
			NVIC_SystemReset();
		}
		else if (COMMAND_IAP == addr)
		{
			Console::Instance()->printf("Get IAP command\r\n");
			txQue_.push_array(workBuf_.begin(), workBuf_.size());
			pvf::write(pvf::VAR_BOOT_OPTI, BOOT_PARAM_BL);
			reply();
			while(txQue_.elemsInQue() > 0 || SET != USART_GetFlagStatus(MODBUS_USART, USART_FLAG_TXE));
			NVIC_SystemReset();
		}
		else if (COMMAND_DURATION_MS == addr)
		{
			Console::Instance()->printf("Change sonic query duration\r\n");
			is_cmd_time_up = Timer(data, data);
		}
		return 0;
	}

	return -1;
}

void CModbusRtuSlave::executeErr(uint8_t func, uint8_t errcode)
{
	workBuf_.resize(3 + 2);
	workBuf_.at(0) = CModbusRtuSlave::SLAVE_ID;
	workBuf_.at(1) = func | 0x80;
	workBuf_.at(2) = errcode;

	uint16_t crcResult = usMBCRC16(workBuf_.begin() , workBuf_.size() - 2);

	workBuf_.at(workBuf_.size() - 2) = crcResult & 0xFF;
	workBuf_.at(workBuf_.size() - 1) = crcResult >> 8;
}

/**
	* @brief  reply message to modbus host
	* @param  None
	* @retval None
	*/
void CModbusRtuSlave::reply()
{
	txQue_.push_array(workBuf_.begin(), workBuf_.size());
	workBuf_.clear();
	USART_SendData(MODBUS_USART, txQue_.front());
	txQue_.pop();
}


/**
	* @brief  run modbus slaver for 1 time
	* @param  None
	* @retval None
	*/
void CModbusRtuSlave::run()
{
	if(isFirstIn_)
	{
		Init();
		isFirstIn_ = false;
	}

	if(dataflowBreak_)
	{
		dataflowBreak_ = false;
		uint8_t funCode;
		uint16_t addr;
		uint16_t data;
		if(decode(funCode, addr, data) != 0)
		{
			printWorkBuf();
			return;
		}

		uint32_t ts = TimeStamp::Instance()->getTS();
		inputReg(CModbusRtuSlave::TIMESTAMP_L) = (uint16_t)ts;
		inputReg(CModbusRtuSlave::TIMESTAMP_H) = (uint16_t)(ts >> 16);
		
		execute(funCode, addr, data);
		reply();
	}
}

/**
	* @brief  run modbus slaver for 1 time
	* @param  None
	* @retval None
	*/
void CModbusRtuSlave::irqRun()
{
	if(USART_GetITStatus(MODBUS_USART, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(MODBUS_USART, USART_IT_RXNE);
		workBuf_.push_back(MODBUS_USART->DR);
	}
	else if(USART_GetITStatus(MODBUS_USART, USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit(MODBUS_USART, USART_IT_TC);
		if(txQue_.elemsInQue() > 0)
		{
			USART_SendData(MODBUS_USART, txQue_.front());
			txQue_.pop();
		}
	}
	else if(USART_GetITStatus(MODBUS_USART, USART_IT_IDLE) == SET)
	{
		USART_ReceiveData(MODBUS_USART);
		dataflowBreak_ = true;
	}
}

//end of file
