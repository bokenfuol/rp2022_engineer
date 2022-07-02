/**
 * @file        auto_process.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        6-April-2021
 * @brief       自动化
 */

#include "mode_switch.h"
#include "auto_process.h"
#include "keyboard.h"
#include "auto_process.h"
#include "mode_switch.h"
#include "clamp_control.h"
#include "tranlation_control.h"
#include "lifting_control.h"
#include "cabin_control.h"
#include "ground_control.h"
#include "save_control.h"
#include "cmsis_os.h"


void Auto_Process_Task(void const * argument)
{
  for(;;)
  {
		/*模式切换(内含自动化流程)*/
		if(IS_SYSTEM_RESET&&rc_control.keyboard_mode)
			mode_switch_general();	
    osDelay(1);
  }
}


 
/*------------------process!=0,step=0意为等待，process=0,step=0意为退出，process!=0,step!=0意为进行操作,process=0,step!=0出现错误---------------------------*/

//电池阀状态为需要延时，否则系统反应太快会导致故障


/*自动化流程延时*/
uint32_t autoprocess_delay = 0; 
auto_process_t auto_process = 
{
	.chassis_conversion_master = 0,
	.chassis_conversion_slaver = 0,
	.save_solider = 1,
	.ore_num = 0,
	.emer = OFF,
	.barrier_flag = OFF,
	.clamp_process = {
		.process = 0,
		.step = 0,
	},
	.cabin_process = {
		.process = 0,
		.step = 0,
	},
	.ground_process = {
		.process = 0,
		.step = 0,
	},	
};



 /**
 * @date        6-April-2021
 * @brief       一键化流程
 */
void auto_process_general()
{	
	mode_switch.step = 1;
	/*夹爪自动化*/
	clamp_auto_process();
	/*地矿自动化*/
	ground_auto_process();
	/*矿仓自动化*/
	cabin_auto_process();			
}

 /**
 * @date        6-April-2021
 * @brief       夹爪一键化流程
 */
void clamp_auto_process()
{
/*-------------------------------夹爪自动化------------------------------------*/
	switch(auto_process.clamp_process.process)/*1：空接后 2：小资源岛一键三矿 3：地矿对接 4：兑换 */
	{
		case 1:			/*1：夹矿后的操作*/
			clamp_after();
		break;
		case 2:			/*2：小资源岛一键三矿*/
			clamp_sivler_Three();
		break;
		case 3:			/*3：地矿对接*/
			clamp_after_ground();
		break;
		case 4:		/*4：兑换*/
			clamp_conversion();		
		break;
		default:
			auto_process.clamp_process.step = 0;			
		break;
	}
		
}


 /**
 * @date        6-April-2021
 * @brief       地矿一键化流程
 */
void ground_auto_process()
{
/*-------------------------------地矿自动化------------------------------------*/	
	switch(auto_process.ground_process.process)/*1：地矿  2：障碍块  3：救援*/
	{
		case 1:		/*1：地矿*/
			ground_ground();
		break;	
		case 2:		/*2：障碍块*/
			ground_barrier();
		break;
		case 3:		/*3：救援*/
			ground_save();
		break;
		default:
			auto_process.ground_process.step = 0;	
		break;
	}		
}


 /**
 * @date        6-April-2021
 * @brief       矿仓一键化流程
 */
uint64_t cabin_time=0;
void cabin_auto_process()
{
/*-------------------------------矿仓自动化------------------------------------*/
	if(!cabin_control.state_keylight_up&&cabin_control.state_keylight_down&&auto_process.cabin_process.process == 1)
		auto_process.cabin_process.process = 0;	
	
//	if(cabin_control.state_keylight_up&&auto_process.cabin_process.process == 2)
//		auto_process.cabin_process.process = 0;
	
	if(cabin_time >2000)
	{
		cabin_time = 0;
		auto_process.cabin_process.process = 0;
	}
	switch(auto_process.cabin_process.process)/*1：下矿  2：上矿  3：左翻矿  4：右翻矿*/
	{
		case 1:/*下矿*/
			cabin_time ++;
			cabin_control.target_L = keyboard_control.parameter.cabin_speed;	
			cabin_control.target_R = -keyboard_control.parameter.cabin_speed;
		break;
		case 2:/*上矿*/
			cabin_time ++;
			cabin_control.target_L = -keyboard_control.parameter.cabin_speed;	
			cabin_control.target_R = keyboard_control.parameter.cabin_speed;		
		break;
		case 3:/*左翻矿*/
			cabin_control.target_L = keyboard_control.parameter.cabin_speed;	
			cabin_control.target_R = keyboard_control.parameter.cabin_speed;		
		break;
		case 4:/*右翻矿*/
			cabin_control.target_L = -keyboard_control.parameter.cabin_speed;	
			cabin_control.target_R = -keyboard_control.parameter.cabin_speed;
		break;
		default:
			cabin_control.target_L = 0;	
			cabin_control.target_R = 0;					
		break;
	}		
}


 /**
 * @date        17-May-2022
 * @brief       矿后
 */
