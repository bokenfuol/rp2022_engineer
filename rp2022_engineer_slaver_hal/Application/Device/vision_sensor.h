#ifndef ___VISION_SENSOR_H
#define ___VISION_SENSOR_H

#include "drv_uart.h"
#include "config.h"

//起始字节，协议固定尾0xA5
#define 	VISION_SOF	0XA5
//长度根据协议定义
#define		VISION_LEN_HEADER		3			//帧头长
#define		VISION_LEN_PACKED		29		//数据包长度，可自定义

/* 帧头字节偏移 */
typedef enum {
	sof			= 0,
	Cmd_ID		= 1,
	CRC8		= 2,
	Data		= 3,
	CRC16	= 27,
} Vision_Frame_Header_Offset_t;



//视觉接收
typedef __packed struct vison_sensor_info_struct{
	uint8_t  sof;
	uint8_t  cmd_id;
	uint8_t  crc8;	
	
	float  	float_1;
	float  	float_2;
	
	int 		int_1;
	int			int_2;
	int 		int_3;
	int			int_4;

	uint16_t crc16;	

	int16_t		offline_cnt;
	int16_t		offline_max_cnt;

	dev_work_state_t	vision_sensor_state;	
	
}vison_sensor_info_t;

typedef __packed struct {
	vison_sensor_info_t	  tx;
	vison_sensor_info_t 	rx;
	void									(*ckeck)(struct vison_sensor_info_struct *self);
	void									(*heart_beat)(struct vison_sensor_info_struct *self);	
	void									(*output)(struct vison_sensor_info_struct *self);
}vision_sensor_t;

extern vision_sensor_t vision_sensor;

void vision_check(vison_sensor_info_t *visi);
void vision_heart_beat(vison_sensor_info_t *visi);
void vision_output(vison_sensor_info_t *vis);


#endif

