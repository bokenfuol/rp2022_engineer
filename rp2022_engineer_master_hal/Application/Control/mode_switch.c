/**
 * @file        mode_switch.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        10-March-2021
 * @brief       ģʽ�л�
 */
 
#include "control.h"
#include "mode_switch.h"
#include "auto_process.h"
#include "clamp_control.h"
#include "tranlation_control.h"
#include "lifting_control.h"
#include "cabin_control.h"
#include "ground_control.h"
#include "save_control.h"

 
 
switch_mode_t mode_switch = 
{
	.mode = NORMAL_MODE,
	.step = 1,
};
 

 
 
 
 /**
 * @date        10-March-2021
 * @brief       ģʽ�л�
 */
void mode_switch_general()
{
	 switch(mode_switch.mode)
	 {
		 case NORMAL_MODE:
			 enter_normal_mode();
		 break;
		 case AIR_MODE:
			 enter_air_mode();
		 break;
		 case GOLDENORE_MODE:
			 enter_goldenore_mode();
		 break;
		 case SILVERORE_MODE:
			 enter_silverore_mode();
		 break;
		 case GROUND_MODE:
			 enter_ground_mode();
		 break;
		 case SAVE_MODE:
			 enter_save_mode();
		 break;
		 case CONVERSION_MODE:
			 enter_conversion_mode();
		 break;
		 case	BARRIER_MODE:
			 enter_barrier_mode();
		 break;
		 case SPINNING_MODE:
			 enter_spnning_mode();
		 break;
		 case BLOCK_MODE:
			 enter_block_mode();
		 break;
	 }
}

 /**
 * @date        10-March-2021
 * @brief       ���볣��ģʽ
 */
void enter_normal_mode()
{
	switch(control.system_mode)
	{
		case NORMAL_MODE:
			auto_process_general();		
		break;
		default:
			auto_process.chassis_conversion_master = 0;
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:	
					/*��λ��Ԯ��*/
					save_init();
				
					/*ǰ��*/
					lifting_control.state_pneu = OFF;
							
					/*�жϵؿ�����Ƿ�λ*/
					if(distance(ground_control.measure_pitch,0)<115||lifting_control.measure>SWITCH_LIFT)
						mode_switch.step = 3;
					else
						mode_switch.step ++;	
				break;
				case 2:
					/*̧��*/
					lifting_control.target = SWITCH_LIFT;
					auto_process.cabin_process.process = 1;
					if(distance(lifting_control.measure,SWITCH_LIFT)<300)
						mode_switch.step ++;
				break;
				case 3:
					/*��λ �ؿ����������*/
					ground_init();
					if(distance(clamp_control.measure,0)<115)
						clamp_control.target = MAX_CLAMP/2;
					else
						tranlation_init();	
				
					if(distance(tranlation_control.measure,MID_TRANLASION)<115&&distance(ground_control.measure_pitch,0)<115&&\
					distance(ground_control.measure_tranlation,0)<115)
						mode_switch.step ++;	
				break;
				case 4:
						/*�ж��Ƿ�Я������*/
					if(auto_process.ore_num > 2)
							mode_switch.step = 7;	
						else
							mode_switch.step ++;	
				break;
					
				case 5:
					/*��λ��צ*/
					clamp_init();
					if(distance(clamp_control.measure,MAX_CLAMP)<115)
						mode_switch.step ++;
				break;
				case 6:
					/*��λ̧�����˳�ģʽת��*/
					lifting_control.target = 0;
					if(distance(lifting_control.measure,0)<115&&distance(save_control.measure,0)<115)
						control.system_mode = NORMAL_MODE;
				break;
				case 7:
					clamp_control.target = 3750;
					auto_process.cabin_process.process = 1;
					/*��λ̧�����˳�ģʽת��*/
					lifting_control.target = 13098;
					if(distance(lifting_control.measure,13098)<300&&distance(save_control.measure,0)<115&&distance(clamp_control.measure,3750)<115)
						control.system_mode = NORMAL_MODE;
				break;
			}
		}
}


 /**
 * @date        10-March-2021
 * @brief       ����ս�ģʽ
 */
