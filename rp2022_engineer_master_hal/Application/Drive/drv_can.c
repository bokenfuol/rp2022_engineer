/**
 * @file        drv_can.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        13-November-2021
 * @brief       drv_can
 */

#include "drv_can.h"
#include "can.h"
#include "string.h"

CAN_FilterTypeDef CAN_FillterType;
CAN_TxHeaderTypeDef CAN_TxHeadeType;
CAN_RxHeaderTypeDef CAN_RxHeadeType;


void CAN_Init()
{
	HAL_CAN_FilterInit();
	HAL_CAN_RxHeadeInit();

	
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);//中断使能
	HAL_CAN_Start(&hcan1);//外设使能
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);//中断使能
	HAL_CAN_Start(&hcan2);//外设使能	
}


void HAL_CAN_FilterInit()
{
	CAN_FillterType.FilterActivation = ENABLE;
	CAN_FillterType.FilterIdHigh = 0;
	CAN_FillterType.FilterIdLow = 0;
	CAN_FillterType.FilterMaskIdHigh = 0x0000;
	CAN_FillterType.FilterMaskIdLow = 0x0000;
	CAN_FillterType.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_FillterType.FilterBank = 0;
	CAN_FillterType.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FillterType.FilterScale = CAN_FILTERSCALE_32BIT;
	HAL_CAN_ConfigFilter(&hcan1,&CAN_FillterType);
	

	CAN_FillterType.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_FillterType.FilterBank = 1;
	HAL_CAN_ConfigFilter(&hcan2,&CAN_FillterType);
}
void HAL_CAN_RxHeadeInit()
{
	CAN_RxHeadeType.StdId = 0x0000;
	CAN_RxHeadeType.ExtId = 0x0000;
	CAN_RxHeadeType.DLC = 0;
	CAN_RxHeadeType.IDE = CAN_ID_STD;
	CAN_RxHeadeType.RTR = 0;
}
void HAL_CAN_TxHeadeInit(uint16_t ID)
{
	CAN_TxHeadeType.StdId = ID;
	CAN_TxHeadeType.ExtId = 0x0000;
	CAN_TxHeadeType.DLC = 8;
	CAN_TxHeadeType.IDE = CAN_ID_STD;
	CAN_TxHeadeType.RTR = CAN_RTR_DATA;
}
