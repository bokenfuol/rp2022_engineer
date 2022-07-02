/**
 * @file        keyboard.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        13-November-2021
 * @brief       ����
 */

#include "mode_switch.h"
#include "auto_process.h"
#include "keyboard.h"
#include "control.h"
#include "clamp_control.h"
#include "ground_control.h"
#include "lifting_control.h"
#include "cabin_control.h"
#include "tranlation_control.h"
#include "save_control.h"
#include "filter.h"


keyboard_control_t keyboard_control = {
	/*���̲����б���*/
	.car_lock = OFF,
	.parameter = {
		.clamp = 25/660.f,			//��צ����				
		.tranlation = 25/660.f,			 	//���Ʋ���
		.lifting = 25/660.f,					//̧������
		.cabin_speed = 5000,					//��ֵ���ٶ�
		.ground_tranlation = 25/660.f,//�ؿ����
		.ground_pitch = 25/660.f	
	},
};



/*������Ӧʱ��*/
keyboard_sensor_t keyboard_sensor = {
	.keyboard = {
		[KEY_W] = {.shortdown_time = 300,},
		[KEY_S] = {.shortdown_time = 300,},
		[KEY_A] = {.shortdown_time = 300,},
		[KEY_D] = {.shortdown_time = 300,},
		[KEY_Shift] = {.shortdown_time = 50,},
		[KEY_Ctrl] = {.shortdown_time = 50,},
		[KEY_Q] = {.shortdown_time = 50,},
		[KEY_E] = {.shortdown_time = 50,},
		[KEY_F] = {.shortdown_time = 150,},
		[KEY_G] = {.shortdown_time = 300,},
		[KEY_Z] = {.shortdown_time = 300,},
		[KEY_X] = {.shortdown_time = 300,},
		[KEY_C] = {.shortdown_time = 300,},
		[KEY_V] = {.shortdown_time = 300,},
		[KEY_B] = {.shortdown_time = 300,},
	},
	.mouse = {
		[MOUSE_LEFT] = {.shortdown_time = 100,},
		[MOUSE_RIGHT] = {.shortdown_time = 100,},
	},
	.update = keyboard_update,
	.check = keyboard_check,

};


/**
 * @date        3-November-2021
 * @brief       ���̿���
 */
void Keyboard_control()
{
	keyboard_sensor.update(&keyboard_sensor,&rc_sensor);
	keyboard_sensor.check(&keyboard_sensor,&rc_sensor);
}


/**
 * @date        13-November-2021
 * @brief       keyboard_update
 */
int16_t filter_vx[LEN_FILTER],filter_vy[LEN_FILTER],filter_vz[LEN_FILTER];/*�����˲�����*/
uint32_t key_time = 0;/*����ʱ����*/
uint32_t shake_time[KEYBOARD_CNT+MOUSE_CNT];/*������ʱ��*/
uint16_t max_shake_time=30;/*�����ж����ֵ*/
void keyboard_update(keyboard_sensor_t *key,rc_sensor_t *rc)
{
	key_time ++;
	/*���̷�����*/
	for(uint8_t i = 0 ; i < KEYBOARD_CNT; i++)
	{
		if((rc->key_v>>i) & 0x01)
			shake_time[i] ++;
		else
			shake_time[i] = 0;
		if(shake_time[i]>max_shake_time)
			key->keyboard[i].value = 1;
		else
			key->keyboard[i].value = 0;
	}
	/*��������*/
	if(IF_MOUSE_PRESSED_LEFT)
		shake_time[KEYBOARD_CNT] ++;
	else
		shake_time[KEYBOARD_CNT] = 0;
	if(shake_time[KEYBOARD_CNT]>max_shake_time)
		key->mouse[MOUSE_LEFT].value = 1;
	else 
		key->mouse[MOUSE_LEFT].value = 0;
	/*��������*/
	if(IF_MOUSE_PRESSED_RIGHT)
		shake_time[KEYBOARD_CNT+1] ++;
	else
		shake_time[KEYBOARD_CNT+1] = 0;
	if(shake_time[KEYBOARD_CNT+1]>max_shake_time)
		key->mouse[MOUSE_RIGHT].value = 1;
	else 
		key->mouse[MOUSE_RIGHT].value = 0;
}


