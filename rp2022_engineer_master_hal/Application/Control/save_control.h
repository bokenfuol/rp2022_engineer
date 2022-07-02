#ifndef __SAVE_CONTROL_H 
#define __SAVE_CONTROL_H

#include "config.h"
#include "save.h"




typedef struct {
	float target;
	float measure;
	reset_state_t reset;
}save_control_t;

extern save_control_t save_control;


void general_save_control(void);
reset_state_t save_reset(void);
void save_init(void);

#endif
