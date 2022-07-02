/**
 * @file        vision_protocol.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        12-November-2021
 * @brief       vision_protocol
 */

#include "vision_protocol.h"
#include "usart.h"
#include "crc.h"
#include "stdbool.h"
#include "string.h"


/**
 * @date        12-November-2021
 * @brief       视觉通信接收
 */
void USART3_rxDataHandler(uint8_t *rxBuf)
{
	uint8_t res = false;
		/* 帧首字节是否为0xA5 */
	if(rxBuf[sof] == VISION_SOF) 
	{	
			res = Verify_CRC8_Check_Sum(rxBuf, VISION_LEN_HEADER );
		/* 帧头CRC8校验*/
		if(res == true)
		{
			res = Verify_CRC16_Check_Sum(rxBuf, VISION_LEN_PACKED );
			/* 帧尾CRC16校验 */
			if(res == true) 
				memcpy(&vision_sensor.rx,rxBuf,VISION_LEN_PACKED);
		}			
	}
}


/**
 * @date        12-November-2021
 * @brief       视觉通信发送
 */
uint8_t vision_Txbuffer[VISION_LEN_PACKED];
uint32_t vision_send_time = 0;
void vision_send()
{
	vision_send_time ++;
	if(vision_send_time%4 == 0)
	{
		vision_send_time = 0;
		memcpy(vision_Txbuffer,&vision_sensor.tx,VISION_LEN_PACKED);
		Append_CRC8_Check_Sum(vision_Txbuffer,VISION_LEN_HEADER);
		Append_CRC16_Check_Sum(vision_Txbuffer,VISION_LEN_PACKED);
		HAL_UART_Transmit_DMA(&huart3,vision_Txbuffer,VISION_LEN_PACKED);
	}
}



