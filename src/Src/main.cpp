/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "car.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
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

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	//uint8_t num;
	uint16_t flag=0;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	printf("USART READY!\n");
	//HAL_UART_Receive_IT(&huart1,RxBuffer,1);//开始串口转化
	FlagIT=0;
	Step=0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		USARTOFF;
		// LED1OFF;
		// LED2OFF;
		// while(!HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin));//判断按键，开始程序
		// LED1ON;
		// LED2ON;
		HAL_Delay(500);
		// LED1OFF;
		// LED2OFF;
		HAL_Delay(1000);
		RxBuffer[0]=0;
		USARTON;
		HAL_UART_Receive_IT(&huart1,RxBuffer,1);
//		HAL_UART_Receive_IT(&huart1,RxBuffer,1);//开始串口转化
		while(1)
		{
			if(RxFlag)
			{
				RxFlag=0;
				if((RxBuffer[0]=='1') || (RxBuffer[0]=='2')) 
				{
					USARTOFF;
					break;
				}
				RxBuffer[0]=0;
				HAL_UART_Receive_IT(&huart1,RxBuffer,1);	
			}
		}
		switch(RxBuffer[0])
		{
			case '1':flag=1;LED1ON; break;
			case '2':flag=2;LED2ON; break;	
		}
		
		
		//======================================================
		if(flag==1)
		{
			while(!HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)) LED1ON;
			LED1OFF;
			HAL_Delay(1000);
			Straight();
			HAL_Delay(100);
			HAL_TIM_Base_Start_IT(&htim1);
			Step=1;
			while(!FlagIT);
			Straight();
			HAL_Delay(200);
			HAL_TIM_Base_Start_IT(&htim1);
			Step=2;
			while(!HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin));
			HAL_Delay(1000);
			TurnBack();
			HAL_Delay(200);
			Straight();
			HAL_Delay(50);
			FlagIT=0;
			HAL_TIM_Base_Start_IT(&htim1);
			Step=3;
			while(!FlagIT);
			FlagIT=0;
			Straight();
			HAL_Delay(200);
			Step=4;
			HAL_TIM_Base_Start_IT(&htim1);
			while(!FlagIT);
			HAL_Delay(100);
			Straight();
			HAL_Delay(400);
			TurnBack();
			Step=5;			
		}
		if(flag==2)
		{
			while(!HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)) LED2ON;
			LED2OFF;
			HAL_Delay(1000);
			Straight();
			HAL_Delay(100);
			HAL_TIM_Base_Start_IT(&htim1);
			Step=1;
			while(!FlagIT);
			Straight();
			HAL_Delay(125);
			TurnRight();
			HAL_Delay(200);
			Straight();
			HAL_Delay(50);
			HAL_TIM_Base_Start_IT(&htim1);
			Step=2;
			while(!HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin));
			HAL_Delay(1000);
			TurnBack();
			HAL_Delay(200);
			Straight();
			HAL_Delay(50);
			FlagIT=0;
			HAL_TIM_Base_Start_IT(&htim1);
			Step=3;
			while(!FlagIT);
			FlagIT=0;
			Straight();
			HAL_Delay(125);
			TurnLeft();
			HAL_Delay(200);
			Straight();
			HAL_Delay(50);
			Step=4;
			HAL_TIM_Base_Start_IT(&htim1);
			while(!FlagIT);
			HAL_Delay(100);
			Straight();
			HAL_Delay(400);
			TurnBack();
		}
		FlagIT=0;
		flag=0;
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

