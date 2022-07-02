#ifndef __GROUND_H
#define __GROUND_H

#include "config.h"
#include "motor.h"
#include "pneumatic.h"

#define MAX_GROUND_PITCH 					2700
#define MAX_GROUND_TRANLATION 		17800 



#define	GROUND_PITCH_GROUND 			MAX_GROUND_PITCH
#define	GROUND_TRANLATION_GROUND 	9786


#define BARRIER_PITCH_GROUND 			MAX_GROUND_PITCH
#define BARRIER_TRANLATION_GROUND 0

#define BACK_BARRIER							2082

//Ó¢ÐÛ
#define SAVE_PITCH_HERO								2082
#define SAVE_TRANLATION_HERO	 				3134
//ÂóÂÖ²½±ø
#define SAVE_PITCH_STANDARD1					2146
#define SAVE_TRANLATION_STANDARD1	 		3134
//È«ÏòÂÖ²½±ø
#define SAVE_PITCH_STANDARD2					1999
#define SAVE_TRANLATION_STANDARD2	 		9754
//¶æÂÖ²½±ø
#define SAVE_PITCH_STANDARD3					875
#define SAVE_TRANLATION_STANDARD3	 		0

#define BACK_PITCH						875




typedef enum{
  GROUND_MOTOR_L,
	GROUND_MOTOR_R,
	GROUND_MOTOR_T,
  GROUND_MOTOR_CNT	
}ground_motor_cnt_t;

typedef struct ground_struct{
	position_motor_t 				motor[GROUND_MOTOR_CNT];
}ground_t;

extern ground_t ground;

void ground_check(void);

#endif
