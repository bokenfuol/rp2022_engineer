#ifndef __CHASSIS_CONTROL_H
#define __CHASSIS_CONTROL_H

#include "config.h"
#include "chassis.h"

typedef enum {
	NO_BARKE,
	F,//����ǰ
	RF,//������ǰ
	R,//������
	RB,
	B,
	LB,
	L,
	LF,
}barke_angle_t;//ɲ���ֶ�


/*------------���̿��ƽṹ��-------------*/
typedef struct {
	float move;					//ǰ��ӳ��
	float tranlation;	//ƽ��ӳ��
	float turn; 				//��תӳ��
	
	float XY_speed;			//XY��δ����ת�����ٶ�
	float XY_angle;			//XY��δ����ת���򣩷���Ƕ�
	
	float turn_speed;		//��ת�ٶ�
	float turn_angle[4];//��ת�Ƕ�
	
	float real_speed[4];//������ʵ�ٶ�
	float real_angle[4];//������ʵ�Ƕ�

	reset_state_t reset;
	barke_angle_t barke_angle;//ɲ���Ƕ�
}chassis_control_t;

extern chassis_control_t chassis_control;

/*���̸�λ*/										reset_state_t chassis_reset(void);
/*���̳�ʼ��*/									void chassis_init(void);
/*�����ܿ�*/										void general_chassis_control(void);
/*��������*/										void slow_dwon(chassis_control_t *control);
/*��XY�ٶ�����ת�ٶ�*/					void calculate_speed(chassis_control_t *control);
/*���XYƫ���Ƕ�����ת�Ƕ�*/		void calculate_angle(chassis_control_t *control);
/*����Ʋ���*/									void calculate_real(chassis_motor_t *chas,chassis_control_t *control);
/*����ʵ�Ƕ�*/									void calculate_real_angle(chassis_control_t *control,chassis_motor_cnt_t num);
/*����ʵ�ٶ�*/									void calculate_real_speed(chassis_control_t *control,chassis_motor_cnt_t num);
/*�����ٶȲ���ֹ����*/					void speed_constrain(chassis_control_t *control);
/*�ͽ�ѡ�����ǶȲ����Ŀ��ֵ*/void angle_nearby(chassis_motor_t *chas,chassis_control_t *control);
/*ɲ��*/												void motor_barke_check(void);

#endif

