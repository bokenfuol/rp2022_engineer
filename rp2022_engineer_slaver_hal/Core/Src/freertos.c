/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId heartbeatHandle;
osThreadId chassistaskHandle;
osThreadId controltaskHandle;
osThreadId outputtaskHandle;
osThreadId myTask05Handle;
osThreadId autoprocessHandle;
osThreadId autoprocesstaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Heart_Beat_Task(void const * argument);
void Chassis_Task(void const * argument);
void Control_Task(void const * argument);
void Output_Task(void const * argument);
void StartTask05(void const * argument);
void Auto_Process_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of heartbeat */
  osThreadDef(heartbeat, Heart_Beat_Task, osPriorityHigh, 0, 128);
  heartbeatHandle = osThreadCreate(osThread(heartbeat), NULL);

  /* definition and creation of chassistask */
  osThreadDef(chassistask, Chassis_Task, osPriorityNormal, 0, 128);
  chassistaskHandle = osThreadCreate(osThread(chassistask), NULL);

  /* definition and creation of controltask */
  osThreadDef(controltask, Control_Task, osPriorityAboveNormal, 0, 128);
  controltaskHandle = osThreadCreate(osThread(controltask), NULL);

  /* definition and creation of outputtask */
  osThreadDef(outputtask, Output_Task, osPriorityNormal, 0, 128);
  outputtaskHandle = osThreadCreate(osThread(outputtask), NULL);

  /* definition and creation of myTask05 */
  osThreadDef(myTask05, StartTask05, osPriorityIdle, 0, 128);
  myTask05Handle = osThreadCreate(osThread(myTask05), NULL);

  /* definition and creation of autoprocess */
  osThreadDef(autoprocess, Auto_Process_Task, osPriorityAboveNormal, 0, 128);
  autoprocessHandle = osThreadCreate(osThread(autoprocess), NULL);

  /* definition and creation of autoprocesstask */
  osThreadDef(autoprocesstask, Auto_Process_Task, osPriorityAboveNormal, 0, 128);
  autoprocesstaskHandle = osThreadCreate(osThread(autoprocesstask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Heart_Beat_Task */
/**
  * @brief  Function implementing the heartbeat thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Heart_Beat_Task */
__weak void Heart_Beat_Task(void const * argument)
{
  /* USER CODE BEGIN Heart_Beat_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Heart_Beat_Task */
}

/* USER CODE BEGIN Header_Chassis_Task */
/**
* @brief Function implementing the chassistask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Chassis_Task */
__weak void Chassis_Task(void const * argument)
{
  /* USER CODE BEGIN Chassis_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Chassis_Task */
}

/* USER CODE BEGIN Header_Control_Task */
/**
* @brief Function implementing the controltask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Control_Task */
__weak void Control_Task(void const * argument)
{
  /* USER CODE BEGIN Control_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Control_Task */
}

/* USER CODE BEGIN Header_Output_Task */
/**
* @brief Function implementing the outputtask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Output_Task */
__weak void Output_Task(void const * argument)
{
  /* USER CODE BEGIN Output_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Output_Task */
}

/* USER CODE BEGIN Header_StartTask05 */
/**
* @brief Function implementing the myTask05 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask05 */
void StartTask05(void const * argument)
{
  /* USER CODE BEGIN StartTask05 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTask05 */
}

/* USER CODE BEGIN Header_Auto_Process_Task */
/**
* @brief Function implementing the autoprocess thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Auto_Process_Task */
__weak void Auto_Process_Task(void const * argument)
{
  /* USER CODE BEGIN Auto_Process_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Auto_Process_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
