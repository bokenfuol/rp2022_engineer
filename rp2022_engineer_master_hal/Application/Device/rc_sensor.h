#ifndef __RC_SENSOR_H
#define __RC_SENSOR_H


#include "stdbool.h"
#include "drv_uart.h"
#include "config.h"


#define 	 IS_RC_ONLINE					 (rc_sensor.work_state==DEV_ONLINE)
/* 获取鼠标三轴的移动速度 */
#define    MOUSE_X_MOVE_SPEED    (rc_sensor.mouse_vx)
#define    MOUSE_Y_MOVE_SPEED    (rc_sensor.mouse_vy)
#define    MOUSE_Z_MOVE_SPEED    (rc_sensor.mouse_vz)


/* 检测鼠标按键状态 
   按下为1，没按下为0*/
#define    IF_MOUSE_PRESSED_LEFT    (rc_sensor.mouse_btn_l == 1)
#define    IF_MOUSE_PRESSED_RIGHT    (rc_sensor.mouse_btn_r == 1)



typedef struct {
	float clamp;			//夹爪参数				
	float tranlation;				//横移参数
	float lifting;					//抬升参数
	float cabin_speed;			//矿仓电机速度
	float ground_tranlation;//地矿参数
	float ground_pitch;			
}parameter_t;

typedef enum {
	HIGH,
	MID,
	LOW,
}trigger_state_t;

typedef struct rc_control_struct{
	trigger_state_t		Last_L_Trigger;
	trigger_state_t		Last_R_Trigger;	
	trigger_state_t		L_Trigger;
	trigger_state_t		R_Trigger;	
	state_t 					keyboard_mode;
	parameter_t				parameter;
}rc_control_t;


typedef struct rc_sensor_struct {
	int16_t 	ch0;
	int16_t 	ch1;
	int16_t 	ch2;
	int16_t 	ch3;
	uint8_t  	s1;
	uint8_t  	s2;
	int16_t		mouse_vx;
	int16_t 	mouse_vy;
	int16_t 	mouse_vz;
	uint8_t 	mouse_btn_l;
	uint8_t 	mouse_btn_r;
	uint16_t	key_v;
	int16_t 	thumbwheel;	
	
	int16_t		offline_cnt;
	int16_t		offline_max_cnt;
	
	dev_work_state_t	work_state;
	dev_errno_t			errno;
	
	void      (*heart_beat)(struct rc_sensor_struct *self);
	void			(*check)(struct rc_sensor_struct *self,struct rc_control_struct *con);
} rc_sensor_t;


extern rc_sensor_t rc_sensor;
extern rc_control_t	rc_control;

void rc_sensor_check(rc_sensor_t *rc,rc_control_t *control);
void RC_control(void);
void rc_sensor_info_check(rc_sensor_t *rc_sen);
void rc_sensor_heart_beat(rc_sensor_t *rc);


#endif

