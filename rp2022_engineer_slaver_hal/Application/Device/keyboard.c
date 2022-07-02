/**
 * @file        keyboard.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        13-November-2021
 * @brief       键盘
 */

#include "control.h"
#include "chassis_control.h"
#include "mode_switch.h"
#include "keyboard.h"
#include "filter.h"



keyboard_control_t keyboard_control = {
	/*键盘参数列表区*/
	.car_lock = OFF,
	.parameter = {
		.move = 0.3,
		.tranlation = 0.3,
		.turn = 100,
	},
};



/*按键反应时间*/
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
		[MOUSE_LEFT] = {.shortdown_time = 300,},
		[MOUSE_RIGHT] = {.shortdown_time = 50,},
	},
	.update = keyboard_update,
	.check = keyboard_check,

};


/**
 * @date        3-November-2021
 * @brief       键盘控制
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
int16_t filter_vx[LEN_FILTER],filter_vy[LEN_FILTER],filter_vz[LEN_FILTER];/*滑动滤波数组*/
uint32_t key_time = 0;/*键盘时间线*/
uint32_t shake_time[KEYBOARD_CNT+MOUSE_CNT];/*防抖动时间*/
uint16_t max_shake_time=30;/*抖动判断最大值*/
void keyboard_update(keyboard_sensor_t *key,rc_sensor_t *rc)
{
	key_time ++;
	/*键盘防抖动*/
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
	/*鼠标防抖动*/
	if(IF_MOUSE_PRESSED_LEFT)
		shake_time[KEYBOARD_CNT] ++;
	else
		shake_time[KEYBOARD_CNT] = 0;
	if(shake_time[KEYBOARD_CNT]>max_shake_time)
		key->mouse[MOUSE_LEFT].value = 1;
	else 
		key->mouse[MOUSE_LEFT].value = 0;
	/*鼠标防抖动*/
	if(IF_MOUSE_PRESSED_RIGHT)
		shake_time[KEYBOARD_CNT+1] ++;
	else
		shake_time[KEYBOARD_CNT+1] = 0;
	if(shake_time[KEYBOARD_CNT+1]>max_shake_time)
		key->mouse[MOUSE_RIGHT].value = 1;
	else 
		key->mouse[MOUSE_RIGHT].value = 0;
	
	keyboard_sensor.mouse_vx = rc_sensor.mouse_vx;
	
//	filter_vx[key_time%LEN_FILTER] = rc_sensor.mouse_vx;
//	filter_vy[key_time%LEN_FILTER] = rc_sensor.mouse_vy;
//	filter_vz[key_time%LEN_FILTER] = rc_sensor.mouse_vz;
	
//	moving_filter(filter_vx,&keyboard_sensor.mouse_vx);
//	moving_filter(filter_vy,&keyboard_sensor.mouse_vy);
//	moving_filter(filter_vz,&keyboard_sensor.mouse_vz);

}


/**
 * @date        13-November-2021
 * @brief       keyboard_check
 */
void keyboard_check(keyboard_sensor_t *key,rc_sensor_t *rc)
{
	/*检查键盘结构体*/
	keyboard_W_check(rc,&key->keyboard[KEY_W]);
	keyboard_S_check(rc,&key->keyboard[KEY_S]);
	keyboard_A_check(rc,&key->keyboard[KEY_A]);
	keyboard_D_check(rc,&key->keyboard[KEY_D]);
//	keyboard_Q_check(rc,&key->keyboard[KEY_Q]);
//	keyboard_E_check(rc,&key->keyboard[KEY_E]);
	keyboard_Shift_check(rc,&key->keyboard[KEY_Shift]);
	keyboard_Ctrl_check(rc,&key->keyboard[KEY_Ctrl]);
	keyboard_R_check(rc,&key->keyboard[KEY_R]);
//	keyboard_F_check(rc,&key->keyboard[KEY_F]);
	keyboard_G_check(rc,&key->keyboard[KEY_G]);
//	keyboard_Z_check(rc,&key->keyboard[KEY_Z]);
//	keyboard_X_check(rc,&key->keyboard[KEY_X]);
//	keyboard_C_check(rc,&key->keyboard[KEY_C]);
//	keyboard_V_check(rc,&key->keyboard[KEY_V]);
	keyboard_B_check(rc,&key->keyboard[KEY_B]);
//	mouse_left_check(rc,&key->mouse[MOUSE_LEFT]);
	mouse_right_check(rc,&key->mouse[MOUSE_RIGHT]);		
}


