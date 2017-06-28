#ifndef __SONIC_H__
#define __SONIC_H__

#include "stdint.h"

#define SONIC_ROW_0								0x01
#define SONIC_ROW_1								0x02
#define SONIC_ROW_2								0x04
#define SONIC_ROW_3								0x08

#define SONIC_DETECT_RECV					0x211

class CSonic
{
public:
	CSonic(uint8_t addr);
	~CSonic();
	void reset_id();
	void send_cmd();
	void read_data();
	void detect();
	uint16_t get_data(uint8_t num) {return _data[num];};
	uint16_t get_status()	{return _is_valid;};
	
private:
	uint8_t _addr;
	uint16_t _data[4];
	uint8_t _is_valid;
};

extern CSonic Sonic[6];

#endif

