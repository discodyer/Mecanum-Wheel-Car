#ifndef __CAR_H__
#define __CAR_H__

#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "car.h"
#include "stdio.h"
#include "string.h"

#define R1 HAL_GPIO_ReadPin(R1_GPIO_Port, R1_Pin)
#define R2 HAL_GPIO_ReadPin(R2_GPIO_Port, R2_Pin)
#define M1 HAL_GPIO_ReadPin(M1_GPIO_Port, M1_Pin)
#define L2 HAL_GPIO_ReadPin(L2_GPIO_Port, L2_Pin)
#define L1 HAL_GPIO_ReadPin(L1_GPIO_Port, L1_Pin)
#define LED1ON HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET)
#define LED1OFF HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET)
#define LED2ON HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET)
#define LED2OFF HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET)
#define USARTON HAL_GPIO_WritePin(USART_GPIO_Port,USART_Pin,GPIO_PIN_SET)
#define USARTOFF HAL_GPIO_WritePin(USART_GPIO_Port,USART_Pin,GPIO_PIN_RESET)

uint16_t DutyR10=0;//右前轮前进
uint16_t DutyR20=0;//右后轮前进
uint16_t DutyL10=0;//左后轮前进
uint16_t DutyL20=0;//左前轮前进
uint16_t DutyR11=0;//右前轮后退
uint16_t DutyR21=0;//右后轮后退
uint16_t DutyL11=0;//左后轮后退
uint16_t DutyL21=0;//左前轮后退
volatile uint16_t FlagIT;
uint16_t Step;
uint16_t SSS=0;
uint8_t RxFlag=0;
volatile uint8_t num;
uint8_t RxBuffer[1];

class car
{
private:
    uint16_t DutyR10=0;//右前轮前进
    uint16_t DutyR20=0;//右后轮前进
    uint16_t DutyL10=0;//左后轮前进
    uint16_t DutyL20=0;//左前轮前进
    uint16_t DutyR11=0;//右前轮后退
    uint16_t DutyR21=0;//右后轮后退
    uint16_t DutyL11=0;//左后轮后退
    uint16_t DutyL21=0;//左前轮后退
public:
    car(/* args */);
    ~car();
};

car::car(/* args */)
{
}

car::~car()
{
}

int fputc(int ch,FILE *f)//重定义printf
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);
	return ch;
}
void PWMOUT(void)
{
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, DutyR10);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, DutyR20);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, DutyL10);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, DutyL20);	
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, DutyR11); 
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, DutyR21);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, DutyL11);
	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, DutyL21);
}
void Stop()//停车
{
	DutyR10=1000;
	DutyR20=1000;
	DutyL10=1000;
	DutyL20=1000;
	DutyR11=1000;
	DutyR21=1000;
	DutyL11=1000;
	DutyL21=1000;
	PWMOUT();
	FlagIT=1;
	HAL_TIM_Base_Stop_IT(&htim1);
}
void Straight()//直行
{
	DutyR10=160;
	DutyR20=160;
	DutyL10=160;
	DutyL20=160;
	DutyR11=0;
	DutyR21=0;
	DutyL11=0;
	DutyL21=0;
	PWMOUT();
}
void Right(uint16_t i)//向右偏移
{
	DutyR10=160;
	DutyR20=160;
	DutyL10=160+(20*i);
	DutyL20=160+(20*i);
	DutyR11=0;
	DutyR21=0;
	DutyL11=0;
	DutyL21=0;
	PWMOUT();
}
void Left(uint16_t i)//向左偏移
{
	DutyR10=160+(20*i);
	DutyR20=160+(20*i);
	DutyL10=160;
	DutyL20=160;
	DutyR11=0;
	DutyR21=0;
	DutyL11=0;
	DutyL21=0;
	PWMOUT();
}
void TurnLeft()//右转
{
	DutyR10=170;
	DutyR20=170;
	DutyL10=0;
	DutyL20=0;
	DutyR11=0;
	DutyR21=0;
	DutyL11=170;
	DutyL21=170;
	PWMOUT();
	HAL_Delay(750);
	DutyR10=0;
	DutyR20=0;
	DutyL10=0;
	DutyL20=0;
	DutyR11=0;
	DutyR21=0;
	DutyL11=0;
	DutyL21=0;
	PWMOUT();
}
void TurnRight()//左转
{
	DutyR10=0;
	DutyR20=0;
	DutyL10=170;
	DutyL20=170;
	DutyR11=170;
	DutyR21=170;
	DutyL11=0;
	DutyL21=0;
	PWMOUT();
	HAL_Delay(750);
	DutyR10=0;
	DutyR20=0;
	DutyL10=0;
	DutyL20=0;
	DutyR11=0;
	DutyR21=0;
	DutyL11=0;
	DutyL21=0;
	PWMOUT();
}
void TurnBack()//180转
{
	DutyR10=0;
	DutyR20=0;
	DutyL10=170;
	DutyL20=170;
	DutyR11=170;
	DutyR21=170;
	DutyL11=0;
	DutyL21=0;
	PWMOUT();
	HAL_Delay(1450);
	DutyR10=0;
	DutyR20=0;
	DutyL10=0;
	DutyL20=0;
	DutyR11=0;
	DutyR21=0;
	DutyL11=0;
	DutyL21=0;
	PWMOUT();
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//定时器1中断回调函数
{ 
	if (htim == (&htim1))
    {
			if( R1 &&  R2 &&  M1 &&  L2 &&  L1)   Stop();			//全黑停车
			if(!R1 && !R2 && !M1 && !L2 && !L1)   Stop();			//全白停车
			if(!R1 && !R2 &&  M1 && !L2 && !L1)   Straight();	//中  	11011
			if(!R1 &&  R2 &&  M1 && !L2 && !L1)   Right(1);		//右1   10011
			if(!R1 &&  R2 && !M1 && !L2 && !L1)   Right(2);		//右2   10111
			if( R1 &&  R2 && !M1 && !L2 && !L1)	  Right(4);		//右3   00111
			if( R1 && !R2 && !M1 && !L2 && !L1)   Right(6);		//右3   01111
			if(!R1 && !R2 &&  M1 &&  L2 && !L1)   Left(1);		//左1   11001
			if(!R1 && !R2 && !M1 &&  L2 && !L1)   Left(2);		//左2   11101
			if(!R1 && !R2 && !M1 &&  L2 &&  L1)   Left(4);		//左3   11100
			if(!R1 && !R2 && !M1 && !L2 &&  L1)   Left(6);		//左3   11110				  
    }
}

#endif