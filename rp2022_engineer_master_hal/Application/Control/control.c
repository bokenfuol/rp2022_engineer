/**
 * @file        control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        3-November-2021
 * @brief       控制
 */

#include "control.h"
#include "mode_switch.h"
#include "clamp_control.h"
#include "tranlation_control.h"
#include "lifting_control.h"
#include "cabin_control.h"
#include "ground_control.h"
#include "save_control.h"
#include "rc_sensor.h"
#include "led.h"
#include "cmsis_os.h"

/*系统标志结构体*/
control_struct control = {
	.master_reset = RESET_NOT_OK,
	.slaver_reset = RESET_NOT_OK,
	.system_mode = AIR_MODE,
};

/**
 * @date        29-November-2021
 * @brief       总控任务
 */
void Control_Task(void const * argument)
{
	for(;;)
  {
		general_control();
    osDelay(1);
  }
}


/**
 * @date        3-November-2021
 * @brief       总控
 */
void general_control()
{
	/*复位*/
	general_reset();
	/*系统指示灯*/
	system_led();
	/*遥控器数据传递*/
	rc_sensor.check(&rc_sensor,&rc_control);
}


/**
 * @date        6-November-2021
 * @brief       复位
 */
void general_reset()
{
	if(!IS_SYSTEM_RESET&&IS_RC_ONLINE)
	{
		clamp.pneu->state = OFF;
		lifting.pneu->state = OFF;
		lifting_control.reset = lifting_reset();
		cabin_control.reset = RESET_OK;		
		save_control.reset = save_reset();
		if(lifting_control.reset&&lifting_control.measure>8000)
		{
			ground_control.reset = ground_reset();
   		clamp_control.reset = clamp_reset();
			if(clamp_control.reset)
				tranlation_control.reset = tranlation_reset();
			
			
			if(clamp_control.reset&&tranlation_control.reset&&lifting_control.reset&&cabin_control.reset\
			&&ground_control.reset&&save_control.reset&&control.slaver_reset)
				control.master_reset = RESET_OK;
		}

	}
	else if(!IS_RC_ONLINE)
	{	
		/*遥控器离线重新复位*/
		clamp_control.target = 0;
		tranlation_control.target = 0;
		lifting_control.target = 0;
		cabin_control.target_L = 0;
		cabin_control.target_R = 0;
		ground_control.target_pitch = 0;
		ground_control.target_tranlation = 0;
		save_control.target = 0;
		
		control.system_mode = AIR_MODE;
		
		mode_switch.mode = NORMAL_MODE;
		mode_switch.step = 1;
				
		tranlation_control.reset = RESET_NOT_OK;
		clamp_control.reset = RESET_NOT_OK;
		ground_control.reset = RESET_NOT_OK;
		cabin_control.reset = RESET_NOT_OK;
		lifting_control.reset = RESET_NOT_OK;
		control.slaver_reset = RESET_NOT_OK;
		save_control.reset = RESET_NOT_OK;
		control.master_reset = RESET_NOT_OK;
	}
}




	
