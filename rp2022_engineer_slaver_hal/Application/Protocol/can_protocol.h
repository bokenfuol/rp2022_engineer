#ifndef __CAN_PROTOCOL_H
#define __CAN_PROTOCOL_H

#include "config.h"

/*CAN���ͻ��ǽ��յ�ID*/
typedef enum
{
	CAN_Master_ID	= 0x001,	
	CAN_Slaver_ID = 0x010,
	
	CAN_0x200_four_ID	= 0x200,	
	//����ת�����	
	
	CAN_0x1ff_four_ID = 0x1FF,
	
	
	//���̶�����
	CAN_Rudder_MotorLF_ID 	= 0x205,    //��ǰ
	CAN_Rudder_MotorRF_ID 	= 0x206,		//��ǰ
	CAN_Rudder_MotorLB_ID 	= 0x207,    //���
	CAN_Rudder_MotorRB_ID 	= 0x208,		//�Һ�		
	
}CAN_Message_ID;

void can_send(void);


#endif

