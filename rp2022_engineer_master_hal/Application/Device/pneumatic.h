#ifndef __PNEUMATIC_H
#define __PNEUMATIC_H

#include "config.h"

typedef struct pneu_info_struct{
	GPIO_TypeDef* GPIOx;
	uint16_t 			GPIO_Pin;
	state_t				state;
	void 					(*output)(struct pneu_info_struct *self, GPIO_PinState);
	void 					(*init)(struct pneu_info_struct *self);
}pneu_info_t;

extern pneu_info_t pneu_clamp;
extern pneu_info_t pneu_protract;
extern pneu_info_t pneu_linkage;
extern pneu_info_t pneu_save;

void Pneu_Check(state_t state,pneu_info_t *self);
void Pneu_Ctrl(pneu_info_t *self, GPIO_PinState	state);
void Relay_Init(pneu_info_t *self);


#endif
