/**
 * @file        auto_process.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        6-April-2021
 * @brief       自动化
 */

#include "control.h"
#include "auto_process.h"
#include "mode_switch.h"
#include "vision_sensor.h"
#include "keyboard.h"
#include "chassis_control.h"
#include "cmsis_os.h"

auto_process_t auto_process = 
{
	.chassis_conversion_master = 0,
	.chassis_conversion_slaver = 0,
};


void Auto_Process_Task(void const * argument)
{
  for(;;)
  {
		/*自动化流程*/
#if slave_mode
		if(IS_SYSTEM_RESET&&rc_control.keyboard_mode)
			auto_process_general();
#endif		
    osDelay(1);
  }
}





 /**
 * @date        6-April-2021
 * @brief       一键化流程
 */
void auto_process_general()
{	
	float offset;
	
	if(auto_process.chassis_conversion_master == 0)
	{
		chassis_motor[CHAS_FL].power_motor.info.total_angle = 0;
		auto_process.chassis_conversion_slaver = 0;
	}
	
	
	if(keyboard_control.special_ctrl)
		keyboard_control.parameter.turn = 40;
	else if(keyboard_control.special_shift)
		keyboard_control.parameter.turn = 160;
	else
		keyboard_control.parameter.turn = 100;
	
	
	
	if(control.system_mode != mode_switch.mode)
	{
		keyboard_control.car_lock = ON;
		chassis_control.turn = 0;
		chassis_control.move = 0;
		chassis_control.tranlation = 0;
	}
	else 
		switch(control.system_mode)
		{
			case NORMAL_MODE:
				chassis_control.turn = keyboard_control.parameter.turn*keyboard_sensor.mouse_vx;			
				chassis_control.tranlation = (keyboard_control.A_move+keyboard_control.D_move);		
				chassis_control.move = (keyboard_control.W_move+keyboard_control.S_move);						
			break;
			case AIR_MODE:
			case GOLDENORE_MODE:
			case SILVERORE_MODE:
					if(!keyboard_control.car_lock)
						chassis_control.turn = keyboard_control.parameter.turn*keyboard_sensor.mouse_vx;
					else
						chassis_control.turn = 0;
					if(control.vision_state)
					 chassis_control.tranlation = 20*vision_sensor.rx.float_1;
					else
					 chassis_control.tranlation = (keyboard_control.A_move+keyboard_control.D_move);		
					chassis_control.move = (keyboard_control.W_move+keyboard_control.S_move) + 0.05*abs(chassis_control.tranlation);
			break;
			case GROUND_MODE:
			case BARRIER_MODE:
					if(!keyboard_control.car_lock)
						chassis_control.turn = keyboard_control.parameter.turn*keyboard_sensor.mouse_vx;
					else
						chassis_control.turn = 0;
					if(control.vision_state)
						chassis_control.tranlation = 10*vision_sensor.rx.float_1;
					else
						chassis_control.tranlation = (keyboard_control.A_move+keyboard_control.D_move);		
					chassis_control.move = (keyboard_control.W_move+keyboard_control.S_move);
			 break;
			 case SPINNING_MODE:
					chassis_control.turn = 5000;
					chassis_control.move = 0;
					chassis_control.tranlation = 0;
			 break;
			 case BLOCK_MODE:
					if(!keyboard_control.car_lock)
						chassis_control.turn = keyboard_control.parameter.turn*keyboard_sensor.mouse_vx;
					else
						chassis_control.turn = 0;
					chassis_control.move = keyboard_control.W_move+keyboard_control.S_move;
					chassis_control.tranlation = keyboard_control.A_move+keyboard_control.D_move;
					
					/*格挡模式默认速*/
					chassis_control.turn = constrain(chassis_control.turn,-2000,2000);
					chassis_control.move = constrain(chassis_control.move,-2000,2000);
					chassis_control.tranlation = constrain(chassis_control.tranlation,-2000,2000);
			 break;
			 case SAVE_MODE:
					if(auto_process.save_flag)
					{				 
						chassis_control.move = keyboard_control.W_move+keyboard_control.S_move;
						chassis_control.tranlation = -keyboard_control.parameter.turn*keyboard_sensor.mouse_vx/1.414;
						chassis_control.turn = -0.5*chassis_control.tranlation;
					}
					else
					{
						chassis_control.turn = keyboard_control.parameter.turn*keyboard_sensor.mouse_vx;
						chassis_control.move = keyboard_control.W_move+keyboard_control.S_move;
						chassis_control.tranlation = keyboard_control.A_move+keyboard_control.D_move;					
					}
			 break;
			 case CONVERSION_MODE:
				 //一号轮组
					if(chassis_motor[CHAS_FL].power_motor.info.work_state == DEV_ONLINE)
					{
						 if(auto_process.chassis_conversion_master == 1)
						 {
							 //后退
								offset = (chassis_motor[CHAS_FL].power_motor.info.angle - chassis_motor[CHAS_FL].power_motor.info.last_angle)/8192.f*360.f;
								angle_constrain(&offset);
								chassis_motor[CHAS_FL].power_motor.info.last_angle = chassis_motor[CHAS_FL].power_motor.info.angle;
								chassis_motor[CHAS_FL].power_motor.info.total_angle += offset;
								if(abs(chassis_motor[CHAS_FL].power_motor.info.total_angle) <= CONVERSION_STEP1)
									chassis_control.move = -1000;
								else
								{
									auto_process.chassis_conversion_slaver = 1;
									chassis_control.move = 0;
								}
					 }
					 else if(auto_process.chassis_conversion_master == 2)
					 {
						 //前进
							offset = (chassis_motor[CHAS_FL].power_motor.info.angle - chassis_motor[CHAS_FL].power_motor.info.last_angle)/8192.f*360.f;
							angle_constrain(&offset);
							chassis_motor[CHAS_FL].power_motor.info.last_angle = chassis_motor[CHAS_FL].power_motor.info.angle;
							chassis_motor[CHAS_FL].power_motor.info.total_angle += offset;	 
							if(abs(chassis_motor[CHAS_FL].power_motor.info.total_angle) >= CONVERSION_STEP2)
								chassis_control.move = 1000;
							else
							{
								auto_process.chassis_conversion_slaver = 2;
								chassis_control.move = 0;
							}
					 }
					 else if(auto_process.chassis_conversion_master == 0)
					 {
							chassis_control.turn = keyboard_control.parameter.turn*keyboard_sensor.mouse_vx;
							chassis_control.move = keyboard_control.W_move+keyboard_control.S_move;
							chassis_control.tranlation = keyboard_control.A_move+keyboard_control.D_move;							 
					 } 
					}	
					//二号轮组
					else if(chassis_motor[CHAS_FR].power_motor.info.work_state == DEV_ONLINE)
					{
					 if(auto_process.chassis_conversion_master == 1)
					 {
						 //后退
							offset = (chassis_motor[CHAS_FR].power_motor.info.angle - chassis_motor[CHAS_FR].power_motor.info.last_angle)/8192.f*360.f;
							angle_constrain(&offset);
							chassis_motor[CHAS_FR].power_motor.info.last_angle = chassis_motor[CHAS_FR].power_motor.info.angle;
							chassis_motor[CHAS_FR].power_motor.info.total_angle += offset;
							if(abs(chassis_motor[CHAS_FR].power_motor.info.total_angle) <= CONVERSION_STEP1)
								chassis_control.move = -1000;
							else
							{
								auto_process.chassis_conversion_slaver = 1;
								chassis_control.move = 0;
							}
					 }
					 else if(auto_process.chassis_conversion_master == 2)
					 {
						 //前进
							offset = (chassis_motor[CHAS_FR].power_motor.info.angle - chassis_motor[CHAS_FR].power_motor.info.last_angle)/8192.f*360.f;
							angle_constrain(&offset);
							chassis_motor[CHAS_FR].power_motor.info.last_angle = chassis_motor[CHAS_FR].power_motor.info.angle;
							chassis_motor[CHAS_FR].power_motor.info.total_angle += offset;	 
						 if(abs(chassis_motor[CHAS_FR].power_motor.info.total_angle) >= CONVERSION_STEP2)
							 chassis_control.move = 1000;
						 else
						 {
							 auto_process.chassis_conversion_slaver = 2;
							 chassis_control.move = 0;
						 }
					 }
					 else if(auto_process.chassis_conversion_master == 0)
					 {
							chassis_control.turn = keyboard_control.parameter.turn*keyboard_sensor.mouse_vx;
								chassis_control.move = keyboard_control.W_move+keyboard_control.S_move;
							chassis_control.tranlation = keyboard_control.A_move+keyboard_control.D_move;							 
					 } 
					}	
					//三号轮组
					else if(chassis_motor[CHAS_BL].power_motor.info.work_state == DEV_ONLINE)
					{
					 if(auto_process.chassis_conversion_master == 1)
					 {
						 //后退
							offset = (chassis_motor[CHAS_BL].power_motor.info.angle - chassis_motor[CHAS_BL].power_motor.info.last_angle)/8192.f*360.f;
							angle_constrain(&offset);
							chassis_motor[CHAS_BL].power_motor.info.last_angle = chassis_motor[CHAS_BL].power_motor.info.angle;
							chassis_motor[CHAS_BL].power_motor.info.total_angle += offset;
							if(abs(chassis_motor[CHAS_BL].power_motor.info.total_angle) <= CONVERSION_STEP1)
								chassis_control.move = -1000;
							else
							{
								auto_process.chassis_conversion_slaver = 1;
								chassis_control.move = 0;
							}
					 }
					 else if(auto_process.chassis_conversion_master == 2)
					 {
						 //前进
							offset = (chassis_motor[CHAS_BL].power_motor.info.angle - chassis_motor[CHAS_BL].power_motor.info.last_angle)/8192.f*360.f;
							angle_constrain(&offset);
							chassis_motor[CHAS_BL].power_motor.info.last_angle = chassis_motor[CHAS_BL].power_motor.info.angle;
							chassis_motor[CHAS_BL].power_motor.info.total_angle += offset;	 
						 if(abs(chassis_motor[CHAS_BL].power_motor.info.total_angle) >= CONVERSION_STEP2)
							 chassis_control.move = 1000;
						 else
						 {
							 auto_process.chassis_conversion_slaver = 2;
							 chassis_control.move = 0;
						 }
					 }
					 else if(auto_process.chassis_conversion_master == 0)
					 {
							chassis_control.turn = keyboard_control.parameter.turn*keyboard_sensor.mouse_vx;
								chassis_control.move = keyboard_control.W_move+keyboard_control.S_move;
							chassis_control.tranlation = keyboard_control.A_move+keyboard_control.D_move;							 
					 } 
					}
					//四号轮组
					else if(chassis_motor[CHAS_BR].power_motor.info.work_state == DEV_ONLINE)
					{
					 if(auto_process.chassis_conversion_master == 1)
					 {
						 //前进
							offset = (chassis_motor[CHAS_BR].power_motor.info.angle - chassis_motor[CHAS_BR].power_motor.info.last_angle)/8192.f*360.f;
							angle_constrain(&offset);
							chassis_motor[CHAS_BR].power_motor.info.last_angle = chassis_motor[CHAS_BR].power_motor.info.angle;
							chassis_motor[CHAS_BR].power_motor.info.total_angle += offset;
							if(abs(chassis_motor[CHAS_BR].power_motor.info.total_angle) <= CONVERSION_STEP1)
								chassis_control.move = -1000;
							else
							{
								auto_process.chassis_conversion_slaver = 1;
								chassis_control.move = 0;
							}
					 }
					 else if(auto_process.chassis_conversion_master == 2)
					 {
						  //后退
							offset = (chassis_motor[CHAS_BR].power_motor.info.angle - chassis_motor[CHAS_BR].power_motor.info.last_angle)/8192.f*360.f;
							angle_constrain(&offset);
							chassis_motor[CHAS_BR].power_motor.info.last_angle = chassis_motor[CHAS_BR].power_motor.info.angle;
							chassis_motor[CHAS_BR].power_motor.info.total_angle += offset;	 
						 if(abs(chassis_motor[CHAS_BR].power_motor.info.total_angle) >= CONVERSION_STEP2)
							 chassis_control.move = 1000;
						 else
						 {
							 auto_process.chassis_conversion_slaver = 2;
							 chassis_control.move = 0;
						 }
					 }
					 else if(auto_process.chassis_conversion_master == 0)
					 {
							chassis_control.turn = keyboard_control.parameter.turn*keyboard_sensor.mouse_vx;
								chassis_control.move = keyboard_control.W_move+keyboard_control.S_move;
							chassis_control.tranlation = keyboard_control.A_move+keyboard_control.D_move;							 
					 } 
					}				 
			 break;
		}

}




 
