#ifndef __CABIN_CONTROL_H 
#define __CABIN_CONTROL_H

#include "config.h"
#include "cabin.h"

#define OVERTURN_CABIN 	3300
#define UPORE_CABIN			3300
#define DWON_CABIN			3300
#define LIFT_CABIN			3300

typedef struct {
	int16_t target_L;
	int16_t target_R;
	int16_t measure_L;
	int16_t measure_R;
	state_t state_keylight_up;
	state_t state_keylight_down;
	reset_state_t reset;
}cabin_control_t;

extern cabin_control_t cabin_control;


void general_cabin_control(void);
reset_state_t cabin_reset(void);
void cabin_init(void);

#endif
