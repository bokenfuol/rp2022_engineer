#ifndef __DRV_CAN_H
#define __DRV_CAN_H

#include "config.h"

extern CAN_FilterTypeDef CAN_FillterType;
extern CAN_TxHeaderTypeDef CAN_TxHeadeType;
extern CAN_RxHeaderTypeDef CAN_RxHeadeType;

void CAN_Init(void);
void HAL_CAN_FilterInit(void);
void HAL_CAN_RxHeadeInit(void);
void HAL_CAN_TxHeadeInit(uint16_t ID);

#endif
