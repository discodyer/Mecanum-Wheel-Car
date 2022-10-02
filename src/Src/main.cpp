#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "car.h"
#include "dma.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

void SystemClock_Config(void);
void Data_Analyzer(uint8_t *buffer, uint16_t size);
// int fputc(int ch, FILE *f);

car c;

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
	MX_DMA_Init();
	MX_USART1_UART_Init();

	// 开启串口1空闲中断
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

	// 开启DMA发送通道的发送完成中断，才能实现封装发送函数里面的等待功能
	__HAL_DMA_ENABLE_IT(&hdma_usart1_tx, DMA_IT_TC);

	// 清除空闲标志位，防止中断误入
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);

	// 立即就要打开DMA接收
	// 不然DMA没有提前准备，第一次接收的数据是读取不出来的
	HAL_UART_Receive_DMA(&huart1, p_IsToReceive, MAX_RX_LEN);

	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	my_printf("USART READY!\n");

	while (1)
	{
	}
}

// int fputc(int ch, FILE *f) //重定义printf
// {
// 	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
// 	return ch;
// }

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

void Data_Analyzer(uint8_t *buffer, uint16_t size)
{
	int16_t x = 0, y = 0, r = 0;
	if (size < 3)
	{
		my_printf("no\n");
		return;
	}
	if (buffer[0] != '(')
	{
		my_printf("no1\n");
		return;
	}
	if (buffer[size - 1] != '\n' && buffer[size - 2] != ')')
	{
		my_printf("no-1\n");
		return;
	}
	if (buffer[size - 1] == '\n' && buffer[size - 2] == ')' && buffer[size - 3] == '(')
	{
		c.Stop();
		my_printf("ok\n");
		return;
	}

	sscanf((char *)buffer, "(%hd,%hd,%hd)%[\n]", &x, &y, &r);

	my_printf("ok\n");

	c.move(x, y, r);
}