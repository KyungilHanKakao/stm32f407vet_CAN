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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include <stdio.h>
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define SERVER_IP   "192.168.0.27"  // Replace with your server's IP address
#define SERVER_PORT 6000             // Replace with your server's port
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
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void tcp_client_task(void *arg) {

	int sock;
	struct sockaddr_in server_addr;
	char message[] = "Hello from STM32";
	char buffer[1024];
	int bytes_received;

	printf("after\n");
	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("Failed to create socket\n");
		vTaskDelete(NULL);
		return;
	}

	// Set up the server address structure
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	if (!inet_aton(SERVER_IP, &server_addr.sin_addr)) {
			printf("Invalid IP address\n");
			lwip_close(sock);
			return;
		}

	// Set up server address
	//server_addr.sin_family = AF_INET;
	//server_addr.sin_port = 6000;//htons(SERVER_PORT);
	//inet_aton(SERVER_IP, &server_addr.sin_addr);



	// Connect to server
	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Failed to connect to server : %d\n", errno);
		close(sock);
		vTaskDelete(NULL);

		return;
	}

	printf("Connected to server\n");

	// Send data to server
	if (send(sock, message, strlen(message), 0) < 0) {
		printf("Failed to send data\n");
		close(sock);
		vTaskDelete(NULL);
		return;
	}

	printf("Data sent successfully\n");

	// Receive data from server
	bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
	if (bytes_received > 0) {
		buffer[bytes_received] = '\0';  // Null-terminate received data
		printf("Received from server: %s\n", buffer);
	} else {
		printf("Failed to receive data\n");
	}

	// Close socket and clean up
	close(sock);

	vTaskDelete(NULL);  // Delete task after completion
}
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);

extern void MX_LWIP_Init(void);
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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 512);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityLow, 0, 256);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityLow, 0, 256);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN StartDefaultTask */

  osDelay(5000);
  printf("before\n");
  //tcp_client_task((void*)argument);

  int sock;
  	struct sockaddr_in server_addr;
  	char message[] = "Hello from STM32";
  	char buffer[1024];
  	int bytes_received;

  	printf("after\n");
  	// Create socket
  	sock = socket(AF_INET, SOCK_STREAM, 0);
  	if (sock < 0) {
  		printf("Failed to create socket\n");
  		vTaskDelete(NULL);
  		return;
  	}

  	// Set up the server address structure
  	memset(&server_addr, 0, sizeof(server_addr));
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_port = htons(SERVER_PORT);
  	if (!inet_aton(SERVER_IP, &server_addr.sin_addr)) {
  			printf("Invalid IP address\n");
  			lwip_close(sock);
  			return;
  		}

  	// Set up server address
  	//server_addr.sin_family = AF_INET;
  	//server_addr.sin_port = 6000;//htons(SERVER_PORT);
  	//inet_aton(SERVER_IP, &server_addr.sin_addr);



  	// Connect to server
  	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
  		printf("Failed to connect to server : %d\n", errno);
  		close(sock);
  		vTaskDelete(NULL);

  		return;
  	}

  	printf("Connected to server\n");

  	// Send data to server
  	if (send(sock, message, strlen(message), 0) < 0) {
  		printf("Failed to send data\n");
  		close(sock);
  		vTaskDelete(NULL);
  		return;
  	}

  	printf("Data sent successfully\n");

  	// Receive data from server
  	bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
  	if (bytes_received > 0) {
  		buffer[bytes_received] = '\0';  // Null-terminate received data
  		printf("Received from server: %s\n", buffer);
  	} else {
  		printf("Failed to receive data\n");
  	}

  	// Close socket and clean up
  	close(sock);

  	vTaskDelete(NULL);  // Delete task after completion




  	  /* Infinite loop */
  for(;;)
  {
	  osDelay(1);
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
void StartTask02(void const * argument)
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
void StartTask03(void const * argument)
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
