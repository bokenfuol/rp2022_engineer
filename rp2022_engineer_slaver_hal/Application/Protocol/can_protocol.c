/**
 * @file        can_protocol.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        3-November-2021
 * @brief       can_protocol
 */

#include "control.h"
#include "mode_switch.h"
#include "keyboard.h"
#include "auto_process.h"
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
			if(CAN_RxHeadeType.StdId == CAN_Master_ID)
			{
				/*bit1:reset bit2-3:auto_conversion bit4:save_over bit5:silver_over*/	
				control.master_reset = (reset_state_t)(CAN_Rxbuffer[0]&0x01);
				auto_process.chassis_conversion_master = (CAN_Rxbuffer[0]&0x06)>>1;
				auto_process.save_flag = (CAN_Rxbuffer[0]&0x08)>>3;
				auto_process.silver_flag = (CAN_Rxbuffer[0]&0x10)>>4;
				/*bit1-4:mode bit5-8:modeswitch_mode*/	
				control.system_mode = (CAN_Rxbuffer[1]&0x0F);
				mode_switch.mode = (CAN_Rxbuffer[1]&0xF0)>>4;
				
				auto_process.save_solider = CAN_Rxbuffer[2];
				
				control.master_motor_online = 0;
				control.master_motor_online |= CAN_Rxbuffer[3];
				control.master_motor_online |= CAN_Rxbuffer[4]<<8;
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
uint8_t CAN_Txbuffer[8],hcan1_send=0;
void can_send()
{
	HAL_CAN_TxHeadeInit(CAN_0x200_four_ID );
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
	
	
	
	/*板间通信包*/	
	HAL_CAN_TxHeadeInit(CAN_Slaver_ID);/*bit1:reset 2-3bit auto_conversion*/
	CAN_Txbuffer[0] = (control.slaver_reset)|auto_process.chassis_conversion_slaver<<1;
	CAN_Txbuffer[1] = keyboard_control.car_lock;
	
	if(hcan1_send)
	{
		hcan1_send = 0;
		while(HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,CAN_Txbuffer,(uint32_t *)CAN_TX_MAILBOX1)!=HAL_OK);
	}
	else
		hcan1_send = 1;
	
	memset(CAN_Txbuffer,0,8);	
	
	

	/*0x1ff包*/
	HAL_CAN_TxHeadeInit(CAN_0x1ff_four_ID);
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

