#ifndef __LIFTING_CONTROL_H 
#define __LIFTING_CONTROL_H

#include "config.h"
#include "lifting.h"

typedef struct {
	float target;
	float measure;
	state_t state_pneu;
	reset_state_t reset;
}lifting_control_t;

extern lifting_control_t lifting_control;


void general_lifting_control(void);
reset_state_t lifting_reset(void);
void lifting_init(void);

#endif
