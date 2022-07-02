/**
 * @file        keylight.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        146January-2022
 * @brief       LEDµÈ
 */

#include "led.h"
#include "control.h"

void system_led()
{
	if(control.system_mode & 0x01)
		GREEN_ON;
	else
		GREEN_OFF;
	if((control.system_mode & 0x02)>>1)
		RED_ON;
	else
		RED_OFF;
	if((control.system_mode & 0x04)>>2)
		BLUE_ON;
	else
		BLUE_OFF;
	if(control.master_reset == RESET_OK)
		ORANGE_ON;
	else
		ORANGE_OFF;
}