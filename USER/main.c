#include "sys.h"
#include "com_dbg.h"
//STM32开发板工程模板
u32 Connection_Status=0;
u8 Connection_RowSta=0;
u32 Sensor_Config=0;
	
int main(void)
{	
 	delay_init();
	LED_Init();
	Tim7_Init();
	Key_Init();
	Sensor_PwrInit();
	Distance_Init();
	Distance_Init232();
	Sensor_PwrCtrl(6);
	Sensor_Config=STMFlash_ReadWord(FLASH_SAVEADDR);
	while(1)
	{
		for(u8 n=0;n<6;++n)
		{
			if(Sensor_Config&(0xF<<(n*4)))
			{
				Distance_Update(n);
				//Distance_Upload(n,Sensor_Config);
				LED_Toggle();
			}
		}
		Distance_UploadAll(Sensor_Config);
		//COM_printf("1:%d\r\n",Distance_Structure[0][0].Data);
		//COM_printf("2:%d\r\n",Distance_Structure[0][1].Data);
		if(Key1_Status)
		{
			for(u8 i=0;i<6;++i)
			{
				Sensor_PwrCtrl(i);
				Distance_DefaultAddr(i);
			}
//			Sensor_PwrCtrl(6);
//			delay_ms(1000);
			Sensor_PwrCtrl(7);
			delay_ms(1000);
			STMFlash_WriteWord(FLASH_SAVEADDR,Distance_Test());
			//COM_printf("system restart\n\r");
			__set_FAULTMASK(1);
			NVIC_SystemReset();
		}
		if(Config_CMD==0x01)
		{
			Config_CMD=0;
			for(u8 i=0;i<6;++i)
			{
				Sensor_PwrCtrl(i);
				Distance_DefaultAddr(i);
			}
			Sensor_PwrCtrl(7);
			delay_ms(1000);
			STMFlash_WriteWord(FLASH_SAVEADDR,Distance_Test());
			__set_FAULTMASK(1);
			NVIC_SystemReset();
		}
	}
}