void enter_air_mode()
{
	switch(control.system_mode)
	{
		case AIR_MODE:
			/*ģʽ�л���ɺ�����Զ���*/
			auto_process_general();			
		break;
		default:
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:
					/*��Ԯ����λ*/
					save_init();
					/*̧����λ*/
					lifting_control.target = AIR_LIFT;
					/*��צ�ſ�*/
					clamp_control.state_pneu = OFF;
					/*��צ*/
					clamp_control.target = AIR_CLAMP;
				
					if(lifting_control.measure>SWITCH_LIFT&&distance(clamp_control.measure,AIR_CLAMP)<115)
						mode_switch.step ++;
				break;
				case 2:
					/*��λ �ؿ����������*/
					ground_init();
					tranlation_init();
				
					if(distance(tranlation_control.measure,MID_TRANLASION)<115&&distance(ground_control.measure_pitch,0)<115&&
					distance(ground_control.measure_tranlation,0)<115)
						mode_switch.step ++ ;
				break;
				case 3:
					/*ǰ�����*/
					lifting_control.state_pneu = ON;
					
					if(distance(lifting_control.measure,AIR_LIFT)<300&&distance(save_control.measure,0)<115)
					{
						control.system_mode = AIR_MODE;
						auto_process.clamp_process.process = 1;
						auto_process.clamp_process.step = 1;
					}
				break;
			}
		break;
	}
}

 /**
 * @date        10-March-2021
 * @brief       ������ģʽ
 */
void enter_goldenore_mode()
{
	switch(control.system_mode)
	{
		case GOLDENORE_MODE:
			/*ģʽ�л���ɺ�����Զ���*/
			auto_process_general();
		break;
		default:
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:
					/*��Ԯ����λ*/
					save_init();   
					
					/*��צ*/
					clamp_control.target = MAX_CLAMP/2;
								
					if(distance(clamp_control.measure,MAX_CLAMP/2)<115)
					{
						if(distance(ground_control.measure_pitch,0)<115)
							mode_switch.step = 4;
						else if(lifting_control.measure>SWITCH_LIFT)
							mode_switch.step = 3;
						else
							mode_switch.step ++;
					}			
				break;
				case 2:
					/*̧��*/
					lifting_control.target = SWITCH_LIFT;
				
					if(distance(lifting_control.measure,SWITCH_LIFT)<300)
						mode_switch.step ++;					
				break;	
				case 3:
					/*��λ �ؿ����*/
					ground_init();
				
					if(distance(ground_control.measure_pitch,0)<115&&distance(ground_control.measure_tranlation,0)<115)
						mode_switch.step ++ ;
				break;
				case 4:
					/*���Ƹ�λ*/
					tranlation_init();
				
					if(distance(tranlation_control.measure,MID_TRANLASION)<115)
						mode_switch.step ++ ;
				break;
				case 5:
					/*̧��*/
					lifting_control.target = GOLDERN_LIFT;
					if(distance(lifting_control.measure,GOLDERN_LIFT)<300)
						mode_switch.step ++ ;
				break;
				case 6:
					/*ǰ�����*/
					lifting_control.state_pneu = ON;	
					clamp_control.state_pneu = OFF;
					if(distance(save_control.measure,0)<115)	
					{
						control.system_mode = GOLDENORE_MODE;
						auto_process.clamp_process.process = 1;
						auto_process.clamp_process.step = 0;
					}						
				break;
			}
		break;
	}
}

 /**
 * @date        10-March-2021
 * @brief       ��������ģʽ
 */
void enter_silverore_mode()
{
	switch(control.system_mode)
	{
		case SILVERORE_MODE:
			/*ģʽ�л���ɺ�����Զ���*/
			auto_process_general();			
		break;
		default:
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:
					/*��Ԯ����λ*/
					save_init();
					/*̧����λ*/
					lifting_control.target = SILVER_LIFT;
					/*��צ�ſ�*/
					clamp_control.state_pneu = OFF;
				
					lifting_control.state_pneu = OFF;
					/*��צ*/
					clamp_control.target = MAX_CLAMP/2;
				
					if(lifting_control.measure>SWITCH_LIFT&&distance(clamp_control.measure,MAX_CLAMP/2)<115)
						mode_switch.step ++;
				break;
				case 2:
					/*��λ �ؿ����������*/
					ground_init();
					tranlation_init();
				
					if(distance(tranlation_control.measure,MID_TRANLASION)<115&&distance(ground_control.measure_pitch,0)<115&&
					distance(ground_control.measure_tranlation,0)<115)
						mode_switch.step ++ ;
				break;
				case 3:
					/*��צ*/
					clamp_control.target = SILVER_CLAMP;					
					if(distance(lifting_control.measure,SILVER_LIFT)<300&&distance(save_control.measure,0)<115&&distance(clamp_control.measure,SILVER_CLAMP)<115)
					{
						control.system_mode = SILVERORE_MODE;
						auto_process.clamp_process.process = 2;
						auto_process.clamp_process.step = 0;
					}
				break;
			}
		break;
	}
}

 /**
 * @date        10-March-2021
 * @brief       ����ؿ�ģʽ
 */
