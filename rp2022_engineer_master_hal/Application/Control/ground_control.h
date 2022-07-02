#ifndef __GROUND_CONTROL_H 
#define __GROUND_CONTROL_H

#include "config.h"
#include "ground.h"

#define BLOCK_GROUND 	309
#define ORE_GROUND		3300
#define SAVE_GROUND   3300
#define SAVE_FINISH		3000

typedef struct {
	float target_pitch;
	float target_tranlation;
	float measure_pitch;
	float measure_tranlation;
	reset_state_t reset;
}ground_control_t;

extern ground_control_t ground_control;


void general_ground_control(void);
reset_state_t ground_reset(void);
void ground_init(void);

#endif
