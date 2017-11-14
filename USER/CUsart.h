#ifndef _FW_CUSART_H
#define _FW_CUSART_H
#include "stdint.h"
#include "stm32f10x.h"

class CUsart
{
public:		
	CUsart(USART_TypeDef *,
		uint8_t* rxBuff,
		uint16_t rxBuffSize,
		uint32_t BaudRate = 115200);
	~CUsart();

	uint8_t send_Array(uint8_t*, uint8_t);
	uint8_t read_RxFifo(uint8_t*);
	uint8_t get_BytesInTxFifo();
	uint8_t get_BytesInRxFifo();
	void clear_rxFifo();

	void InitSciGpio();
	void InitSci();
	void setBaudrate(uint32_t);
	uint8_t getRxOverflowCount();
	
	enum IOGroup_Type
	{
		GROUP_A2A3 = 0,
		GROUP_A9A10,
		GROUP_B10B11,
		GROUP_C12D2,
		GROUP_C10C11,
		GROUP_C6C7
	};
	static DMA_Channel_TypeDef * RxDMA(USART_TypeDef *);
	static DMA_Channel_TypeDef * TxDMA(USART_TypeDef *);
	static IOGroup_Type BspIOGroup(USART_TypeDef *); 
	
private:
	USART_TypeDef * USARTx_;
	IOGroup_Type IOGroup_;
	DMA_Channel_TypeDef * DmaTxChannel_;
	DMA_Channel_TypeDef * DmaRxChannel_;
	uint32_t baudRate_;
	uint8_t rxOverflowCount_;
	uint8_t *rxBuff_;
	uint16_t rxBuffSize_;
	
};

#endif
//end of file