/**
 * @date        13-November-2021
 * @brief       keyboard_check
 */
void keyboard_check(keyboard_sensor_t *key,rc_sensor_t *rc)
{
	/*�����̽ṹ��*/
//	keyboard_W_check(rc,&key->keyboard[KEY_W]);
//	keyboard_S_check(rc,&key->keyboard[KEY_S]);
//	keyboard_A_check(rc,&key->keyboard[KEY_A]);
//	keyboard_D_check(rc,&key->keyboard[KEY_D]);
	keyboard_Q_check(rc,&key->keyboard[KEY_Q]);
	keyboard_E_check(rc,&key->keyboard[KEY_E]);
	keyboard_Shift_check(rc,&key->keyboard[KEY_Shift]);
	keyboard_Ctrl_check(rc,&key->keyboard[KEY_Ctrl]);
	keyboard_R_check(rc,&key->keyboard[KEY_R]);
	keyboard_F_check(rc,&key->keyboard[KEY_F]);
	keyboard_G_check(rc,&key->keyboard[KEY_G]);
	keyboard_Z_check(rc,&key->keyboard[KEY_Z]);
	keyboard_X_check(rc,&key->keyboard[KEY_X]);
	keyboard_C_check(rc,&key->keyboard[KEY_C]);
	keyboard_V_check(rc,&key->keyboard[KEY_V]);
	keyboard_B_check(rc,&key->keyboard[KEY_B]);
	mouse_left_check(rc,&key->mouse[MOUSE_LEFT]);
	mouse_right_check(rc,&key->mouse[MOUSE_RIGHT]);		
}



/*----------------���������ṹ��-------------------*/
///**
// * @date        7-April-2022
// * @brief       W��
// */
//void keyboard_W_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
//{
//	keyboard_judge(rc_sen,rc_key);
//	switch (rc_key->keyboard_state)
//	{
//		case UP:
//		break;
//		case PRESS:
//		break;
//		case SHORT_DOWN:
//		break;
//		case DOWN:
//		break;
//		case RELEASE:
//		break;
//	}
//}


///**
// * @date        7-April-2022
// * @brief       S��
// */
//void keyboard_S_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
//{
//	keyboard_judge(rc_sen,rc_key);
//	switch (rc_key->keyboard_state)
//	{
//		case UP:
//		break;
//		case PRESS:
//		break;
//		case SHORT_DOWN:
//		break;
//		case DOWN:
//		break;
//		case RELEASE:
//		break;
//	}
//}


///**
// * @date        7-April-2022
// * @brief       A��
// */
//void keyboard_A_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
//{
//	keyboard_judge(rc_sen,rc_key);
//	switch (rc_key->keyboard_state)
//	{
//		case UP:
//		break;
//		case PRESS:
//		break;
//		case SHORT_DOWN:
//		break;
//		case DOWN:
//		break;
//		case RELEASE:
//		break;
//	}
//}


///**
// * @date        7-April-2022
// * @brief       D��
// */
//void keyboard_D_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
//{
//	keyboard_judge(rc_sen,rc_key);
//	switch (rc_key->keyboard_state)
//	{
//		case UP:
//		break;
//		case PRESS:
//		break;
//		case SHORT_DOWN:
//		break;
//		case DOWN:
//		break;
//		case RELEASE:
//		break;
//	}
//}

/**
 * @date        7-April-2022
 * @brief       Q��
 */
