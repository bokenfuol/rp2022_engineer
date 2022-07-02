#ifndef __CONTROL_H
#define __CONTROL_H

#include "config.h"

#define IS_SYSTEM_RESET	(control.master_reset == RESET_OK)

typedef enum
{
	NORMAL_MODE,//����ģʽ
	AIR_MODE,//�ս�ģʽ
	SILVERORE_MODE,//����ģʽ
	GOLDENORE_MODE,//���ģʽ
	GROUND_MODE,//�ؿ�ģʽ
	CONVERSION_MODE,//�һ�ģʽ
	SAVE_MODE,//��Ԯģʽ
	BARRIER_MODE,//�ϰ���ģʽ
	BLOCK_MODE,//��ģʽ
	SPINNING_MODE,//����ģʽ
}system_mode_t;



typedef struct {
	reset_state_t				 	master_reset;	//���帴λ��־
	reset_state_t					slaver_reset;	//�Ӱ帴λ��־
	system_mode_t 				system_mode;	//ϵͳ����
	
	state_t				 				vision_state;	//�Ӿ���������ָ��
		
	int32_t								master_motor_online;
	int32_t								motor_online;
}control_struct;


extern control_struct control;

void general_control(void);
void general_reset(void);


#endif

