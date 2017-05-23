#ifndef __DISTANCE_H
#define __DISTANCE_H
#include "stm32f10x.h"

enum USART_Status
{
	Busy,
	Complete,
};

typedef struct
{
	USART_TypeDef* USARTx;
	u8 Addr;
	u8 ConectionStatus;
	u16 Data;
}Distance_TypeDef;

extern Distance_TypeDef Distance_Structure[6][4];
extern u8 Config_CMD;

int Distance_Init(void);
int Distance_Init232(void);
int Distance_SendBuffer232(u8 length,u16 *buffer);
void Distance_232SendByte(u8 data);
void Distance_232SendDword(u32 data);
int Distance_SendByte(USART_TypeDef* USARTx,unsigned char data);
int Distance_SetPowerMode(u8 addr,u8 Mode);
int COM1_GetData(u8 addr,u8 mode,u16 *buffer);
int Distance_SetAddr(USART_TypeDef* USARTx,u8 addr_old,unsigned char addr_new);
void Distance_DefaultAddr(u8 row);
u8 Distance_ShowConnectionStatus(u8 row);
u32 Distance_Test(void);
void Distance_Update(u8 row);
void Distance_Upload(u8 row,u32 config);
void Distance_UploadAll(u32 config);
void Distance_ResetAddr(Distance_TypeDef* Distance_Structure);
int Distance_TestConection(Distance_TypeDef* Distance_Structure);
#endif





