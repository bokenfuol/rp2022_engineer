/**
 * @file        device.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        17-November-2021
 * @brief       device
*/
 
#include "chassis.h"
#include "device.h"
 
device_t device [] ={
	[DEVICE_01] = {
	 .motor = &chassis_motor[CHAS_FL].power_motor.info,
	},
	[DEVICE_02] = {
	 .motor = &chassis_motor[CHAS_FR].power_motor.info,
	}, 
	[DEVICE_03] = {
	 .motor = &chassis_motor[CHAS_BL].power_motor.info,
	}, 
	[DEVICE_04] = {
	 .motor = &chassis_motor[CHAS_BR].power_motor.info,
	}, 
	[DEVICE_05] = {
	 .motor = &chassis_motor[CHAS_FL].rudder_motor.info,
	}, 
	[DEVICE_06] = {
	 .motor = &chassis_motor[CHAS_FR].rudder_motor.info,
	}, 
	[DEVICE_07] = {
	 .motor = &chassis_motor[CHAS_BL].rudder_motor.info,
	}, 
	[DEVICE_08] = {
	 .motor = &chassis_motor[CHAS_BR].rudder_motor.info,
	}, 
};