void keyboard_Q_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
		break;
		case PRESS:
			//Shift+Q������ģʽ
			if(keyboard_control.special_shift)
				/*�����ת����*/
				mode_switch.mode = NORMAL_MODE;
			else if(keyboard_control.special_reset)
			{
				control.master_reset = RESET_OK;
				control.system_mode = NORMAL_MODE;
				clamp_control.target = 0;
				tranlation_control.target = 0;
				lifting_control.target = 0;
				cabin_control.target_L = 0;
				cabin_control.measure_R = 0;
				ground_control.target_pitch = 0;
				ground_control.measure_tranlation = 0;
				clamp.motor[CLAMP_MOTOR_LEFT].info.total_angle = 0;
				tranlation.motor[TRANLATION_MOTOR].info.total_angle = 0;
				lifting.motor[LIFTING_MOTOR_L].info.total_angle = 0;
				lifting.motor[LIFTING_MOTOR_R].info.total_angle = 0;
				ground.motor[GROUND_MOTOR_L].info.total_angle = 0;
				ground.motor[GROUND_MOTOR_T].info.total_angle = 0;
				save.motor[SAVE_CARD_MOTOR].info.total_angle = 0;
			}
			else
				switch(control.system_mode)
				{
					case AIR_MODE:
					case SILVERORE_MODE:
					case GOLDENORE_MODE:
					case CONVERSION_MODE:
					case GROUND_MODE:
						/*�󷭿�*/
						auto_process.cabin_process.process = 3;
					break;
					case SAVE_MODE:
						auto_process.save_solider --;
						if(auto_process.save_solider == 0)
							auto_process.save_solider = 4;	

					break;
					default:
					break;
				}
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
			if(control.system_mode == NORMAL_MODE)
				lifting_control.target += 10;
		break;
		case RELEASE:
			auto_process.cabin_process.process = 0;
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       E��
 */
void keyboard_E_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
		break;
		case PRESS:
			//Shift+E���ս�ģʽ
			if(keyboard_control.special_shift)
				/*�����ת����*/
				mode_switch.mode = AIR_MODE;
			else if(keyboard_control.special_ctrl)
				mode_switch.mode = NORMAL_MODE;
			else
				switch(control.system_mode)
				{
					case AIR_MODE:
					case SILVERORE_MODE:
					case GOLDENORE_MODE:
					case CONVERSION_MODE:
					case GROUND_MODE:
						/*�ҷ���*/
						auto_process.cabin_process.process = 4;
					break;
					case SAVE_MODE:
						auto_process.save_solider ++;
						if(auto_process.save_solider == 5)
							auto_process.save_solider = 1;
					break;
					default:
					break;
				}				
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
			if(control.system_mode == NORMAL_MODE)
				lifting_control.target -= 10;
		break;
		case RELEASE:
			auto_process.cabin_process.process = 0;
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       Shift��
 */
void keyboard_Shift_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
			keyboard_control.special_shift = OFF;
		break;
		case PRESS:
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
			keyboard_control.special_shift = ON;
		break;
		case RELEASE:
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       Ctrl��
 */
void keyboard_Ctrl_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
			keyboard_control.special_ctrl = OFF;
		break;
		case PRESS:
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
			keyboard_control.special_ctrl = ON;
		break;
		case RELEASE:
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       R��
 */
void keyboard_R_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
		break;
		case PRESS:
			//Shift+R������ģʽ
			if(keyboard_control.special_shift)
				/*�����ת����*/
				mode_switch.mode = SILVERORE_MODE;
			else if(keyboard_control.special_ctrl)
				mode_switch.mode = NORMAL_MODE;				
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       F��
 */
void keyboard_F_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
		break;
		case PRESS:
			//Shift+F�����ģʽ
			if(keyboard_control.special_shift)
				/*�����ת����*/
				mode_switch.mode = GOLDENORE_MODE;
			else if(keyboard_control.special_ctrl)
				mode_switch.mode = NORMAL_MODE;	
			else
			{				
//���Զ����Ѿ�ֹͣ��ͨ��ģʽת����͵��
				if(!auto_process.cabin_process.process&&!auto_process.clamp_process.process&&!auto_process.ground_process.process)
					control.system_mode = NORMAL_MODE;					
				else//ֹͣ�Զ���
				{
					auto_process.cabin_process.process = 0;
					auto_process.clamp_process.process = 0;
					auto_process.ground_process.process = 0;
					auto_process.chassis_conversion_master = 0;
				}
			}				
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       G��
 */
