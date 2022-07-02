/**
 * @file        rc_protocol.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        12-November-2021
 * @brief       rc_protocol
 */


#include "rc_sensor.h"
#include "rc_protocol.h"


/**
 * @date        2-November-2021
 * @brief       rc_update
 */
void USART2_rxDataHandler(uint8_t *rxBuf)
{
	rc_sensor.ch0 = (rxBuf[0] | (rxBuf[1] << 8)) & 0x07ff;
	rc_sensor.ch0 = rc_sensor.ch0 -=1024;
	rc_sensor.ch1 = ((rxBuf[1] >> 3) | (rxBuf[2] << 5)) & 0x07ff;
	rc_sensor.ch1 -=1024;
	rc_sensor.ch2 = ((rxBuf[2] >> 6) | (rxBuf[3] << 2) |(rxBuf[4] << 10)) & 0x07FF;
	rc_sensor.ch2 -=1024;
	rc_sensor.ch3 = ((rxBuf[4] >> 1) | (rxBuf[5]<<7)) & 0x07FF;
	rc_sensor.ch3 -=1024;
	
	rc_sensor.s1 = ((rxBuf[5] >> 4) & 0x000C) >> 2;
	rc_sensor.s2 = (rxBuf[5] >> 4) & 0x0003;	
	
	rc_sensor.mouse_vx = rxBuf[6]  | (rxBuf[7 ] << 8);
	rc_sensor.mouse_vy = rxBuf[8]  | (rxBuf[9 ] << 8);
	rc_sensor.mouse_vz = rxBuf[10] | (rxBuf[11] << 8);
	rc_sensor.mouse_btn_l = rxBuf[12];
	rc_sensor.mouse_btn_r = rxBuf[13];
	
	rc_sensor.key_v = rxBuf[14] | (rxBuf[15] << 8);	
	
	rc_sensor.thumbwheel = ((int16_t)rxBuf[16] | ((int16_t)rxBuf[17] << 8)) & 0x07ff;
	rc_sensor.thumbwheel -= 1024;
	
	rc_sensor.offline_cnt = 0;
}


