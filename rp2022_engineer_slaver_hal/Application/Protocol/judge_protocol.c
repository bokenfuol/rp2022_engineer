/**
 * @file        judge_protocol.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        3-November-2021
 * @brief       judge_protocol
 */

#include "judge_protocol.h"
#include "drv_uart.h"
#include "drv_can.h"
#include "string.h"
#include "crc.h"
#include "can.h"

uint8_t judge_Txbuffer[8]={0};

judge_t judge = {
	.power_heat_data_offline_max_cnt = 30,
};
uint16_t frame_length;

void judge_update(judge_t *judge_sen,uint8_t *rxBuf)
{
	if(Verify_CRC8_Check_Sum(rxBuf, LEN_FRAME_HEAD) == true)
	{
		memcpy(&judge_sen->fream_header, rxBuf, LEN_FRAME_HEAD);//5个字节
		frame_length = LEN_FRAME_HEAD + LEN_CMD_ID + judge_sen->fream_header.data_length + LEN_FRAME_TAIL;
		if(judge_sen->fream_header.sof == 0xA5)
		{
			if(Verify_CRC16_Check_Sum(rxBuf, frame_length) == true) 
			{
				uint32_t cmd_id = rxBuf[5] | (rxBuf[6]<<8);
				switch(cmd_id)
				{
					case ID_game_state:					
						memcpy(&judge_sen->ext_game_status,rxBuf+7, judge_sen->fream_header.data_length);
						judge_sen->power_heat_data_offline_cnt = 0;				
					break;
				}
			}
			judge_send();
		}
		if(rxBuf[frame_length] == 0xA5)
			judge_update(judge_sen,&rxBuf[frame_length]);
	}
}


void judge_send()
{
		int * p;
		p = (int *)&judge.shoot_data.bullet_speed;
		HAL_CAN_TxHeadeInit(0x001);
		if(judge.power_heat_data_state == DEV_ONLINE)
		{
			judge_Txbuffer[0] = (judge.power_heat_data.chassis_power_buffer>>8)&0xff;
			judge_Txbuffer[1] = judge.power_heat_data.chassis_power_buffer&0xff;
		}
		judge_Txbuffer[2] = (judge.game_robot_status.chassis_power_limit>>8)&0xff;
		judge_Txbuffer[3] = judge.game_robot_status.chassis_power_limit&0xff;	
	
		judge_Txbuffer[4] = (judge.power_heat_data.shooter_id1_42mm_cooling_heat>>8)&0xff;
		judge_Txbuffer[5] = judge.power_heat_data.shooter_id1_42mm_cooling_heat&0xff;
	
		judge_Txbuffer[6] = (judge.game_robot_status.shooter_id1_42mm_cooling_limit>>8)&0xff;
		judge_Txbuffer[7] = judge.game_robot_status.shooter_id1_42mm_cooling_limit&0xff;
		
		while(HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,judge_Txbuffer,(uint32_t *)CAN_TX_MAILBOX0)!=HAL_OK);
		memset(judge_Txbuffer,0,8);
		HAL_CAN_TxHeadeInit(0x002);
		judge_Txbuffer[0] = (*p>>24)&0xff;
		judge_Txbuffer[1] = (*p>>16)&0xff;
		judge_Txbuffer[2] = (*p>>8)&0xff;
		judge_Txbuffer[4] = *p&0xff;
		
		judge_Txbuffer[5] = (judge.game_robot_status.shooter_id1_42mm_speed_limit>>8)&0xff;
		judge_Txbuffer[6] = judge.game_robot_status.shooter_id1_42mm_speed_limit&0xff;
		while(HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,judge_Txbuffer,(uint32_t *)CAN_TX_MAILBOX0)!=HAL_OK);
		memset(judge_Txbuffer,0,8);
}


void USART4_rxDataHandler(uint8_t *rxBuf)
{
	judge_update(&judge,rxBuf);
}

//检查裁判系统失联
void check_judge_offline(judge_t *judge_sen)
{
	judge_sen->power_heat_data_offline_cnt++;
	if(judge_sen->power_heat_data_offline_cnt > judge_sen->power_heat_data_offline_max_cnt)
	{
		judge_sen->power_heat_data_offline_cnt = judge_sen->power_heat_data_offline_max_cnt;
		judge_sen->power_heat_data_state = DEV_OFFLINE;
	}
	else if(judge_sen->power_heat_data_state == DEV_OFFLINE)
		judge_sen->power_heat_data_state = DEV_ONLINE;
	
}