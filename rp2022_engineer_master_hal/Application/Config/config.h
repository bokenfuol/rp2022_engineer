#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"

#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define	abs(x) (x>0 ? x:-x)
#define max(x,y) (x>y?x:y)
#define distance(x,y)	((x>=y)?(x-y):(y-x))

typedef enum{
	OFF,
	ON,
}state_t;

typedef enum {
    DEV_OFFLINE,
    DEV_ONLINE,
} dev_work_state_t;

typedef enum {
	NONE_ERR,			// ����(�޴���)
	DEV_ID_ERR,		// �豸ID����
	DEV_INIT_ERR,	// �豸��ʼ������
	DEV_DATA_ERR,	// �豸���ݴ���
} dev_errno_t;

typedef enum {
	 RESET_NOT_OK,
   RESET_OK,
} reset_state_t;





#endif
