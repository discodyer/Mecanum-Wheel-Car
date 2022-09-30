#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "car.h"

#include "stdio.h"
#include "string.h"

volatile uint16_t FlagIT;
uint16_t Step;
uint16_t SSS = 0;
uint8_t RxFlag = 0;
volatile uint8_t num;
uint8_t RxBuffer[1];

void SystemClock_Config(void);
int fputc(int ch, FILE *f);

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	HAL_Init();

	// uint16_t flag=0;

	SystemClock_Config();

	MX_GPIO_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_USART1_UART_Init();
	MX_TIM1_Init();

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
	printf("USART READY!\n");
	HAL_UART_Receive_IT(&huart1,RxBuffer,1);//开始串口转化
	FlagIT = 0;
	Step = 0;
	car c;

	while (1)
	{
		c.move();
	}
}

int fputc(int ch, FILE *f) //重定义printf
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //定时器1中断回调函数
{
    // if (htim == (&htim1))
    // {
    // 		if( R1 &&  R2 &&  M1 &&  L2 &&  L1)   Stop();			//全黑停车
    // 		if(!R1 && !R2 && !M1 && !L2 && !L1)   Stop();			//全白停车
    // 		if(!R1 && !R2 &&  M1 && !L2 && !L1)   Straight();	//中  	11011
    // 		if(!R1 &&  R2 &&  M1 && !L2 && !L1)   Right(1);		//右1   10011
    // 		if(!R1 &&  R2 && !M1 && !L2 && !L1)   Right(2);		//右2   10111
    // 		if( R1 &&  R2 && !M1 && !L2 && !L1)	  Right(4);		//右3   00111
    // 		if( R1 && !R2 && !M1 && !L2 && !L1)   Right(6);		//右3   01111
    // 		if(!R1 && !R2 &&  M1 &&  L2 && !L1)   Left(1);		//左1   11001
    // 		if(!R1 && !R2 && !M1 &&  L2 && !L1)   Left(2);		//左2   11101
    // 		if(!R1 && !R2 && !M1 &&  L2 &&  L1)   Left(4);		//左3   11100
    // 		if(!R1 && !R2 && !M1 && !L2 &&  L1)   Left(6);		//左3   11110
    // }
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

#ifdef USE_FULL_ASSERT
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
