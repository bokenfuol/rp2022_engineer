#ifndef __MODE_SWITCH_H
#define __MODE_SWITCH_H

#include "config.h"
#include "control.h"

typedef struct {
	system_mode_t 				mode;//进入模式标志位
	uint8_t								step;//模式切换步骤
}switch_mode_t;
	
void mode_switch_general(void);
void enter_normal_mode(void);
void enter_air_mode(void);
void enter_goldenore_mode(void);
void enter_silverore_mode(void);
void enter_ground_mode(void);
void enter_conversion_mode(void);
void enter_barrier_mode(void);
void enter_spnning_mode(void);
void enter_block_mode(void);
void enter_save_mode(void);

extern switch_mode_t mode_switch;

#endif
