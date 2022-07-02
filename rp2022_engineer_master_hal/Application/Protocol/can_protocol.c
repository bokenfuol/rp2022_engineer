/**
 * @file        can_protocol.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        3-November-2021
 * @brief       can_protocol
 */

#include "control.h"
#include "mode_switch.h"
#include "auto_process.h"
#include "keyboard.h"
#include "device.h"
#include "device.h"
#include "rc_sensor.h"
#include "can_protocol.h"
#include "drv_can.h"
#include "can.h"
#include "string.h"


/**
 * @date        3-November-2021
 * @brief       CAN接收
 */
uint8_t CAN_Rxbuffer[8];
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1)
	{
		if(HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0,&CAN_RxHeadeType, CAN_Rxbuffer)==HAL_OK)
		{
			if(CAN_RxHeadeType.StdId == CAN_Slaver_ID)/*bit1:reset 2-3bit auto_conversion*/
			{
				control.slaver_reset = (reset_state_t)(CAN_Rxbuffer[0]&0x01);
				auto_process.chassis_conversion_slaver = (CAN_Rxbuffer[0]&0x06)>>1;
				keyboard_control.car_lock = CAN_Rxbuffer[1];
			}
			
			/*接收电机遍历*/
			for(uint16_t i=0;i<DEVICE_CNT;i++)
				if(device[i].motor->CAN_select == HCAN1)
					if(device[i].motor->motor_id == CAN_RxHeadeType.StdId)
					{
						motor.update(device[i].motor,CAN_Rxbuffer);
						break;
					}			
		}			
	}
	else if(hcan->Instance == CAN2)
	{
		if(HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO0,&CAN_RxHeadeType, CAN_Rxbuffer)==HAL_OK)
		{
			/*接收电机遍历*/
			for(uint16_t i=0;i<DEVICE_CNT;i++)
				if(device[i].motor->CAN_select == HCAN2)
					if(device[i].motor->motor_id == CAN_RxHeadeType.StdId)
					{
						motor.update(device[i].motor,CAN_Rxbuffer);
						break;
					}
		}
	}
}



/**
 * @date        3-November-2021
 * @brief       CAN发送
 */
uint8_t CAN_Txbuffer[8];
void can_send()
{
	/*板间通信包*/
	HAL_CAN_TxHeadeInit(CAN_Master_ID);
	
	/*bit1:reset bit2-3:auto_conversion bit4:save_over bit5:silver_over*/		
	CAN_Txbuffer[0] = control.master_reset|auto_process.chassis_conversion_master<<1|((auto_process.ground_process.process == 3&&auto_process.ground_process.step == 3)<<3)\
	|((mode_switch.mode == SILVERORE_MODE&&auto_process.clamp_process.step == 0)<<4);
	/*bit1-4:mode bit5-8:modeswitch_mode*/	
	CAN_Txbuffer[1] = (control.system_mode)|(mode_switch.mode<<4);
	/*vision_data*/
	CAN_Txbuffer[2] = auto_process.save_solider;
	for(uint8_t i = 0 ; i < 8 ; i ++)
		CAN_Txbuffer[3] |= device[i].motor->work_state<<i;
	CAN_Txbuffer[4] |= device[DEVICE_09].motor->work_state;
	CAN_Txbuffer[4] |= device[DEVICE_10].motor->work_state<<1;
	CAN_Txbuffer[4] |= device[DEVICE_11].motor->work_state<<2;
	

	while(HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,CAN_Txbuffer,(uint32_t *)CAN_TX_MAILBOX0)!=HAL_OK);
	memset(CAN_Txbuffer,0,8);
	
	/*0x200包*/
	HAL_CAN_TxHeadeInit(CAN_0x200_four_ID);
	if(IS_RC_ONLINE)
	{
		/*发送电机遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->CAN_select == HCAN1)
				if(device[i].motor->motor_id>=0x201&&device[i].motor->motor_id<=0x204)
					motor.output(device[i].motor,CAN_Txbuffer);
	}
	while(HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,CAN_Txbuffer,(uint32_t *)CAN_TX_MAILBOX0)!=HAL_OK);
	memset(CAN_Txbuffer,0,8);
	
	if(IS_RC_ONLINE)
	{
		/*发送电机遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->CAN_select == HCAN2)
				if(device[i].motor->motor_id>=0x201&&device[i].motor->motor_id<=0x204)
					motor.output(device[i].motor,CAN_Txbuffer);
	}
	while(HAL_CAN_AddTxMessage(&hcan2,&CAN_TxHeadeType,CAN_Txbuffer,(uint32_t *)CAN_TX_MAILBOX1)!=HAL_OK);
	memset(CAN_Txbuffer,0,8);

	/*0x1ff包*/
	HAL_CAN_TxHeadeInit(CAN_0x1ff_four_ID);
	if(IS_RC_ONLINE)
	{
		/*发送电机遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->CAN_select == HCAN1)
				if(device[i].motor->motor_id>=0x205&&device[i].motor->motor_id<=0x208)
					motor.output(device[i].motor,CAN_Txbuffer);		
	}
	while(HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,CAN_Txbuffer,(uint32_t *)CAN_TX_MAILBOX0)!=HAL_OK);
	memset(CAN_Txbuffer,0,8);
	if(IS_RC_ONLINE)
	{
		/*发送电机遍历*/
		for(uint16_t i=0;i<DEVICE_CNT;i++)
			if(device[i].motor->CAN_select == HCAN2)
				if(device[i].motor->motor_id>=0x205&&device[i].motor->motor_id<=0x208)
					motor.output(device[i].motor,CAN_Txbuffer);
	}
	while(HAL_CAN_AddTxMessage(&hcan2,&CAN_TxHeadeType,CAN_Txbuffer,(uint32_t *)CAN_TX_MAILBOX1)!=HAL_OK);
	memset(CAN_Txbuffer,0,8);		

}

