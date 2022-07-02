/**
 * @file        auto_process.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        6-April-2021
 * @brief       �Զ���
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
		/*ģʽ�л�(�ں��Զ�������)*/
		if(IS_SYSTEM_RESET&&rc_control.keyboard_mode)
			mode_switch_general();	
    osDelay(1);
  }
}


 
/*------------------process!=0,step=0��Ϊ�ȴ���process=0,step=0��Ϊ�˳���process!=0,step!=0��Ϊ���в���,process=0,step!=0���ִ���---------------------------*/

//��ط�״̬Ϊ��Ҫ��ʱ������ϵͳ��Ӧ̫��ᵼ�¹���


/*�Զ���������ʱ*/
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
 * @brief       һ��������
 */
void auto_process_general()
{	
	mode_switch.step = 1;
	/*��צ�Զ���*/
	clamp_auto_process();
	/*�ؿ��Զ���*/
	ground_auto_process();
	/*����Զ���*/
	cabin_auto_process();			
}

 /**
 * @date        6-April-2021
 * @brief       ��צһ��������
 */
void clamp_auto_process()
{
/*-------------------------------��צ�Զ���------------------------------------*/
	switch(auto_process.clamp_process.process)/*1���սӺ� 2��С��Դ��һ������ 3���ؿ�Խ� 4���һ� */
	{
		case 1:			/*1���п��Ĳ���*/
			clamp_after();
		break;
		case 2:			/*2��С��Դ��һ������*/
			clamp_sivler_Three();
		break;
		case 3:			/*3���ؿ�Խ�*/
			clamp_after_ground();
		break;
		case 4:		/*4���һ�*/
			clamp_conversion();		
		break;
		default:
			auto_process.clamp_process.step = 0;			
		break;
	}
		
}


 /**
 * @date        6-April-2021
 * @brief       �ؿ�һ��������
 */
void ground_auto_process()
{
/*-------------------------------�ؿ��Զ���------------------------------------*/	
	switch(auto_process.ground_process.process)/*1���ؿ�  2���ϰ���  3����Ԯ*/
	{
		case 1:		/*1���ؿ�*/
			ground_ground();
		break;	
		case 2:		/*2���ϰ���*/
			ground_barrier();
		break;
		case 3:		/*3����Ԯ*/
			ground_save();
		break;
		default:
			auto_process.ground_process.step = 0;	
		break;
	}		
}


 /**
 * @date        6-April-2021
 * @brief       ���һ��������
 */
