#ifndef __IMU_SENSOR_H
#define __IMU_SENSOR_H

#include "main.h"
#include "config.h"

typedef struct{
	float imu_center;
	float imu_angle_sum;
	float imu_angle_pre;
}flag_longtime_t;

typedef struct imu_sensor_info_struct {
	float yaw;
	float pitch;
	float roll;
	short rate_yaw;
	short rate_pitch;
	short rate_roll;
	
	float last_yaw;
	float yaw_offset;
	float yaw_total;
		
	
	dev_work_state_t imu_sen_state;
	
	uint32_t  msg_cnt;
	uint8_t offline_cnt;
	uint8_t offline_max_cnt;
	
	void		(*update)(struct imu_sensor_info_struct *self);
	void		(*heart_beat)(struct imu_sensor_info_struct *self);
} imu_sensor_t;

extern imu_sensor_t imu_sensor;

void imu_sensor_update(imu_sensor_t *imu_sen);
void imu_heart_beat(imu_sensor_t *motor);


#endif