void keyboard_G_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:		
		break;
		case PRESS:
			//Shift+G���ؿ�ģʽ
			if(keyboard_control.special_shift)
				/*�����ת����*/
				mode_switch.mode = GROUND_MODE;
			else if(keyboard_control.special_ctrl)
				mode_switch.mode = NORMAL_MODE;
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       Z��
 */
void keyboard_Z_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
		break;
		case PRESS:
			//Shift+G����ģʽ
			if(keyboard_control.special_shift)
				/*�����ת����*/
				mode_switch.mode = BLOCK_MODE;
			else if(keyboard_control.special_ctrl)
				mode_switch.mode = NORMAL_MODE;
			else
				switch(control.system_mode)
				{
					case AIR_MODE:
					case SILVERORE_MODE:
					case GOLDENORE_MODE:
					case CONVERSION_MODE:
					case GROUND_MODE:
						/*�Ͽ�*/
						auto_process.cabin_process.process = 2;
					break;
					default:
					break;
				}
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
			auto_process.cabin_process.process = 0;		
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       X��
 */
void keyboard_X_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:		
		break;
		case PRESS:	
			//Shift+X���һ�ģʽ
			if(keyboard_control.special_shift)
				/*�����ת����*/
				mode_switch.mode = CONVERSION_MODE;
			else if(keyboard_control.special_ctrl)
				mode_switch.mode = NORMAL_MODE;
			else
				switch(control.system_mode)
				{
					case AIR_MODE:
					case SILVERORE_MODE:
					case GOLDENORE_MODE:
					case CONVERSION_MODE:
					case GROUND_MODE:
						/*�¿�*/
						auto_process.cabin_process.process = 1;
					break;
					default:
					break;
				}
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
			auto_process.cabin_process.process = 0;		
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       C��
 */
void keyboard_C_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
		break;
		case PRESS:
			//Shift+C���ϰ���ģʽ
			if(keyboard_control.special_shift)
				/*�����ת����*/
				mode_switch.mode = BARRIER_MODE;
			else if(keyboard_control.special_ctrl)
				mode_switch.mode = NORMAL_MODE;	
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
	break;
	}
}


/**
 * @date        7-April-2022
 * @brief       V��
 */
void keyboard_V_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
		break;
		case PRESS:
			//Shift+V����Ԯģʽ
			if(keyboard_control.special_shift)
				/*�����ת����*/
				mode_switch.mode = SAVE_MODE;
			else if(keyboard_control.special_ctrl)
				mode_switch.mode = NORMAL_MODE;
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief       B��
 */
void keyboard_B_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:	
		break;
		case PRESS:
			if(keyboard_control.special_reset)
			{				
				//reset CPU
				__set_FAULTMASK(1);
				NVIC_SystemReset();
			}	
			else
			{
				//Shift+B��ԭ��С����ģʽ
				if(keyboard_control.special_shift)
					/*�����ת����*/
					mode_switch.mode = SPINNING_MODE;
				else if(keyboard_control.special_ctrl)
					mode_switch.mode = NORMAL_MODE;				
			}
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
		break;
	}
}


/**
 * @date        7-April-2022
 * @brief      ������
 */
