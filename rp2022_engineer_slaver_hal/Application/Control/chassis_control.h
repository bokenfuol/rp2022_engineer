#ifndef __CHASSIS_CONTROL_H
#define __CHASSIS_CONTROL_H

#include "config.h"
#include "chassis.h"

typedef enum {
	NO_BARKE,
	F,//方向朝前
	RF,//方向朝右前
	R,//方向朝右
	RB,
	B,
	LB,
	L,
	LF,
}barke_angle_t;//刹车分段


/*------------底盘控制结构体-------------*/
typedef struct {
	float move;					//前进映射
	float tranlation;	//平移映射
	float turn; 				//旋转映射
	
	float XY_speed;			//XY（未加旋转方向）速度
	float XY_angle;			//XY（未加旋转方向）方向角度
	
	float turn_speed;		//旋转速度
	float turn_angle[4];//旋转角度
	
	float real_speed[4];//轮组真实速度
	float real_angle[4];//轮组真实角度

	reset_state_t reset;
	barke_angle_t barke_angle;//刹车角度
}chassis_control_t;

extern chassis_control_t chassis_control;

/*底盘复位*/										reset_state_t chassis_reset(void);
/*底盘初始化*/									void chassis_init(void);
/*底盘总控*/										void general_chassis_control(void);
/*慢速增减*/										void slow_dwon(chassis_control_t *control);
/*求XY速度与旋转速度*/					void calculate_speed(chassis_control_t *control);
/*获得XY偏航角度与旋转角度*/		void calculate_angle(chassis_control_t *control);
/*求控制参数*/									void calculate_real(chassis_motor_t *chas,chassis_control_t *control);
/*求真实角度*/									void calculate_real_angle(chassis_control_t *control,chassis_motor_cnt_t num);
/*求真实速度*/									void calculate_real_speed(chassis_control_t *control,chassis_motor_cnt_t num);
/*限制速度并防止差速*/					void speed_constrain(chassis_control_t *control);
/*就近选择舵向角度并输出目标值*/void angle_nearby(chassis_motor_t *chas,chassis_control_t *control);
/*刹车*/												void motor_barke_check(void);

#endif