uint64_t cabin_time=0;
void cabin_auto_process()
{
/*-------------------------------����Զ���------------------------------------*/
	if(!cabin_control.state_keylight_up&&cabin_control.state_keylight_down&&auto_process.cabin_process.process == 1)
		auto_process.cabin_process.process = 0;	
	
//	if(cabin_control.state_keylight_up&&auto_process.cabin_process.process == 2)
//		auto_process.cabin_process.process = 0;
	
	if(cabin_time >2000)
	{
		cabin_time = 0;
		auto_process.cabin_process.process = 0;
	}
	switch(auto_process.cabin_process.process)/*1���¿�  2���Ͽ�  3���󷭿�  4���ҷ���*/
	{
		case 1:/*�¿�*/
			cabin_time ++;
			cabin_control.target_L = keyboard_control.parameter.cabin_speed;	
			cabin_control.target_R = -keyboard_control.parameter.cabin_speed;
		break;
		case 2:/*�Ͽ�*/
			cabin_time ++;
			cabin_control.target_L = -keyboard_control.parameter.cabin_speed;	
			cabin_control.target_R = keyboard_control.parameter.cabin_speed;		
		break;
		case 3:/*�󷭿�*/
			cabin_control.target_L = keyboard_control.parameter.cabin_speed;	
			cabin_control.target_R = keyboard_control.parameter.cabin_speed;		
		break;
		case 4:/*�ҷ���*/
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
 * @brief       ���
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
			/*��ȡ*/
			clamp_control.state_pneu = ON;
			/*��ʱ500ms*/
			if(autoprocess_delay>300)
			{
				/*�����ʱ��������һ��*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 3:
			/*��צ*/
			clamp_control.target = MAX_CLAMP/2;		
			if(distance(clamp_control.measure, MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;
		break;
		case 4:
			/*���ƻص��м�*/
			tranlation_control.target = MID_TRANLASION;				
			if(distance(tranlation_control.measure,MID_TRANLASION)<115)
				auto_process.clamp_process.step ++;					
		break;
		case 5:
			autoprocess_delay ++;
			/*ǰ��*/
			lifting_control.state_pneu = OFF;
			if(autoprocess_delay>800&&!cabin_control.state_keylight_up)
			{
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 6:
			if(auto_process.emer)//Ӧ������
				auto_process.clamp_process.process = 0;
			else 
			{
				/*��צ��*/
				clamp_control.target = MAX_CLAMP;
				if(distance(clamp_control.measure,MAX_CLAMP)<115)
					auto_process.clamp_process.step ++;
			}
		break;
		case 7:
			/*�ɿ���ʯ*/
			clamp_control.state_pneu = OFF;
			autoprocess_delay ++;
			auto_process.cabin_process.process = 1;
			if(autoprocess_delay>300)
			{
				/*�����ʱ��������һ��*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 8:
			/*�����Զ���*/
			auto_process.clamp_process.process = 0;
//�˴���???
			if(!cabin_control.state_keylight_down||!cabin_control.state_keylight_up)//�ӵڶ�����
				/*�����ת����*/
				control.system_mode = NORMAL_MODE;
			else
				clamp_control.target = MAX_CLAMP/2;
		break;
			
			
		case 10:
			/*��צ*/
			clamp_control.target = GOLDERN_CALMP;		
			if(distance(clamp_control.measure, GOLDERN_CALMP)<115)
				auto_process.clamp_process.step = 2;			
		break;
	}
}	

 /**
 * @date        17-May-2022
 * @brief       һ������
 */
void clamp_sivler_Three()
{
	switch(auto_process.clamp_process.step)
	{
		case 1:
			autoprocess_delay ++;
			/*��ȡ*/
			clamp_control.state_pneu = ON;
			/*��ʱ500ms*/
			if(autoprocess_delay>300&&!cabin_control.state_keylight_up)
			{
				/*�����ʱ��������һ��*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 2:
			if(auto_process.emer)//Ӧ������
				auto_process.clamp_process.process = 0;
			else
			{
				/*��צ��*/
				clamp_control.target = MAX_CLAMP;
				if(distance(clamp_control.measure,MAX_CLAMP)<115)
					auto_process.clamp_process.step ++;
			}
		break;
		case 3:
			/*�ɿ���ʯ*/
			clamp_control.state_pneu = OFF;
			autoprocess_delay ++;
			auto_process.cabin_process.process = 1;
			if(autoprocess_delay>300)
			{
				/*�����ʱ��������һ��*/
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
			/*��ȡ*/
			clamp_control.state_pneu = ON;
			/*��ʱ500ms*/
			if(autoprocess_delay>300)
			{
				/*�����ʱ��������һ��*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 8:
			/*��צ*/
			clamp_control.target = MAX_CLAMP/2;		
			if(distance(clamp_control.measure, MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;
		break;
		case 9:
			/*���ƻص��м�*/
			tranlation_control.target = MID_TRANLASION;				
			if(distance(tranlation_control.measure,MID_TRANLASION)<115&&!cabin_control.state_keylight_up)
				auto_process.clamp_process.step ++;					
		break;
		case 10:
			if(!cabin_control.state_keylight_up)
			{
				/*��צ��*/
				clamp_control.target = MAX_CLAMP;
				if(distance(clamp_control.measure,MAX_CLAMP)<115)
					auto_process.clamp_process.step ++;
			}
		break;			
		case 11:
			/*�ɿ���ʯ*/
			clamp_control.state_pneu = OFF;
			autoprocess_delay ++;
			auto_process.cabin_process.process = 1;
			if(autoprocess_delay>300)
			{
				/*�����ʱ��������һ��*/
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
			/*��ȡ*/
			clamp_control.state_pneu = ON;
			/*��ʱ500ms*/
			if(autoprocess_delay>300)
			{
				/*�����ʱ��������һ��*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 16:
			/*��צ*/
			clamp_control.target = MAX_CLAMP/2;		
			if(distance(clamp_control.measure, MAX_CLAMP/2)<115)
				auto_process.clamp_process.step ++;
		break;
		case 17:
			/*���ƻص��м�*/
			tranlation_control.target = MID_TRANLASION;				
			if(distance(tranlation_control.measure,MID_TRANLASION)<115)
				auto_process.clamp_process.step ++;					
		break;	
		case 18:
			auto_process.ore_num = 3;
			clamp_control.target = 3750;	
			/*�����Զ���*/
			auto_process.clamp_process.process = 0;
		break;
	}	
}


 /**
 * @date        17-May-2022
 * @brief       �ؿ��
 */
void clamp_after_ground()
{
	switch(auto_process.clamp_process.step)
	{
		case 1:
			autoprocess_delay ++;
			/*�Խ�*/
			clamp_control.state_pneu = ON;
			if(autoprocess_delay>500)
				auto_process.ground_process.step ++;
		break;
		case 2:
			autoprocess_delay ++;
			/*��צ��*/
			clamp_control.target = MAX_CLAMP;
			if(distance(clamp_control.measure,MAX_CLAMP)<115&&autoprocess_delay>800)
				auto_process.clamp_process.step ++;
		break;
		case 3:
			/*�ɿ���ʯ*/
			clamp_control.state_pneu = OFF;
			autoprocess_delay ++;
			auto_process.cabin_process.process = 1;
			if(autoprocess_delay>300)
			{
				/*�����ʱ��������һ��*/
				autoprocess_delay = 0;
				auto_process.clamp_process.step ++;
			}
		break;
		case 5:
			/*�����Զ���*/
			auto_process.clamp_process.process = 0;
		break;
	}		
}

 /**
 * @date        17-May-2022
 * @brief       �һ�
 */
void clamp_conversion()
{
switch(auto_process.clamp_process.step)
	{
		case 0:	
			/*��������һ������*/
			auto_process.chassis_conversion_master = 1;
			if(auto_process.chassis_conversion_slaver == 1)
				auto_process.clamp_process.step ++;
		break;
		case 1:
			/*�ж��Ƿ�����*/
			if(auto_process.ore_num == 3||auto_process.emer)
			{
				auto_process.ore_num --;
				auto_process.clamp_process.step = 6;
			}
			else
				auto_process.clamp_process.step ++;
		break;
		/*2Ϊ�ȴ������һ��*/
		case 3:
			/*�����п�*/
			clamp_control.target = MAX_CLAMP;
			if(distance(clamp_control.measure,MAX_CLAMP)<115)
				auto_process.clamp_process.step ++;
		break;
		case 4:
			/*��ȡ*/
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
			/*ǰ��*/
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

		/*�ɿ�*/
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
		/*10Ϊ�ȴ����*/
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
			/*�Ƿ���տ��*/
			if(!cabin_control.state_keylight_up&&!cabin_control.state_keylight_down&&!clamp_control.state_keylight)
				mode_switch.mode = NORMAL_MODE;
			else
				/*�����ת����*/
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
 * @brief       �¼�צ�ؿ�
 */
void ground_ground()
{
		switch(auto_process.ground_process.step)
		{ 
			case 1:
				/*�п�*/
				ground_control.target_tranlation = MAX_GROUND_TRANLATION;
				lifting_control.target = SAVE_LIFT;
				if(distance(ground_control.measure_tranlation,16450)<115&&distance(lifting_control.measure,SAVE_LIFT)<300)
//				if(distance(ground.motor[GROUND_MOTOR_T].info.last_angle,ground.motor[GROUND_MOTOR_T].info.angle)<115&&distance(lifting_control.measure,SAVE_LIFT)<300)
					auto_process.ground_process.step ++;
			break;
			case 2:
				/*�Խ�*/
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
				/*�ɿ�*/
				ground_control.target_tranlation = GROUND_TRANLATION_GROUND;
				if(ground_control.measure_tranlation<16100)
				{
					/*�����ʱ��������һ��*/
					autoprocess_delay = 0;
					auto_process.clamp_process.step ++;
					auto_process.ground_process.step ++;
				}
			break;
			case 5:
				/*��λ*/
				ground_control.target_pitch = GROUND_PITCH_GROUND;
				if(distance(ground_control.measure_pitch,GROUND_PITCH_GROUND)<115)
					auto_process.ground_process.process = 0;
			break;
		}
}

 /**
 * @date        17-May-2022
 * @brief       �¼�צȡ�ϰ���
 */
void ground_barrier()
{
		switch(auto_process.ground_process.step)
		{
			case 1:
				/*̧��*/
				ground_control.target_pitch = BACK_BARRIER;
				lifting_control.target = SWITCH_LIFT;
			break;
			case 0:
				/*��λ*/
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
 * @brief       �¼�צ��Ԯ
 */
void ground_save()
{
		switch(auto_process.ground_process.step)
		{
			case 0:
				/*̧��*/
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
				/*��Ԯ*/
				ground_control.target_pitch += 300;
				auto_process.ground_process.step = 3;
			break;
		}		
}


 
