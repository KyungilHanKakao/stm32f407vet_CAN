/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2	;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myTask03 */
osThreadId_t myTask03Handle;
const osThreadAttr_t myTask03_attributes = {
  .name = "myTask03",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void StartTask03(void *argument);

extern void MX_LWIP_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* creation of myTask03 */
  myTask03Handle = osThreadNew(StartTask03, NULL, &myTask03_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */
  uint8_t tx_data[8] = {0};
  	uint32_t tx_mailbox_number;

  	// setup transmit header
  	CAN_TxHeaderTypeDef tx_header;
  	tx_header.StdId = 0x0AA;
  	tx_header.RTR = CAN_RTR_DATA;
  	tx_header.IDE = CAN_ID_STD;
  	tx_header.DLC = 1U;
  	tx_header.TransmitGlobalTime = DISABLE;



  	  /* Infinite loop */
  	  for(;;)
  	  {
  		  printf("send %d\n", tx_data[0]);
  		  tx_data[0] = !tx_data[0];

  		  HAL_CAN_AddTxMessage(&hcan2, &tx_header, tx_data, &tx_mailbox_number);


  		  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
  		  osDelay(1000);
  	  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
	uint8_t tx_data[8] = {0};
	uint32_t tx_mailbox_number;

	// setup transmit header
	CAN_TxHeaderTypeDef tx_header;
	tx_header.StdId = 0x055;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.IDE = CAN_ID_STD;
	tx_header.DLC = 1U;
	tx_header.TransmitGlobalTime = DISABLE;



	  /* Infinite loop */
	  for(;;)
	  {
		  printf("send %d\n", tx_data[0]);
		  tx_data[0] = !tx_data[0];

		  HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data, &tx_mailbox_number);


		  //HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		  osDelay(1000);
	  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void *argument)
{
  /* USER CODE BEGIN StartTask03 */
  /* Infinite loop */

  for(;;)
  {
	  if (!HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin)){
		  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	  }

	  if (!HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin)){
		  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
	  }
	  if (!HAL_GPIO_ReadPin(SW3_GPIO_Port, SW3_Pin)){
		  HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
	  }
    osDelay(1);
  }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// Callback for receiving messages via interrupt
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{

	CAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];

	HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &rx_header, rx_data);

	HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
//	if (rx_header.StdId == 0x34) {
//		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
//	}
//
//	if (rx_header.StdId == 0x43) {
//			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
//	}

}
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{

	CAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];

	HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO1, &rx_header, rx_data);

	if (rx_header.StdId == 0x34) {
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
	}

	if (rx_header.StdId == 0x43) {
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
	}

}
/* USER CODE END Application */

