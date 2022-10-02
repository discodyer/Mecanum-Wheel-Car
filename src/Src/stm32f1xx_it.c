/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f1xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t USART1_TX_BUF[MAX_TX_LEN];   // my_printf的发送缓冲，下文详述其作用。
volatile uint8_t USART1_TX_FLAG = 0; // USART发送标志，启动发送时置1，加volatile防编译器优化
uint8_t u1rxbuf[MAX_RX_LEN];         // 数据接收缓冲1
uint8_t u2rxbuf[MAX_RX_LEN];         // 数据接收缓冲2
uint8_t WhichBufIsReady = 0;         // 双缓存指示器。
// 0:u1rxbuf 被DMA占用接收,  u2rxbuf 可以读取.
// 0:u2rxbuf 被DMA占用接收,  u1rxbuf 可以读取.
uint8_t *p_IsOK = u2rxbuf;        // 指针——指向可以读取的那个缓冲
uint8_t *p_IsToReceive = u1rxbuf; // 指针——指向被占用的那个缓冲
// extern Data_Analyzer(uint8_t *buffer, uint16_t size);
//注意定义的时候要先让这两个指针按照WhichBufIsReady的初始状态先初始化一下。下文详述为什么要这样子。

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
/* USER CODE BEGIN EV */
extern uint8_t RxBuffer[1];
extern uint8_t RxFlag;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
 * @brief This function handles System service call via SWI instruction.
 */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
 * @brief This function handles Pendable request for system service.
 */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles TIM1 update interrupt.
 */
void TIM1_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_IRQn 0 */

  /* USER CODE END TIM1_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_IRQn 1 */

  /* USER CODE END TIM1_UP_IRQn 1 */
}

/**
 * @brief This function handles USART1 global interrupt.
 */
// void USART1_IRQHandler(void)
// {
//   /* USER CODE BEGIN USART1_IRQn 0 */

//   /* USER CODE END USART1_IRQn 0 */
//   HAL_UART_IRQHandler(&huart1);
//   /* USER CODE BEGIN USART1_IRQn 1 */
// 	RxFlag=1;
// 	//HAL_UART_Receive_IT(&huart1,RxBuffer,1);
//   /* USER CODE END USART1_IRQn 1 */
// }

/* USER CODE BEGIN 1 */
/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles DMA1 channel4 global interrupt.
 */
void DMA1_Channel4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */
  if (__HAL_DMA_GET_FLAG(&hdma_usart1_tx, DMA_FLAG_TC4) != RESET) //数据发送完成中断
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart1); //清除串口空闲中断标志位，发送完成那么串口也是空闲态哦~
    USART1_TX_FLAG = 0;                 // 重置发送标志位
    huart1.gState = HAL_UART_STATE_READY;
    hdma_usart1_tx.State = HAL_DMA_STATE_READY;
    __HAL_UNLOCK(&hdma_usart1_tx);
    // 这里疑似是HAL库函数的bug，具体可以参考我给的链接
    // huart1,hdma_usart1_tx 的状态要手动复位成READY状态
    // 不然发送函数会一直以为通道忙，就不再发送数据了！
  }
  /* USER CODE END DMA1_Channel4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
  /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

  /* USER CODE END DMA1_Channel4_IRQn 1 */
}

/**
 * @brief This function handles DMA1 channel5 global interrupt.
 */
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

  /* USER CODE END DMA1_Channel5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  if (RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
  { // 我记得好像HAL库里面没有给串口空闲中断预留专用的回调函数 qaq

    // __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    // 这一部分其实在 HAL_UART_IRQHandler(&huart1) 也完成了。

    HAL_UART_DMAStop(&huart1); // 把DMA接收停掉，防止速度过快导致中断重入，数据被覆写。

    uint32_t data_length = MAX_RX_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
    // 数据总长度=极限接收长度-DMA剩余的接收长度
    if (WhichBufIsReady) // WhichBufIsReady=1
    {
      p_IsOK = u2rxbuf;        // u2rxbuf 可以读取，就绪指针指向它。
      p_IsToReceive = u1rxbuf; // u1rxbuf 作为下一次DMA存储的缓冲，占用指针指向它。
      WhichBufIsReady = 0;     //切换一下指示器状态
    }
    else // WhichBufIsReady=0
    {
      p_IsOK = u1rxbuf;        // u1rxbuf 可以读取，就绪指针指向它。
      p_IsToReceive = u2rxbuf; // u2rxbuf 作为下一次DMA存储的缓冲，占用指针指向它。
      WhichBufIsReady = 1;     //切换一下指示器状态
    }
    //从下面开始可以处理你接收到的数据啦！举个栗子，把你收到的数据原原本本的还回去

    // DMA_USART1_Tx_Data(p_IsOK, data_length); //数据打回去，长度就是数据长度
    Data_Analyzer(p_IsOK, data_length);

    ///不管是复制也好，放进去队列也罢，处理你接收到的数据的代码建议从这里结束
    memset((uint8_t *)p_IsToReceive, 0, MAX_RX_LEN); // 把接收数据的指针指向的缓冲区清空
  }

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  HAL_UART_Receive_DMA(&huart1, p_IsToReceive, MAX_RX_LEN); //数据处理完毕，重新启动接收
  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void USART1_TX_Wait(void)
{
  uint16_t delay = 20000;
  while (USART1_TX_FLAG)
  {
    delay--;
    if (delay == 0)
      return;
  }
}

void DMA_USART1_Tx_Data(uint8_t *buffer, uint16_t size)
{
  USART1_TX_Wait();                             // 等待上一次发送完成（USART1_TX_FLAG为1即还在发送数据）
  USART1_TX_FLAG = 1;                           // USART1发送标志（启动发送）
  HAL_UART_Transmit_DMA(&huart1, buffer, size); // 发送指定长度的数据
}

void my_printf(char *format, ...)
{
  // VA_LIST 是在C语言中解决变参问题的一组宏，
  //所在头文件：#include <stdarg.h>，用于获取不确定个数的参数。
  va_list arg_ptr; //实例化可变长参数列表

  USART1_TX_Wait(); //等待上一次发送完成（USART1_TX_FLAG为1即还在发送数据）

  va_start(arg_ptr, format); //初始化可变参数列表，设置format为可变长列表的起始点（第一个元素）

  // MAX_TX_LEN+1可接受的最大字符数(非字节数，UNICODE一个字符两个字节), 防止产生数组越界
  vsnprintf((char *)USART1_TX_BUF, MAX_TX_LEN + 1, format, arg_ptr);
  //从USART1_TX_BUF的首地址开始拼合，拼合format内容；MAX_TX_LEN+1限制长度，防止产生数组越界

  va_end(arg_ptr); //注意必须关闭

  DMA_USART1_Tx_Data(USART1_TX_BUF, strlen((const char *)USART1_TX_BUF));
  // 记得把buf里面的东西用HAL发出去
}

/* USER CODE END 1 */
