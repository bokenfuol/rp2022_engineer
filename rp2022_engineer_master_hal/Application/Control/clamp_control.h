#ifndef __CLAMP_CONTROL_H 
#define __CLAMP_CONTROL_H

#include "config.h"
#include "clamp.h"


typedef struct {
	float target;
	float measure;
	state_t	state_pneu;
	state_t	state_keylight;
	reset_state_t reset;
}clamp_control_t;

extern clamp_control_t clamp_control;


void general_clamp_control(void);
reset_state_t clamp_reset(void);
void clamp_init(void);

#endif