void enter_ground_mode()
{	
	switch(control.system_mode)
	{
		case GROUND_MODE:
			/*ģʽ�л���ɺ�����Զ���*/
			auto_process_general();					
		break;
		default:
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:	
					/*��λ��Ԯ��*/
					save_init();
				
					/*ǰ��*/
					lifting_control.state_pneu = OFF;
				
					clamp_control.state_pneu = OFF;

					/*�жϵؿ�����������Ƿ�λ*/
					if(distance(ground_control.measure_pitch,0)>115||lifting_control.measure>SWITCH_LIFT)
						mode_switch.step = 3;	
					else
						mode_switch.step ++;
				break;
				case 2:
					/*̧��*/
					lifting_control.target = SWITCH_LIFT;
				
					if(distance(lifting_control.measure,SWITCH_LIFT)<300)
						mode_switch.step ++;
				break;
				case 3:
					/*��λ �ؿ����������*/
					ground_control.target_pitch = GROUND_PITCH_GROUND;
					ground_control.target_tranlation = GROUND_TRANLATION_GROUND;
					if(distance(clamp_control.measure,0)<115)
						clamp_control.target = MAX_CLAMP/2;
					else
						tranlation_init();	
				
					if(distance(tranlation_control.measure,MID_TRANLASION)<115&&distance(ground_control.measure_pitch,GROUND_PITCH_GROUND)<115&&
					distance(ground_control.measure_tranlation,GROUND_TRANLATION_GROUND)<115)
						mode_switch.step ++;	
				break;
				case 4:
					/*��צ*/
					clamp_control.target = 0;
					/*��λ̧�����˳�ģʽת��*/
					lifting_control.target = 0;
			
					if(distance(lifting_control.measure,0)<300&&distance(save_control.measure,0)<115&&distance(clamp_control.measure,0)<115)
					{
						control.system_mode = GROUND_MODE;
						auto_process.ground_process.process = 1;
						auto_process.ground_process.step = 0;
					}
				break;
			}
		break;
	}
}
	
 /**
 * @date        10-March-2021
 * @brief       ����һ�ģʽ
 */
void enter_conversion_mode()
{
	switch(control.system_mode)
	{
		case CONVERSION_MODE:
			/*ģʽ�л���ɺ�����Զ���*/
			auto_process_general();
		break;
		default:
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:	
					/*��λ��Ԯ��*/
					save_init();
				
					/*ǰ��*/
					lifting_control.state_pneu = OFF;
				
					lifting_control.target = CONVERSION_LIFT;

					/*̧��*/
					if(distance(lifting_control.measure,CONVERSION_LIFT)<300)
						mode_switch.step ++;
				break;
				case 2:
					/*��λ �ؿ����������*/
					ground_init();
					if(distance(clamp_control.measure,0)<115)
						clamp_control.target = MAX_CLAMP/2;
					else
						tranlation_init();

					if(distance(tranlation_control.measure,MID_TRANLASION)<115&&distance(ground_control.measure_pitch,0)<115&&\
					distance(ground_control.measure_tranlation,0)<115)
						mode_switch.step ++ ;	
				break;
				case 3:
					/*��λ��צ*/
					clamp_control.target = MAX_CLAMP/2;
					if(distance(clamp_control.measure,MAX_CLAMP/2)<115)
						mode_switch.step ++;
				break;
				case 4:
					/*��λ̧�����˳�ģʽת��*/
					if(distance(save_control.measure,0)<115)
					{
						control.system_mode = CONVERSION_MODE;
						auto_process.clamp_process.process = 4;
						auto_process.clamp_process.step = 0;
					}
				break;
				}
		break;
	}
}
	
 /**
 * @date        10-March-2021
 * @brief       �����ģʽ
 */
