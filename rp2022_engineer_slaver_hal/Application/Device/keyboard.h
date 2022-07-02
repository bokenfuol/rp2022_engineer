#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "config.h"
#include "rc_sensor.h"




typedef enum {
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_Shift,
	KEY_Ctrl,
	KEY_Q,
	KEY_E,
	KEY_R,
	KEY_F,
	KEY_G,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEYBOARD_CNT,
}keyboard_cnt_t;

typedef enum {
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_CNT,
} mouse_cnt_t;

typedef enum {
	UP,
	PRESS,
	DOWN,
	SHORT_DOWN,
	RELEASE,
} keyboard_state_t;


typedef struct{
	uint8_t value;
	keyboard_state_t keyboard_state;
	uint32_t  press_time;
	uint32_t  shortdown_time;
	uint8_t 	press_flag;
	uint32_t  msg_cnt;
}rc_keybroad_t;

//键盘结构体
typedef struct keyboard_sensor_struct{
	/*结构体*/
	rc_keybroad_t keyboard[KEYBOARD_CNT];
	rc_keybroad_t mouse[MOUSE_CNT];
	
	float mouse_vx;
	float mouse_vy;
	float mouse_vz;
	
	/*指针函数*/
	void	(*update)(struct keyboard_sensor_struct *self,struct rc_sensor_struct *rc);
	void	(*check)(struct keyboard_sensor_struct *self,struct rc_sensor_struct *rc);
}keyboard_sensor_t;

typedef struct keboard_control_struct{
	float W_move;
	float S_move;
	float A_move;
	float D_move;
	state_t car_lock;				//车辆锁定状态
	state_t	special_shift;	//特殊键Shift
	state_t	special_ctrl;		//特殊键Ctrl
	state_t	special_reset;	//复位特殊标志（Shift+鼠标右键）
	parameter_t	parameter;
}keyboard_control_t;


void Keyboard_control(void);
void keyboard_update(keyboard_sensor_t *key,rc_sensor_t *rc);
void keyboard_check(keyboard_sensor_t *key,rc_sensor_t *rc);
void keyboard_judge(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_W_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_S_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_A_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_D_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_Q_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_E_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_Shift_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_Ctrl_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_R_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_F_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_G_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_Z_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_X_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_C_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_V_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void keyboard_B_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_key);
void mouse_left_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_mouse);
void mouse_right_check(rc_sensor_t *rc_sen,rc_keybroad_t *rc_mouse);


extern uint32_t keyboard_time;
extern keyboard_sensor_t keyboard_sensor;
extern keyboard_control_t keyboard_control;

#endif