void mouse_left_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_mouse)
{
	keyboard_judge(rc_sen,rc_mouse);
	switch (rc_mouse->keyboard_state)
	{
		case UP:
		break;
		case PRESS:
			switch(control.system_mode)
			{
				case GOLDENORE_MODE:
					if(auto_process.clamp_process.step == 0)
					{
						auto_process.clamp_process.step = 10;
						autoprocess_delay = 0;
					}	
				break;
				case SILVERORE_MODE:
					if(auto_process.clamp_process.step == 0)
					{
						auto_process.clamp_process.step = 1;
						autoprocess_delay = 0;
					}
				break;
				case BARRIER_MODE:
					(auto_process.ground_process.step == 0)?(auto_process.ground_process.step = 1):(auto_process.ground_process.step = 0);
				break;
				case SAVE_MODE:
					auto_process.ground_process.step ++;
					if(auto_process.ground_process.step >= 3)
						auto_process.ground_process.step = 0;		
				break;
				case GROUND_MODE:
					if(auto_process.ground_process.step == 0)
					{
						auto_process.ground_process.step = 1;
						autoprocess_delay = 0;
					}	
					else if(auto_process.ground_process.step == 1)
						auto_process.ground_process.step = 2;
				break;
				case CONVERSION_MODE:
					if(auto_process.clamp_process.step == 2)
						auto_process.clamp_process.step = 3;
					else if(auto_process.clamp_process.step == 10)
						auto_process.clamp_process.step = 11;
					else if(auto_process.clamp_process.step == 7)
						auto_process.clamp_process.step = 8;
				break;
				case AIR_MODE:
					/*��һ����λ*/
					if(distance(tranlation_control.measure,MID_TRANLASION)<115)
						tranlation_control.target = 100;
					else if(distance(tranlation_control.measure,22000)<115)
						tranlation_control.target = MID_TRANLASION;
				break;
				default:
				break;
			}
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
		break;
	}	
}


/**
 * @date        7-April-2022
 * @brief      ����Ҽ�
 */
void mouse_right_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_mouse)
{
	keyboard_judge(rc_sen,rc_mouse);
	switch (rc_mouse->keyboard_state)
	{
		case UP:
			keyboard_control.special_reset = OFF;
		break;
		case PRESS:
			if(keyboard_control.special_ctrl&&keyboard_control.special_shift)
				auto_process.emer = !auto_process.emer;
			else
				switch(control.system_mode)
				{
					case SAVE_MODE:
						if(distance(save_control.measure,0)<500)
						{
							if(auto_process.save_solider == 1)
								save_control.target = SAVE_CARD_HERO;
							else if(auto_process.save_solider == 2)
								save_control.target = SAVE_CARD_STANDARD1;
							else if(auto_process.save_solider == 3)
								save_control.target = SAVE_CARD_STANDARD2;
							else if(auto_process.save_solider == 4)
								save_control.target = SAVE_CARD_STANDARD3;	
						}
						else
							save_control.target = 0;
					break;
					case BARRIER_MODE:	
						auto_process.barrier_flag = !auto_process.barrier_flag;
					break;
					case CONVERSION_MODE:
						if(auto_process.clamp_process.step == 2)
							auto_process.clamp_process.step = 20;
						else if(auto_process.clamp_process.step == 20)
							auto_process.clamp_process.step = 21;
					break;
					case AIR_MODE:
						/*��һ����λ*/
						if(distance(tranlation_control.measure,MID_TRANLASION)<115)
							tranlation_control.target = 22000;
						else if(distance(tranlation_control.measure,100)<115)
							tranlation_control.target = MID_TRANLASION;
					break;
					default:
					break;
				}
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
			if(keyboard_control.special_ctrl)
				keyboard_control.special_reset = ON;
		break;
		case RELEASE:
		break;
	}	
}


/**
 * @date        7-April-2022
 * @brief      ����״̬�ж�
 */
void keyboard_judge(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	rc_key->msg_cnt++;
	if(rc_key->value && !rc_key->press_flag)
	{
		rc_key->keyboard_state = PRESS;
		rc_key->press_time = rc_key->msg_cnt;
		rc_key->press_flag = 1;
	}
	else if(rc_key->value && rc_key->press_flag)
	{
		if((rc_key->msg_cnt - rc_key->press_time)<rc_key->shortdown_time)
			rc_key->keyboard_state = SHORT_DOWN;
		else if((rc_key->msg_cnt - rc_key->press_time)>=rc_key->shortdown_time)
			rc_key->keyboard_state = DOWN;
	}
	else if(!rc_key->value && rc_key->press_flag)
	{

		rc_key->keyboard_state = RELEASE;
		rc_key->press_flag = 0;
	}
	else if(!rc_key->value && !rc_key->press_flag)
		rc_key->keyboard_state = UP;	
}

