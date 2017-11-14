#include "ModbusRtuSlave.h"
#include <stdint.h>
#include "mbcrc.h"
#include "ringque.h"

#define MODBUS_UART USART1
#define DEFAULT_SLAVE_ID	1

const uint8_t RECV_BUFF_SIZE = 30;
static uint8_t recvBuf[RECV_BUFF_SIZE];

uint16_t CModbusRtuSlave::inputReg_[INPUT_REG_NUM];
uint16_t  CModbusRtuSlave::holdingReg_[HOLDING_REG_NUM];
uint8_t CModbusRtuSlave::workBuf_[WORK_BUF_LEN];

#define MODBUS_USART UART5

CModbusCharDev::CModbusCharDev(uint32_t break_period)
	:CCharDev(break_period)
{
}

/**
	* @brief  open device
	* @param  None
	* @retval 1: success
	*/
int CModbusCharDev::open()
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
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	else
		while(1);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ITConfig(MODBUS_USART,USART_IT_TXE,ENABLE);
	USART_ITConfig(MODBUS_USART,USART_IT_RXNE,ENABLE);

	USART_Cmd(MODBUS_USART, DISABLE);
	USART_Init(MODBUS_USART, &USART_InitStructure);
	USART_Cmd(MODBUS_USART, ENABLE);

	return 1;
}

#ifdef __cplusplus
 extern "C" {
#endif
void UART5_IRQHandler(void)
{
	if(USART_GetITStatus(UART5,USART_IT_RXNE) == SET)
	{

	}
	else if(USART_GetITStatus(UART5,USART_IT_TXE) == SET)
	{

	}

}
#ifdef __cplusplus
 }
#endif
/**
	* @brief  write
	* @param  None
	* @retval None
	*/
int CModbusCharDev::write(const uint8_t* buff, uint32_t len)
{
	if(data_in_write_buf() > 0)
	{
		return -1;
	}

	return txQue_.push_array((uint8_t*)buff, len);
}

/**
	* @brief  read
	* @param  buff:
	* @param  len:
	* @retval bytes that actually get
	*/
int CModbusCharDev::read(uint8_t* buff, uint32_t len)
{
	return rxQue_.push_array(buff, len);
}

/**
	* @brief  data_in_write_buf
	* @param  None
	* @retval None
	*/
uint32_t CModbusCharDev::data_in_write_buf()
{
	return txQue_.elemsInQue();
}

/**
	* @brief  freesize_in_write_buf
	* @param  None
	* @retval None
	*/
uint32_t CModbusCharDev::freesize_in_write_buf()
{
	return txQue_.emptyElemsInQue();
}

/**
	* @brief  data_in_read_buf
	* @param  None
	* @retval None
	*/
uint32_t CModbusCharDev::data_in_read_buf()
{
	return rxQue_.elemsInQue();
}

/**
	* @brief  clear_read_buf
	* @param  None
	* @retval None
	*/
void CModbusCharDev::clear_read_buf()
{
	rxQue_.clear();
}

/**
	* @brief  Constructor of ModbusSlave
	* @param  None
	*/
CModbusRtuSlave::CModbusRtuSlave()
	:isFirstIn_(true),
	errCnt_(0),
	charDev_(5)
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
int CModbusRtuSlave::decode(uint8_t& funCode, uint16_t& addr, uint16_t& data)
{
	if(workBuf_[0]!=  CModbusRtuSlave::SLAVE_ID)
	{
		//Console::Instance()->printf("slaver id%d unmatch", workBuf_[0]);
		return -1;
	}

	if(workBufMsgLen_ < 3)
	{
		//Console::Instance()->printf("Message less than 3 bytes\r\n");
		return -1;
	}

	uint16_t crcResult = ((uint16_t)workBuf_[workBufMsgLen_ - 1] << 8) + workBuf_[workBufMsgLen_ - 2];
	if(usMBCRC16(workBuf_, workBufMsgLen_ - 2) != crcResult)
	{
		//Console::Instance()->printf("CRC failed,should be 0x%X, get 0x%X\r\n", usMBCRC16(workBuf_.begin(), workBufMsgLen_ - 2), crcResult);
		return -1;
	}

	funCode = workBuf_[1];
	addr = ((uint16_t)workBuf_[2] << 8) + workBuf_[3];
	data = ((uint16_t)workBuf_[4] << 8) + workBuf_[5];
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
		(MB_FUNC_READ_INPUT_REGISTER == funCode) ? (pReg = (uint16_t*)&inputReg_) : (pReg = (uint16_t*)&holdingReg_);
		if(addr + dataNum > INPUT_REG_NUM)
		{
			//Console::Instance()->printf("invalid address\r\n");
			return -1;
		}

		workBuf_[0] = CModbusRtuSlave::SLAVE_ID;
		workBuf_[1] = funCode;
		workBuf_[2] = 2 * dataNum;//2bytes per register

		workBufMsgLen_ = 3 + 2 * dataNum + 2;

		for(int i = 0; i < dataNum; i++)
		{
			workBuf_[3 + 2*i] = pReg[i] >> 8;
			workBuf_[3 + 2*i + 1] = pReg[i] & 0xFF;
		}
		uint16_t crcResult = usMBCRC16(workBuf_, workBufMsgLen_ - 2);

		workBuf_[workBufMsgLen_ - 2] = crcResult & 0xFF;
		workBuf_[workBufMsgLen_ - 1] = crcResult >> 8;

		return 0;
	}
	else if(MB_FUNC_WRITE_REGISTER == funCode)
	{
		if(addr >= HOLDING_REG_NUM)
		{
			//Console::Instance()->printf("invalid address\r\n");
			return -1;
		}

		holdingReg_[addr] = data;

		// do command logic
		if (COMMAND_RESET == addr)
		{
			NVIC_SystemReset();
		}
		else if (COMMAND_IAP == addr)
		{
			// todo
		}
		return 0;
	}

	return -1;
}

/**
	* @brief  reply message to modbus host
	* @param  None
	* @retval None
	*/
void CModbusRtuSlave::reply()
{
	charDev_.write(workBuf_, workBufMsgLen_);
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
		charDev_.open();
		isFirstIn_ = false;
	}

	charDev_.update_data_break_flag();

	if(charDev_.is_dataflow_break())
	{
		uint8_t funCode;
		uint16_t addr;
		uint16_t data;
		workBufMsgLen_ = charDev_.read(workBuf_, charDev_.data_in_read_buf());
		if(decode(funCode, addr, data) != 0)
		{
			printWorkBuf();
			return;
		}

		if(execute(funCode, addr, data) != 0)
		{
			printWorkBuf();
			return;
		}
		charDev_.clear_read_buf();
		reply();
	}
}

//end of file
