#ifndef __PID_H
#define __PID_H

#include "config.h"

typedef struct{
    float		target;
    float		measure;
    float 	err;
    float 	last_err;
    float		kp;
    float 	ki;
    float 	kd;
    float 	out;
    float		integral;
    float 	integral_max;
		float		max_abs;
    float 	out_max;
	  float   deta_err;
}pid_ctrl;


float PIDControl(pid_ctrl *pid);

#endif
