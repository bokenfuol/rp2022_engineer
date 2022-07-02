#ifndef __TRANLATION_CONTROL_H 
#define __TRANLATION_CONTROL_H

#include "config.h"
#include "tranlation.h"

typedef struct {
	float target;
	float measure;
	reset_state_t reset;
}tranlation_control_t;

extern tranlation_control_t tranlation_control;


void general_tranlation_control(void);
reset_state_t tranlation_reset(void);
void tranlation_init(void);

#endif
