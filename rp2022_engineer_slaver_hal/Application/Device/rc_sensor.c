/**
 * @file        rc_sensor.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        2-November-2021
 * @brief       Ò£¿ØÆ÷
 */



#include "control.h"
#include "chassis_control.h"
#include "rc_sensor.h"
#include "keyboard.h"



rc_control_t rc_control = {
	.Last_L_Trigger = LOW,
	.Last_R_Trigger = LOW,
	.L_Trigger = LOW,
	.R_Trigger = LOW,
	.keyboard_mode = OFF,
	.parameter = {
		.move = 1/660.f*8000.f,
		.tranlation = 1/660.f*8000.f,
		.turn = 1/660.f*8000.f,
	},
};


//Ò£¿ØÆ÷½á¹¹Ìå
rc_sensor_t rc_sensor = {
	.offline_cnt = 100,
	.offline_max_cnt = 100,
	.work_state = DEV_OFFLINE,
	.heart_beat = rc_sensor_heart_beat,
	.check = rc_sensor_check,
};


/**
 * @date        2-November-2021
 * @brief       rc_sensor_check
 */
void rc_sensor_check(rc_sensor_t *rc,rc_control_t *control)
{
	/*Êý¾Ý´íÎó¼ì²é*/
	rc_sensor_info_check(rc);
	if(rc->errno == NONE_ERR)
	{
		control->Last_L_Trigger = control->L_Trigger;
		control->Last_R_Trigger = control->R_Trigger;
		switch(rc->s1)
		{
			case 3:
				control->L_Trigger = MID;
			break;
			case 1:
				control->L_Trigger = HIGH;
			break;
			case 2:
				control->L_Trigger = LOW;
			break;
		}
		switch(rc->s2)
		{
			case 3:
				rc_control.keyboard_mode = OFF;	
				control->R_Trigger = MID;
				RC_control();
			break;
			case 1:
				rc_control.keyboard_mode = ON;	
				control->R_Trigger = HIGH;
				Keyboard_control();
			break;	
			case 2:
				rc_control.keyboard_mode = OFF;	
				control->R_Trigger = LOW;
				RC_control();
			break;
		}
	}
	else
	{				
		//reset CPU
		__set_FAULTMASK(1);
		NVIC_SystemReset();
	}
}


/**
 * @date        3-November-2021
 * @brief       Ò£¿ØÆ÷¿ØÖÆ
 */
void RC_control()
{	switch(rc_control.R_Trigger)
	{
		case MID:/*»úÐµÄ£Ê½*/
			chassis_control.move = rc_sensor.ch3*rc_control.parameter.move;
			chassis_control.tranlation = rc_sensor.ch2*rc_control.parameter.tranlation;
			chassis_control.turn = rc_sensor.ch0*rc_control.parameter.turn;
		break;
		case LOW:
			chassis_control.move = 0;
			chassis_control.tranlation = 0;
			chassis_control.turn = 0;
		default:
		break;
	}
}

/**
 * @date        8-March-2022
 * @brief       Ò£¿ØÆ÷Êý¾Ý¼ì²é
 */
void rc_sensor_info_check(rc_sensor_t *rc_sen)
{
	
	if(abs(rc_sen->ch0) > 660 ||
	   abs(rc_sen->ch1) > 660 ||
	   abs(rc_sen->ch2) > 660 ||
	   abs(rc_sen->ch3) > 660)
	{
		rc_sen->errno = DEV_DATA_ERR;
		rc_sen->ch0 = 0;
		rc_sen->ch1 = 0;
		rc_sen->ch2 = 0;
		rc_sen->ch3 = 0;		
		rc_sen->s1 = MID;
		rc_sen->s2 = MID;
		rc_sen->thumbwheel = 0;
	}
	else
		rc_sen->errno = NONE_ERR;
}


/**
 * @date        2-November-2021
 * @brief       Ò£¿ØÆ÷ÐÄÌø
 */
void rc_sensor_heart_beat(rc_sensor_t *rc)
{
	rc->offline_cnt++;
	if(rc->offline_cnt > rc->offline_max_cnt)
	{
		rc->offline_cnt = rc->offline_max_cnt;
		rc->work_state = DEV_OFFLINE;
	}
	else if(rc->work_state == DEV_OFFLINE)
		rc->work_state = DEV_ONLINE;
}


