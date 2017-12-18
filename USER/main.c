#include "timer.h"
#include "puart.h"
#include "task.h"
#include "pwr_ctrl.h"
#include "CommonConfig.h"
#include "ModbusRtuSlave.h"
#include "CUsart.h"
#include "Console.h"
#include "powerupOption.h"
#include <stdint.h>

int main(void)
{
	SCB->VTOR = 0x08008000;
	CommonConfig();
	pvf::initInApp();
	BaseTimer::Instance()->initialize();
	BaseTimer::Instance()->start();
	pwr_init();

	// initialize sonic uart
	puart_init();

	Console::Instance()->printf("Program start\r\n");
	// initialize modbus slave
	ModbusSlave::Instance()->run();

	ModbusSlave::Instance()->inputReg(CModbusRtuSlave::VERSION_MAJOR) = 2;
	ModbusSlave::Instance()->inputReg(CModbusRtuSlave::VERSION_MINOR) = 1;
	ModbusSlave::Instance()->inputReg(CModbusRtuSlave::VERSION_FIX) = 2;

	while(1)
	{
		led_do_run();
		sonnic_do_run();
		key_scan_do_run();
		TimeStamp::Instance()->run();

		ModbusSlave::Instance()->run();
		Console::Instance()->runTransmitter();
		ServiceDog();
	}

}

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	Timer loopTimer(1000,1000);

  /* Infinite loop */
  while (1)
  {
		if(loopTimer.isAbsoluteTimeUp())
		{
			// say something fuck
		}
  }
}