void enter_block_mode()
{
	switch(control.system_mode)
	{
		case BLOCK_MODE:
			/*ģʽ�л���ɺ�����Զ���*/
			auto_process_general();
		break;
		default:
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:	
					/*��λ��Ԯ��*/
					save_init();
				
					/*ǰ��*/
					lifting_control.state_pneu = OFF;
							
					/*�жϵؿ�����������Ƿ�λ*/
					if(distance(ground_control.measure_pitch,0)<115||lifting_control.measure>SWITCH_LIFT)
						mode_switch.step = 3;	
					else
						mode_switch.step ++;
				break;
				case 2:
					/*̧��*/
					lifting_control.target = SWITCH_LIFT;
				
					if(distance(lifting_control.measure,SWITCH_LIFT)<300)
						mode_switch.step ++;
				break;
				case 3:
					/*��λ �ؿ����������*/
					ground_init();
					if(distance(clamp_control.measure,0)<115)
						clamp_control.target = MAX_CLAMP/2;
					else
						tranlation_init();
				
					if(distance(tranlation_control.measure,MID_TRANLASION)<115&&distance(ground_control.measure_pitch,0)<115&&
					distance(ground_control.measure_tranlation,0)<115)
						mode_switch.step ++ ;	
				break;
				case 4:
					/*�ж��Ƿ�Я������*/
					if(clamp_control.state_keylight&&cabin_control.state_keylight_down&&cabin_control.state_keylight_up&&lifting_control.measure>SWITCH_LIFT)
						mode_switch.step = 6;	
					else
						mode_switch.step ++;	
				break;
					
				case 5:
					/*��λ��צ*/
					clamp_init();
					if(distance(clamp_control.measure,MAX_CLAMP)<115)
						mode_switch.step ++;
				break;
				case 6:
					/*��λ̧�����˳�ģʽת��*/
					lifting_control.target = BLOCK_LIFT;
					if(distance(lifting_control.measure,BLOCK_LIFT)<300&&distance(save_control.measure,0)<115)
						control.system_mode = BLOCK_MODE;
				break;
			}
		break;
	}
}

 /**
 * @date        10-March-2021
 * @brief       �����Ԯģʽ
 */
void enter_save_mode()
{
	switch(control.system_mode)
	{
		case SAVE_MODE:
			/*ģʽ�л���ɺ�����Զ���*/
			auto_process_general();					
		break;
		default:
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:	
					/*��λ��Ԯ��*/
					save_init();
				
					/*ǰ��*/
					lifting_control.state_pneu = OFF;
				
					clamp_control.state_pneu = OFF;
							
					/*�жϵؿ�����������Ƿ�λ*/
					if(distance(ground_control.measure_pitch,0)>115||lifting_control.measure>SWITCH_LIFT)
						mode_switch.step = 3;	
					else
						mode_switch.step ++;
				break;
				case 2:
					/*̧��*/
					lifting_control.target = SWITCH_LIFT;
				
					if(distance(lifting_control.measure,SWITCH_LIFT)<300)
						mode_switch.step ++;
				break;
				case 3:
					/*��λ �ؿ����/��Ԯ*/
					ground_control.target_pitch = SAVE_PITCH_HERO;
					ground_control.target_tranlation = SAVE_TRANLATION_HERO;
					if(distance(clamp_control.measure,0)<115)
						clamp_control.target = MAX_CLAMP/2;
					else
						tranlation_init();
				
					if(distance(tranlation_control.measure,MID_TRANLASION)<115&&distance(ground_control.measure_pitch,SAVE_PITCH_HERO)<115&&
					distance(ground_control.measure_tranlation,SAVE_TRANLATION_HERO)<115)
						mode_switch.step ++;	
				break;			
				case 4:
					/*��λ��צ*/
					clamp_init();
					if(distance(clamp_control.measure,MAX_CLAMP)<115)
						mode_switch.step ++;
				break;
				case 5:
					/*��λ̧�����˳�ģʽת��*/
					lifting_control.target = 0;
					if(distance(lifting_control.measure,0)<300&&distance(save_control.measure,0)<115)
					{
						auto_process.save_solider = 1;
						control.system_mode = SAVE_MODE;
						auto_process.ground_process.process = 3;
						auto_process.ground_process.step = 0;
					}
				break;
			}
		break;
	}
}


 /**
 * @date        10-March-2021
 * @brief       �����ϰ���ģʽ
 */
