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
		
	
}CAN_Message_ID;

void can_send(void);


#endif

