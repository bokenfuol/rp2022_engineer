/**
 * @file        pid.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        13-November-2021
 * @brief       PIDËã·¨
 */

#include "pid.h"

/**
 * @brief       PID
 */
float PIDControl(pid_ctrl *pid)
{
	pid->err = pid->target - pid->measure;
	pid->err = 0.7f * pid->last_err + 0.3f * pid->err;
	pid->deta_err = pid->err - pid->last_err;
	pid->integral += pid->err;
	pid->integral = constrain(pid->integral, -pid->integral_max, pid->integral_max);
	pid->last_err = pid->err;
	pid->out = pid->kp * pid->err + pid->ki * pid->integral + pid->kd * pid->deta_err;
	pid->out = constrain(pid->out, -pid->out_max, pid->out_max);
	return pid->out;
}