/*----------------检查各按键结构体-------------------*/
void keyboard_W_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
			keyboard_control.W_move = 0;
		break;
		case PRESS:
		break;
		case SHORT_DOWN:
		case DOWN:
			if(keyboard_control.special_shift)
				keyboard_control.W_move = 7000;
			else if(keyboard_control.special_ctrl)
				keyboard_control.W_move = 2000;
			else
				keyboard_control.W_move = 5000;
		break;
		case RELEASE:
		break;
	}
}

void keyboard_S_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
			keyboard_control.S_move= 0;
		break;
		case PRESS:
		break;
		case SHORT_DOWN:
		case DOWN:
			if(keyboard_control.special_shift)
				keyboard_control.S_move = -7000;
			else if(keyboard_control.special_ctrl)
				keyboard_control.S_move = -2000;
			else
				keyboard_control.S_move = -5000;
		break;
		case RELEASE:
		break;
	}
}

void keyboard_A_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
			keyboard_control.A_move = 0;
		break;
		case PRESS:
		break;
		case SHORT_DOWN:
		case DOWN:
			if(keyboard_control.special_shift)
				keyboard_control.A_move = -7000;
			else if(keyboard_control.special_ctrl)
				keyboard_control.A_move = -2000;
			else
				keyboard_control.A_move = -5000;
		break;
		case RELEASE:
		break;
	}
}

void keyboard_D_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
			keyboard_control.D_move = 0;
		break;
		case PRESS:
		break;
		case SHORT_DOWN:
		case DOWN:
			if(keyboard_control.special_shift)
				keyboard_control.D_move = 7000;
			else if(keyboard_control.special_ctrl)
				keyboard_control.D_move = 2000;
			else
				keyboard_control.D_move = 5000;			
		break;
		case RELEASE:
		break;
	}
}

//	/**
// * @date        7-April-2022
// * @brief       Q键
// */
//void keyboard_Q_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
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
// * @brief       E键
// */
//void keyboard_E_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
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
 * @brief       Shift键
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
 * @brief       Ctrl键
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
 * @brief       R键
 */
void keyboard_R_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:
		break;
		case PRESS:			
			if(!keyboard_control.special_shift&&!keyboard_control.special_ctrl)
				/*对位*/
				switch(control.system_mode)
				{
					case GROUND_MODE:
					case AIR_MODE:
//					case SILVERORE_MODE:
					case GOLDENORE_MODE:
					case BARRIER_MODE:
						control.vision_state = !control.vision_state;
						keyboard_control.car_lock = !control.vision_state;
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


///**
// * @date        7-April-2022
// * @brief       F键
// */
//void keyboard_F_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
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
 * @brief       G键
 */
void keyboard_G_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
{
	keyboard_judge(rc_sen,rc_key);
	switch (rc_key->keyboard_state)
	{
		case UP:		
		break;
		case PRESS:
			keyboard_control.car_lock = !keyboard_control.car_lock;
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
		break;
	}
}


///**
// * @date        7-April-2022
// * @brief       Z键
// */
//void keyboard_Z_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
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
// * @brief       X键
// */
//void keyboard_X_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
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
// * @brief       C键
// */
//void keyboard_C_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
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
//	break;
//	}
//}


///**
// * @date        7-April-2022
// * @brief       V键
// */
//void keyboard_V_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key)
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
 * @brief       B键
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
		break;
		case SHORT_DOWN:
		break;
		case DOWN:
		break;
		case RELEASE:
		break;
	}
}


///**
// * @date        7-April-2022
// * @brief      鼠标左键
// */
//void mouse_left_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_mouse)
//{
//	keyboard_judge(rc_sen,rc_mouse);
//	switch (rc_mouse->keyboard_state)
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
 * @brief      鼠标右键
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
 * @brief      按键状态判断
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
