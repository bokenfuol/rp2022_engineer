/**
 * @file        rc_sensor.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        2-November-2021
 * @brief       遥控器
 */
 
 
//-----------------------------------抬升机构运动时矿仓摩擦轮是否要运动？？？-----------------------------------------//

#include "control.h"
#include "clamp_control.h"
#include "tranlation_control.h"
#include "lifting_control.h"
#include "cabin_control.h"
#include "ground_control.h"
#include "save_control.h"
#include "rc_sensor.h"
#include "keyboard.h"


rc_control_t rc_control = {
	/*rc参数列表区*/
	.parameter =  {
		.clamp = 0.008f,			//夹爪参数				
		.tranlation = 25/660.f,			 	//横移参数
		.lifting = 25/660.f,					//抬升参数
		.cabin_speed = 5000,					//矿仓电机速度
		.ground_tranlation = 25/660.f,//地矿参数
		.ground_pitch = 0.008f			
	},
	.Last_L_Trigger = LOW,
	.Last_R_Trigger = LOW,
	.L_Trigger = LOW,
	.R_Trigger = LOW,
	.keyboard_mode = OFF,
};


//遥控器结构体
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
	/*数据错误检查*/
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
 * @brief       遥控器控制
 */
void RC_control()
{
	
	switch(rc_control.R_Trigger)
	{
		case MID:/*机械模式*/
//			if(clamp_control.state_keylight)
//				clamp_control.state_pneu = ON;
			/*救援卡*/
			if(rc_control.Last_L_Trigger == MID && rc_control.L_Trigger == LOW)
				save_control.target = MAX_SAVE;
			else if(rc_control.L_Trigger == MID)
				save_control.target = 0;
			/*抬升*/
			lifting_control.target -= rc_sensor.thumbwheel*rc_control.parameter.lifting;
		break;
		case LOW:/*夹爪模式*/
			switch(rc_control.L_Trigger)
			{
				case HIGH:
					/*夹爪运动*/
					clamp_control.target -= rc_sensor.ch3*rc_control.parameter.clamp;
					/*横移运动*/
					tranlation_control.target +=  rc_sensor.ch2*rc_control.parameter.tranlation;
					/*前伸*/
					if(rc_sensor.ch1>300)
						lifting_control.state_pneu = ON;
					else if(rc_sensor.ch1<-300)
						lifting_control.state_pneu = OFF;
					/*夹取*/
					if(rc_sensor.thumbwheel>300)
						clamp_control.state_pneu = ON;
					else if(rc_sensor.thumbwheel<-300)
						clamp_control.state_pneu = OFF;
				break;
				case MID:
					/*下夹爪运动*/
					ground_control.target_pitch += rc_sensor.ch3*rc_control.parameter.ground_pitch;
					ground_control.target_tranlation -=  rc_sensor.ch2*rc_control.parameter.ground_tranlation;
				
					/*矿仓运动*/
					cabin_control.target_L = rc_control.parameter.cabin_speed*(rc_sensor.ch0>300||rc_sensor.ch1<-300) - rc_control.parameter.cabin_speed*(rc_sensor.ch0<-300||rc_sensor.ch1>300);
					cabin_control.target_R = rc_control.parameter.cabin_speed*(rc_sensor.ch0>300||rc_sensor.ch1>300) - rc_control.parameter.cabin_speed*(rc_sensor.ch0<-300||rc_sensor.ch1<-300);
					
					/*抬升*/
					lifting_control.target -= rc_sensor.thumbwheel*rc_control.parameter.lifting;		
				break;
				case LOW:
					/*抬升*/
					lifting_control.target += rc_sensor.ch3*rc_control.parameter.lifting;
				
					/*矿仓运动*/
					cabin_control.target_L = rc_control.parameter.cabin_speed*(rc_sensor.ch0>300||rc_sensor.ch1<-300) - rc_control.parameter.cabin_speed*(rc_sensor.ch0<-300||rc_sensor.ch1>300);
					cabin_control.target_R = rc_control.parameter.cabin_speed*(rc_sensor.ch0>300||rc_sensor.ch1>300) - rc_control.parameter.cabin_speed*(rc_sensor.ch0<-300||rc_sensor.ch1<-300);
					
					/*夹取*/
					if(rc_sensor.thumbwheel>300)
						clamp_control.state_pneu = ON;
					else if(rc_sensor.thumbwheel<-300)
						clamp_control.state_pneu = OFF;					
				break;
			}
		break;
		default:
		break;
	}
}                    



/**
 * @date        8-March-2022
 * @brief       遥控器数据检查
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
 * @brief       遥控器心跳
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


