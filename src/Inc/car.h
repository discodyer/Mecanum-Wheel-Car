#ifndef __CAR_H__
#define __CAR_H__

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
#define LED1ON HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1OFF HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET)
#define LED2ON HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET)
#define LED2OFF HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET)
#define USARTON HAL_GPIO_WritePin(USART_GPIO_Port, USART_Pin, GPIO_PIN_SET)
#define USARTOFF HAL_GPIO_WritePin(USART_GPIO_Port, USART_Pin, GPIO_PIN_RESET)

class car
{
private:
	uint16_t DutyR10; //右前轮前进
	uint16_t DutyR20; //右后轮前进
	uint16_t DutyL10; //左后轮前进
	uint16_t DutyL20; //左前轮前进
	uint16_t DutyR11; //右前轮后退
	uint16_t DutyR21; //右后轮后退
	uint16_t DutyL11; //左后轮后退
	uint16_t DutyL21; //左前轮后退

	void PWMOUT();
	void Straight();		//直行
	void Right(uint16_t i); //向右偏移
	void Left(uint16_t i);	//向左偏移
	void TurnLeft();		//左转
	void TurnRight();		//右转
	void TurnBack();		// 180转

public:
	car();
	void move(uint16_t x, uint16_t y, uint16_t r);
	void Stop(); //停车
	~car();
};

#endif