uint32_t process_time = 0;
void clamp_after()
{
	switch(auto_process.clamp_process.step)
	{
		case 1:
			if(clamp_control.state_keylight)
				process_time ++;
			if(process_time>20)
			{
				process_time = 0;
				auto_process.clamp_process.step ++;
			}
		 break;
		case 2:
			autoprocess_delay ++;
			/*夹取*/
			clamp_control.state_pneu = ON;
			/*延时500ms*/
			if(autoprocess_delay>300)
			{
				/*清除延时并进入下一步*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 3:
			/*夹爪*/
			clamp_control.target = MAX_CLAMP/2;		
			if(distance(clamp_control.measure, MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;
		break;
		case 4:
			/*横移回到中间*/
			tranlation_control.target = MID_TRANLASION;				
			if(distance(tranlation_control.measure,MID_TRANLASION)<115)
				auto_process.clamp_process.step ++;					
		break;
		case 5:
			autoprocess_delay ++;
			/*前伸*/
			lifting_control.state_pneu = OFF;
			if(autoprocess_delay>800&&!cabin_control.state_keylight_up)
			{
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 6:
			if(auto_process.emer)//应急处理
				auto_process.clamp_process.process = 0;
			else 
			{
				/*夹爪后翻*/
				clamp_control.target = MAX_CLAMP;
				if(distance(clamp_control.measure,MAX_CLAMP)<115)
					auto_process.clamp_process.step ++;
			}
		break;
		case 7:
			/*松开矿石*/
			clamp_control.state_pneu = OFF;
			autoprocess_delay ++;
			auto_process.cabin_process.process = 1;
			if(autoprocess_delay>300)
			{
				/*清除延时并进入下一步*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 8:
			/*结束自动化*/
			auto_process.clamp_process.process = 0;
//此处改???
			if(!cabin_control.state_keylight_down||!cabin_control.state_keylight_up)//接第二个矿
				/*进入待转区域*/
				control.system_mode = NORMAL_MODE;
			else
				clamp_control.target = MAX_CLAMP/2;
		break;
			
			
		case 10:
			/*夹爪*/
			clamp_control.target = GOLDERN_CALMP;		
			if(distance(clamp_control.measure, GOLDERN_CALMP)<115)
				auto_process.clamp_process.step = 2;			
		break;
	}
}	

 /**
 * @date        17-May-2022
 * @brief       一键三矿
 */
void clamp_sivler_Three()
{
	switch(auto_process.clamp_process.step)
	{
		case 1:
			autoprocess_delay ++;
			/*夹取*/
			clamp_control.state_pneu = ON;
			/*延时500ms*/
			if(autoprocess_delay>300&&!cabin_control.state_keylight_up)
			{
				/*清除延时并进入下一步*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 2:
			if(auto_process.emer)//应急处理
				auto_process.clamp_process.process = 0;
			else
			{
				/*夹爪后翻*/
				clamp_control.target = MAX_CLAMP;
				if(distance(clamp_control.measure,MAX_CLAMP)<115)
					auto_process.clamp_process.step ++;
			}
		break;
		case 3:
			/*松开矿石*/
			clamp_control.state_pneu = OFF;
			autoprocess_delay ++;
			auto_process.cabin_process.process = 1;
			if(autoprocess_delay>300)
			{
				/*清除延时并进入下一步*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 4:	
			clamp_control.target = MAX_CLAMP/2;
			if(distance(clamp_control.measure,MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;
		break;
		case 5:
			tranlation_control.target = LEFT_TRANLASION;
			if(distance(tranlation_control.measure,LEFT_TRANLASION)<115)
				auto_process.clamp_process.step ++;	
		break;
		case 6:
			clamp_control.target = SILVER_CLAMP;
			if(distance(clamp_control.measure,SILVER_CLAMP)<115)
				auto_process.clamp_process.step ++;	
		break;
		case 7:
			autoprocess_delay ++;
			/*夹取*/
			clamp_control.state_pneu = ON;
			/*延时500ms*/
			if(autoprocess_delay>300)
			{
				/*清除延时并进入下一步*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 8:
			/*夹爪*/
			clamp_control.target = MAX_CLAMP/2;		
			if(distance(clamp_control.measure, MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;
		break;
		case 9:
			/*横移回到中间*/
			tranlation_control.target = MID_TRANLASION;				
			if(distance(tranlation_control.measure,MID_TRANLASION)<115&&!cabin_control.state_keylight_up)
				auto_process.clamp_process.step ++;					
		break;
		case 10:
			if(!cabin_control.state_keylight_up)
			{
				/*夹爪后翻*/
				clamp_control.target = MAX_CLAMP;
				if(distance(clamp_control.measure,MAX_CLAMP)<115)
					auto_process.clamp_process.step ++;
			}
		break;			
		case 11:
			/*松开矿石*/
			clamp_control.state_pneu = OFF;
			autoprocess_delay ++;
			auto_process.cabin_process.process = 1;
			if(autoprocess_delay>300)
			{
				/*清除延时并进入下一步*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 12:	
			clamp_control.target = MAX_CLAMP/2;
			if(distance(clamp_control.measure,MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;
		break;
		case 13:
			tranlation_control.target = RIGHT_TRALASION;
			if(distance(tranlation_control.measure,RIGHT_TRALASION)<115)
				auto_process.clamp_process.step ++;	
		break;
		case 14:
			clamp_control.target = SILVER_CLAMP;
			if(distance(clamp_control.measure,SILVER_CLAMP)<115)
				auto_process.clamp_process.step ++;	
		break;
		case 15:
			autoprocess_delay ++;
			/*夹取*/
			clamp_control.state_pneu = ON;
			/*延时500ms*/
			if(autoprocess_delay>300)
			{
				/*清除延时并进入下一步*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 16:
			/*夹爪*/
			clamp_control.target = MAX_CLAMP/2;		
			if(distance(clamp_control.measure, MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;
		break;
		case 17:
			/*横移回到中间*/
			tranlation_control.target = MID_TRANLASION;				
			if(distance(tranlation_control.measure,MID_TRANLASION)<115)
				auto_process.clamp_process.step ++;					
		break;	
		case 18:
			auto_process.ore_num = 3;
			clamp_control.target = 3750;	
			/*结束自动化*/
			auto_process.clamp_process.process = 0;
		break;
	}	
}


 /**
 * @date        17-May-2022
 * @brief       地矿后
 */
void clamp_after_ground()
{
	switch(auto_process.clamp_process.step)
	{
		case 1:
			autoprocess_delay ++;
			/*对接*/
			clamp_control.state_pneu = ON;
			if(autoprocess_delay>500)
				auto_process.ground_process.step ++;
		break;
		case 2:
			autoprocess_delay ++;
			/*夹爪后翻*/
			clamp_control.target = MAX_CLAMP;
			if(distance(clamp_control.measure,MAX_CLAMP)<115&&autoprocess_delay>800)
				auto_process.clamp_process.step ++;
		break;
		case 3:
			/*松开矿石*/
			clamp_control.state_pneu = OFF;
			autoprocess_delay ++;
			auto_process.cabin_process.process = 1;
			if(autoprocess_delay>300)
			{
				/*清除延时并进入下一步*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 5:
			/*结束自动化*/
			auto_process.clamp_process.process = 0;
		break;
	}		
}

 /**
 * @date        17-May-2022
 * @brief       兑换
 */
void clamp_conversion()
{
switch(auto_process.clamp_process.step)
	{
		case 0:	
			/*车往后退一定距离*/
			auto_process.chassis_conversion_master = 1;
			if(auto_process.chassis_conversion_slaver == 1)
				auto_process.clamp_process.step ++;
		break;
		case 1:
			/*判断是否三矿*/
			if(auto_process.ore_num == 3||auto_process.emer)
			{
				auto_process.ore_num --;
				auto_process.clamp_process.step = 6;
			}
			else
				auto_process.clamp_process.step ++;
		break;
		/*2为等待鼠标下一步*/
		case 3:
			/*回来夹矿*/
			clamp_control.target = MAX_CLAMP;
			if(distance(clamp_control.measure,MAX_CLAMP)<115)
				auto_process.clamp_process.step ++;
		break;
		case 4:
			/*夹取*/
			autoprocess_delay ++;
			clamp_control.state_pneu = ON;
			if(autoprocess_delay>500)
			{
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 5:	
			clamp_control.target = MAX_CLAMP/2;
			if(distance(clamp_control.measure,MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;	
		break;
		case 6:
			/*前伸*/
			autoprocess_delay ++;
			lifting_control.state_pneu = ON;
			if(autoprocess_delay>1000)
			{
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 7:
			autoprocess_delay ++;
			clamp_control.target = 326;
			if(distance(clamp_control.measure,326)<115&&autoprocess_delay>800)
			{
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 8:

		/*松旷*/
			clamp_control.state_pneu = OFF;
			autoprocess_delay ++;
			if(autoprocess_delay>500)
			{
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 9:
			clamp_control.target = MAX_CLAMP/2;
			if(distance(clamp_control.measure,MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;
		break;
		/*10为等待鼠标*/
		case 11:
			auto_process.chassis_conversion_master = 2;
			if(auto_process.chassis_conversion_slaver == 2)
				auto_process.clamp_process.step ++;		
		break;
		case 12:
			auto_process.chassis_conversion_master = 1;
			if(auto_process.chassis_conversion_slaver == 1)
				auto_process.clamp_process.step ++;				
		break;
		case 13:
			/*是否清空矿仓*/
			if(!cabin_control.state_keylight_up&&!cabin_control.state_keylight_down&&!clamp_control.state_keylight)
				mode_switch.mode = NORMAL_MODE;
			else
				/*进入待转区域*/
				control.system_mode = NORMAL_MODE;
		break;
			
		case 20:
			lifting_control.target = 0;
//			if(distance(lifting_control.measure,0)<115)
//				auto_process.clamp_process.step ++;
		break;
		case 21:
			lifting_control.target = CONVERSION_LIFT;
			if(distance(lifting_control.measure,CONVERSION_LIFT)<115)
				auto_process.clamp_process.step = 2;
			
	}
}

 /**
 * @date        17-May-2022
 * @brief       下夹爪地矿
 */
void ground_ground()
{
		switch(auto_process.ground_process.step)
		{ 
			case 1:
				/*夹矿*/
				ground_control.target_tranlation = MAX_GROUND_TRANLATION;
				lifting_control.target = SAVE_LIFT;
				if(distance(ground_control.measure_tranlation,16450)<115&&distance(lifting_control.measure,SAVE_LIFT)<300)
//				if(distance(ground.motor[GROUND_MOTOR_T].info.last_angle,ground.motor[GROUND_MOTOR_T].info.angle)<115&&distance(lifting_control.measure,SAVE_LIFT)<300)
					auto_process.ground_process.step ++;
			break;
			case 2:
				/*对接*/
				ground_control.target_pitch = BACK_PITCH;
				if(distance(ground_control.measure_pitch,BACK_PITCH)<115)
				{
					auto_process.ground_process.step ++;
					autoprocess_delay = 0;
				}
			break;
			case 3:
					auto_process.clamp_process.process = 3;
					auto_process.clamp_process.step = 1;
			break;
			case 4:
				/*松矿*/
				ground_control.target_tranlation = GROUND_TRANLATION_GROUND;
				if(ground_control.measure_tranlation<16100)
				{
					/*清除延时并进入下一步*/
					autoprocess_delay = 0;
					auto_process.clamp_process.step ++;
					auto_process.ground_process.step ++;
				}
			break;
			case 5:
				/*回位*/
				ground_control.target_pitch = GROUND_PITCH_GROUND;
				if(distance(ground_control.measure_pitch,GROUND_PITCH_GROUND)<115)
					auto_process.ground_process.process = 0;
			break;
		}
}

 /**
 * @date        17-May-2022
 * @brief       下夹爪取障碍块
 */
void ground_barrier()
{
		switch(auto_process.ground_process.step)
		{
			case 1:
				/*抬起*/
				ground_control.target_pitch = BACK_BARRIER;
				lifting_control.target = SWITCH_LIFT;
			break;
			case 0:
				/*回位*/
				if(auto_process.barrier_flag)
					ground_control.target_pitch = BARRIER_PITCH_GROUND;
				else
					ground_control.target_pitch = 2591;
				lifting_control.target = 0;
			break;
		}		
}


 /**
 * @date        17-May-2022
 * @brief       下夹爪救援
 */
void ground_save()
{
		switch(auto_process.ground_process.step)
		{
			case 0:
				/*抬起*/
				ground_control.target_pitch = BACK_PITCH;
			break;
			case 1:
				if(auto_process.save_solider == 1)
				{
					ground_control.target_pitch = SAVE_PITCH_HERO;
					ground_control.target_tranlation = SAVE_TRANLATION_HERO;
				}
				else if(auto_process.save_solider == 2)
				{
					ground_control.target_pitch = SAVE_PITCH_STANDARD1;
					ground_control.target_tranlation = SAVE_TRANLATION_STANDARD1;
				}
				else if(auto_process.save_solider == 3)
				{
					ground_control.target_pitch = SAVE_PITCH_STANDARD2;
					ground_control.target_tranlation = SAVE_TRANLATION_STANDARD2;
				}
				else if(auto_process.save_solider == 4)
				{
					ground_control.target_pitch = SAVE_PITCH_STANDARD3;
					ground_control.target_tranlation = SAVE_TRANLATION_STANDARD3;
				}				
			break;
			case 2:
				/*救援*/
				ground_control.target_pitch += 300;
				auto_process.ground_process.step = 3;
			break;
		}		
}


 
