#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"

#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define	abs(x) (x>0 ? x:-x)
#define max(x,y) (x>y?x:y)
#define distance(x,y)	((x>=y)?(x-y):(y-x))

//从板模式，开启后为从板模式
#define slave_mode  1

#define LEN_FILTER  150

typedef enum{
	OFF,
	ON,
}state_t;

typedef enum {
    DEV_OFFLINE,
    DEV_ONLINE,
} dev_work_state_t;

typedef enum {
	NONE_ERR,			// 正常(无错误)
	DEV_ID_ERR,		// 设备ID错误
	DEV_INIT_ERR,	// 设备初始化错误
	DEV_DATA_ERR,	// 设备数据错误
} dev_errno_t;

typedef enum {
	 RESET_NOT_OK,
   RESET_OK,
} reset_state_t;



#endif
