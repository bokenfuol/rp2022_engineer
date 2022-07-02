/**
 * @file        device.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        17-November-2021
 * @brief       device
*/

#include "clamp.h"
#include "tranlation.h"
#include "lifting.h"
#include "cabin.h"
#include "ground.h"
#include "save.h"
#include "device.h"
 
device_t device [] ={
	[DEVICE_01] = {
	 .motor = &save.motor[SAVE_CARD_MOTOR].info,
	},
	[DEVICE_02] = {
	 .motor = &ground.motor[GROUND_MOTOR_L].info,
	},
	[DEVICE_03] = {
	 .motor = &ground.motor[GROUND_MOTOR_R].info,	
	},
	[DEVICE_04] = {
	 .motor = &ground.motor[GROUND_MOTOR_T].info,	
	},
	[DEVICE_05] = {
	 .motor = &lifting.motor[LIFTING_MOTOR_L].info,	
	},
	[DEVICE_06] = {
	 .motor = &lifting.motor[LIFTING_MOTOR_R].info,	
	},
	[DEVICE_07] = {
	 .motor = &cabin.motor[CABIN_MOTOR_L].info,	
	},
	[DEVICE_08] = {
	 .motor = &cabin.motor[CABIN_MOTOR_R].info,	
	},
	[DEVICE_09] = {
	 .motor = &tranlation.motor[TRANLATION_MOTOR].info,	
	},	
	[DEVICE_10] = {
	 .motor = &clamp.motor[CLAMP_MOTOR_LEFT].info,	
	},
	[DEVICE_11] = {
	 .motor = &clamp.motor[CLAMP_MOTOR_RIGHT].info,	
	},	
};
