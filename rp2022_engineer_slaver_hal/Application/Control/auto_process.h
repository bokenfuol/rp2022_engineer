#ifndef __AUTO_PROCESS_H
#define __AUTO_PROCESS_H

#include "config.h"

void auto_process_general(void);

#define CONVERSION_STEP1 4500
#define CONVERSION_STEP2 100

typedef struct
{
	uint8_t chassis_conversion_master;
	uint8_t chassis_conversion_slaver;
	state_t save_flag;
	state_t silver_flag;
	
	
	uint8_t	save_solider;//æ»‘Æ±¯÷÷
}auto_process_t;

extern auto_process_t auto_process;

#endif
