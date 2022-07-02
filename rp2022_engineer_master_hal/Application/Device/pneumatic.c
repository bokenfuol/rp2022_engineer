/**
 * @file        pneumatic.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        14-January-2022
 * @brief       气动
 */
#include "pneumatic.h"


/*夹爪开合*/
pneu_info_t pneu_clamp = {
	.GPIOx = PNEU_CLAMP_GPIO_Port,
	.GPIO_Pin = PNEU_CLAMP_Pin,
	.state = OFF,
	.output = Pneu_Ctrl,
	.init = Relay_Init,
};

/*前伸*/
pneu_info_t pneu_protract = {
	.GPIOx = PNEU_PROTRACT_GPIO_Port,
	.GPIO_Pin = PNEU_PROTRACT_Pin,
	.state = OFF,
	.output = Pneu_Ctrl,
	.init = Relay_Init,
};

/*救援连杆*/
pneu_info_t pneu_linkage = {
	.GPIOx = PNEU_CABIN_GPIO_Port,
	.GPIO_Pin = PNEU_CABIN_Pin,
	.state = OFF,
	.output = Pneu_Ctrl,
	.init = Relay_Init,
};

/*救援*/
pneu_info_t pneu_save = {
	.GPIOx = PNEU_SAVE_GPIO_Port,
	.GPIO_Pin = PNEU_SAVE_Pin,
	.state = OFF,
	.output = Pneu_Ctrl,
	.init = Relay_Init,
};

/**
 * @date        14-January-2022
 * @brief       Pneu_Check
 */
void Pneu_Check(state_t state,pneu_info_t *self)
{
	if(state)
		self->output(self,GPIO_PIN_SET);
	else
		self->output(self,GPIO_PIN_RESET);
}


/**
 * @date        14-January-2022
 * @brief       Pneu_Ctrl
 */
void Pneu_Ctrl(pneu_info_t *self, GPIO_PinState	state)
{
	HAL_GPIO_WritePin(self->GPIOx, self->GPIO_Pin, state);
}


/**
 * @date        14-January-2022
 * @brief       Relay_Init
 */
void Relay_Init(pneu_info_t *self)
{
	HAL_GPIO_WritePin(self->GPIOx, self->GPIO_Pin, GPIO_PIN_RESET);
}