void enter_barrier_mode()
{
	switch(control.system_mode)
	{
		case BARRIER_MODE:
			/*ģʽ�л���ɺ�����Զ���*/
			auto_process_general();					
		break;
		default:
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:	
					/*��λ��Ԯ��*/
					save_init();
				
					/*ǰ��*/
					lifting_control.state_pneu = OFF;
				
					clamp_control.state_pneu = OFF;
							
					/*�жϵؿ�����������Ƿ�λ*/
					if(distance(ground_control.measure_pitch,0)>115||lifting_control.measure>SWITCH_LIFT)
						mode_switch.step = 3;	
					else
						mode_switch.step ++;
				break;
				case 2:
					/*̧��*/
					lifting_control.target = SWITCH_LIFT;
				
					if(distance(lifting_control.measure,SWITCH_LIFT)<300)
						mode_switch.step ++;
				break;
				case 3:
					/*��λ �ؿ����/��Ԯ*/
					ground_control.target_pitch = BARRIER_PITCH_GROUND;
					ground_control.target_tranlation = BARRIER_TRANLATION_GROUND;
					if(distance(clamp_control.measure,0)<115)
						clamp_control.target = MAX_CLAMP/2;
					else
						tranlation_init();
				
					if(distance(tranlation_control.measure,MID_TRANLASION)<115&&distance(ground_control.measure_pitch,BARRIER_PITCH_GROUND)<115&&
					distance(ground_control.measure_tranlation,BARRIER_TRANLATION_GROUND)<115)
						mode_switch.step ++;	
				break;
				case 4:	
					/*��λ��צ*/
					clamp_init();
					if(distance(clamp_control.measure,MAX_CLAMP)<115)
						mode_switch.step ++;
				break;
				case 5:
					/*��λ̧�����˳�ģʽת��*/
					lifting_control.target = 0;
					if(distance(lifting_control.measure,0)<300&&distance(save_control.measure,0)<115)
					{
						control.system_mode = BARRIER_MODE;
						auto_process.ground_process.process = 2;
						auto_process.ground_process.step = 0;						
					}
				break;
			}
		break;
	}
}


 /**
 * @date        10-March-2021
 * @brief       ��������ģʽ
 */
void enter_spnning_mode()
{	
	switch(control.system_mode)
	{
		case SPINNING_MODE:
			/*ģʽ�л���ɺ�����Զ���*/
			auto_process_general();
		break;
		default:
			auto_process.ground_process.process = 0;
			auto_process.clamp_process.process = 0;
			switch(mode_switch.step)
			{
				case 1:	
					/*��λ��Ԯ��*/
					save_init();
				
					/*ǰ��*/
					lifting_control.state_pneu = OFF;
				
					clamp_control.state_pneu = OFF;
							
					/*�жϵؿ�����Ƿ�λ*/
					if(distance(ground_control.measure_pitch,0)<115||lifting_control.measure>SWITCH_LIFT)
						mode_switch.step = 3;
					else
						mode_switch.step ++;	
				break;
				case 2:
					/*̧��*/
					lifting_control.target = SWITCH_LIFT;
				
					if(distance(lifting_control.measure,SWITCH_LIFT)<300)
						mode_switch.step ++;
				break;
				case 3:
					/*��λ �ؿ����������*/
					ground_init();
					if(distance(clamp_control.measure,0)<115)
						clamp_control.target = MAX_CLAMP/2;
					else
						tranlation_init();	
				
					if(distance(tranlation_control.measure,MID_TRANLASION)<115&&distance(ground_control.measure_pitch,0)<115&&\
					distance(ground_control.measure_tranlation,0)<115)
						mode_switch.step ++;	
				break;				
				case 4:
					/*��λ��צ*/
					clamp_init();
					if(distance(clamp_control.measure,MAX_CLAMP)<115)
						mode_switch.step ++;
				break;
				case 5:
					/*��λ̧�����˳�ģʽת��*/
					lifting_control.target = 0;
					if(distance(lifting_control.measure,0)<115&&distance(save_control.measure,0)<115)
						control.system_mode = SPINNING_MODE;
				break;
			}
		break;
	}
}
