/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
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
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

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
	static uint8_t a = 0, b = 0;
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  // 交替闪烁
  for (size_t i = 0; i < 10; i++)
  {
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10, SET); // 熄灭两个灯泡
    HAL_Delay(20);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10, RESET); // 点亮两个灯泡
    HAL_Delay(20);
  }
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10, SET); // 熄灭两个灯泡
  for(;;)
  {
    if (!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2))
    {                // 检测按键是否按下
      HAL_Delay(20); // 延时去抖动
      if (!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2))
      { // 判断长短键
        while (!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)&& a < 100)
          { // 循环判断长按，到时跳转
            a++;
            HAL_Delay(10); // 长按判断的计时
          }
        if (a >= 100)
        { // 长键处理
          // 长按后执行程序
          HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10, RESET); // 点亮两个灯泡
		  HAL_Delay(2000); // 长按判断的计时
          while (!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2))
            ; // 一直按不退出
        }
        else
        { // 单击处理
          for (b = 0; b < 20; b++)
          { // 检测双击
            HAL_Delay(20);
            if (!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2))
            {
              a = 0;
              // 双击后执行程序
              // 交替闪烁
              for (size_t i = 0; i < 10; i++)
              {
                HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10, SET); // 熄灭两个灯泡
                HAL_Delay(20);
                HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10, RESET); // 点亮两个灯泡
                HAL_Delay(20);
              }
              while (!HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2))
                ; // 等待放手
            }
          }
          if (a < 50 && a!=0)
          { // 判断单击
            // 单击后执行程序 交替闪烁
            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10, SET); // 熄灭两个灯泡
            HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, RESET);             // 点亮一个
            for (size_t i = 0; i < 10; i++)
            {
              HAL_Delay(200);
              HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10);
            }
          }
        }
      }
    } // 按键判断在此结束
	a = 0;
    b = 0; // 参数清0
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9 | GPIO_PIN_10, SET); // 熄灭两个灯泡
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
