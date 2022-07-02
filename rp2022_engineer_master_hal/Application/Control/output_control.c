/**
 * @file        output_control.c
 * @author      RobotPilots@2022
 * @Version     V1.0
 * @date        8-December-2021
 * @brief       输出
 */
#include "can_protocol.h"
#include "output_control.h"
#include "cmsis_os.h"


/**
 * @date        29-November-2021
 * @brief       输出任务
 */
void Output_Task(void const * argument)
{
	for(;;)
  {
		can_send();
    osDelay(2);
  }
}
