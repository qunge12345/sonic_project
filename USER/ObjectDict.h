#ifndef OBJECT_DICT_H
#define OBJECT_DICT_H

/**
	* @brief  Object dictionary of input register.
	*					Items blow are read-only variables to the hoster
	*/
enum inputRegEnum
{
	SONIC_0 = 0,
	SONIC_1,
	SONIC_2,
	SONIC_3,
	SONIC_4,
	SONIC_5,
	SONIC_6,
	SONIC_7,
	SONIC_8,
	SONIC_9,
	SONIC_10,
	SONIC_11,
	TERMINAL_NUM,
	VERSION_MAJOR,
	VERSION_MINOR,
	VERSION_FIX,
	TIMESTAMP_L,
	TIMESTAMP_H,
	TIMESTAMP_0_L,
	TIMESTAMP_1_L,
	TIMESTAMP_2_L,
	TIMESTAMP_3_L,
	TIMESTAMP_4_L,
	TIMESTAMP_5_L,
	TIMESTAMP_6_L,
	TIMESTAMP_7_L,
	TIMESTAMP_8_L,
	TIMESTAMP_9_L,
	TIMESTAMP_10_L,
	TIMESTAMP_11_L,
	
	INPUT_REG_NUM //do not add element after this line
};

/**
	* @brief  object dictionary of holding register.
	*					Items blow are writable variables to the hoster
	*/
enum holdingRegEnum
{
	COMMAND_RESET = 0,	
	COMMAND_IAP,
	COMMAND_DURATION_MS,
	HOLDING_REG_RSV,
	HOLDING_REG_NUM //do not add element after this line
};

/**
	* @brief  ID of modbus RTU slaver.
	*					Just change the number whithout adding items into enum structure.
	*/
enum slaveIdEnum
{
	SLAVE_ID = 1
};

#define KS103_SONIC_OFF 		0xFFFF
#define KS103_SONIC_OFFLINE 	0xFFFE
#define KS103_SONIC_TIMEOUT 	0xFFFD

#endif
//end of file
