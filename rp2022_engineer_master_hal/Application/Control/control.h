#ifndef __CONTROL_H
#define __CONTROL_H

#include "config.h"

#define IS_SYSTEM_RESET	(control.master_reset == RESET_OK)

typedef enum
{
	NORMAL_MODE=0,//����ģʽ
	AIR_MODE=1,//�ս�ģʽ
	SILVERORE_MODE=2,//����ģʽ
	GOLDENORE_MODE=3,//���ģʽ
	GROUND_MODE=4,//�ؿ�ģʽ
	CONVERSION_MODE=5,//�һ�ģʽ
	SAVE_MODE=6,//��Ԯģʽ
	BARRIER_MODE=7,//�ϰ���ģʽ
	BLOCK_MODE=8,//��ģʽ
	SPINNING_MODE=9,//����ģʽ
}system_mode_t;








typedef struct {
	reset_state_t				 	master_reset;	//���帴λ��־
	reset_state_t					slaver_reset;	//�Ӱ帴λ��־
	system_mode_t 				system_mode;	//ϵͳ����
}control_struct;


extern control_struct control;

void general_control(void);
void general_reset(void);


#endif

