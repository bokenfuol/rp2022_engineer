#ifndef __AUTO_PROCESS_H
#define __AUTO_PROCESS_H

#include "config.h"


typedef struct {
	uint8_t				 				process;//自动化模式
	uint8_t								step;		//步骤
}auto_t;

typedef struct
{
	auto_t				clamp_process;//夹爪自动化			/*1：空接后 2：小资源岛一键三矿 3：地矿对接 4：兑换 */
	auto_t				ground_process;//地矿机构自动化	/*1：地矿  2：障碍块  3：救援*/
	auto_t				cabin_process;//矿仓自动化			/*1：下矿  2：上矿  3：左翻矿  4：右翻矿*/
	
	
	uint8_t				chassis_conversion_master;//兑换主控1前进后退标志
	uint8_t				chassis_conversion_slaver;//兑换主控2前进后退标志
	
	uint8_t				save_solider;//救援兵种
	uint8_t 			ore_num;//矿石数量
	
	state_t				barrier_flag;
	
	state_t				emer;//应急处理
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
