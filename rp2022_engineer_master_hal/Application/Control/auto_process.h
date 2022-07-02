#ifndef __AUTO_PROCESS_H
#define __AUTO_PROCESS_H

#include "config.h"


typedef struct {
	uint8_t				 				process;//�Զ���ģʽ
	uint8_t								step;		//����
}auto_t;

typedef struct
{
	auto_t				clamp_process;//��צ�Զ���			/*1���սӺ� 2��С��Դ��һ������ 3���ؿ�Խ� 4���һ� */
	auto_t				ground_process;//�ؿ�����Զ���	/*1���ؿ�  2���ϰ���  3����Ԯ*/
	auto_t				cabin_process;//����Զ���			/*1���¿�  2���Ͽ�  3���󷭿�  4���ҷ���*/
	
	
	uint8_t				chassis_conversion_master;//�һ�����1ǰ�����˱�־
	uint8_t				chassis_conversion_slaver;//�һ�����2ǰ�����˱�־
	
	uint8_t				save_solider;//��Ԯ����
	uint8_t 			ore_num;//��ʯ����
	
	state_t				barrier_flag;
	
	state_t				emer;//Ӧ������
}auto_process_t;


void auto_process_general(void);
void clamp_auto_process(void);
void ground_auto_process(void);
void cabin_auto_process(void);

void clamp_after(void);
void clamp_sivler_Three(void);
void clamp_after_ground(void);
void clamp_conversion(void);

void ground_ground(void);
void ground_barrier(void);
void ground_save(void);


extern uint32_t autoprocess_delay; 
extern auto_process_t auto_process;

#endif
