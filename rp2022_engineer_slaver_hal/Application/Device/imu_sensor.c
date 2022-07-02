/**
 * @file        imu_sensor.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        2-November-2021
 * @brief       陀螺仪
 */


#include "imu_sensor.h"
#include "bmi.h"


imu_sensor_t imu_sensor = {
	.offline_max_cnt = 50,
	.update = imu_sensor_update,
	.heart_beat = imu_heart_beat,
};



/**
 * @date        2-November-2021
 * @brief       陀螺仪数据更新
 */
short gyrox, gyroy, gyroz;
short accx, accy, accz;
void imu_sensor_update(imu_sensor_t *imu_sen)
{
	//读陀螺仪数据
	BMI_Get_RawData(&gyrox, &gyroy, &gyroz, &accx, &accy, &accz);
	BMI_Get_EulerAngle(&imu_sen->pitch, &imu_sen->roll, &imu_sen->yaw, &gyrox, &gyroy, &gyroz, &accx, &accy, &accz);
    
	imu_sen->rate_roll = gyrox;
	imu_sen->rate_pitch = gyroy;
	imu_sen->rate_yaw = gyroz;
	
	//初始化云台结构体
	imu_sen->yaw_offset = imu_sen->yaw - imu_sen->last_yaw;
	//过零点
	if(abs(imu_sen->yaw_offset)>180)
	{
		if(imu_sen->yaw_offset>0)
			imu_sen->yaw_offset -= 360;
		else
			imu_sen->yaw_offset += 360;
	}
	imu_sen->yaw_total += imu_sen->yaw_offset;
	imu_sen->offline_cnt=0;
	imu_sen->last_yaw = imu_sen->yaw;
	imu_sen->msg_cnt ++;
}

/**
 * @date        2-November-2021
 * @brief       陀螺仪心跳
 */
void imu_heart_beat(imu_sensor_t *imu_sen)
{
	imu_sen->offline_cnt++;
	if(imu_sen->offline_cnt > imu_sen->offline_max_cnt)
	{
		imu_sen->offline_cnt = imu_sen->offline_max_cnt;
		imu_sen->imu_sen_state = DEV_OFFLINE;
	}
	else if(imu_sen->imu_sen_state == DEV_OFFLINE)
	{
		imu_sen->imu_sen_state = DEV_ONLINE;
	}
}
