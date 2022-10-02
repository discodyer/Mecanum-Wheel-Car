#include "car.h"

car::car()
{
    this->DutyR10 = 0; //右前轮前进
    this->DutyR20 = 0; //右后轮前进
    this->DutyL10 = 0; //左后轮前进
    this->DutyL20 = 0; //左前轮前进
    this->DutyR11 = 0; //右前轮后退
    this->DutyR21 = 0; //右后轮后退
    this->DutyL11 = 0; //左后轮后退
    this->DutyL21 = 0; //左前轮后退
}

car::~car()
{
}

/**
 * @brief move car
 *
 * @param x 表示 X 轴运动的速度，即左右方向，定义向右为正
 * @param y 表示 Y 轴运动的速度，即前后方向，定义向前为正
 * @param r 表示 yaw 轴自转的角速度，定义逆时针为正
 */
void car::move(uint16_t x, uint16_t y, uint16_t r)
{
    uint16_t w1, w2, w3, w4;
    w1 = y - x + r; //右前轮
    w2 = y + x - r; //左前轮
    w3 = y - x - r; //左后轮
    w4 = y + x + r; //右后轮

    if (w1 < 0)
    {
        this->DutyR10 = 0; //右前轮前进
        this->DutyR11 = 0; //右前轮后退
    }
    else
    {
        this->DutyR10 = 0; //右前轮前进
        this->DutyR11 = 0; //右前轮后退
    }

    if (w2 < 0)
    {
        this->DutyL20 = 0; //左前轮前进
        this->DutyL21 = 0; //左前轮后退
    }
    else
    {
        this->DutyL20 = 0; //左前轮前进
        this->DutyL21 = 0; //左前轮后退
    }

    if (w3 < 0)
    {
        this->DutyL10 = 0; //左后轮前进
        this->DutyL11 = 0; //左后轮后退
    }
    else
    {
        this->DutyL10 = 0; //左后轮前进
        this->DutyL11 = 0; //左后轮后退
    }

    if (w4 < 0)
    {
        this->DutyR20 = 0; //右后轮前进
        this->DutyR21 = 0; //右后轮后退
    }
    else
    {
        this->DutyR20 = 0; //右后轮前进
        this->DutyR21 = 0; //右后轮后退
    }

    this->PWMOUT();
}

void car::PWMOUT()
{
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, this->DutyR10);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, this->DutyR20);
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, this->DutyL10);
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, this->DutyL20);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, this->DutyR11);
    __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, this->DutyR21);
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, this->DutyL11);
    __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, this->DutyL21);
}

void car::Stop()
{
    this->DutyR10 = 1000;
    this->DutyR20 = 1000;
    this->DutyL10 = 1000;
    this->DutyL20 = 1000;
    this->DutyR11 = 1000;
    this->DutyR21 = 1000;
    this->DutyL11 = 1000;
    this->DutyL21 = 1000;
    this->PWMOUT();
    HAL_TIM_Base_Stop_IT(&htim1);
}

void car::Straight() //直行
{
    this->DutyR10 = 160;
    this->DutyR20 = 160;
    this->DutyL10 = 160;
    this->DutyL20 = 160;
    this->DutyR11 = 0;
    this->DutyR21 = 0;
    this->DutyL11 = 0;
    this->DutyL21 = 0;
    this->PWMOUT();
}

void car::Right(uint16_t i) //向右偏移
{
    this->DutyR10 = 160;
    this->DutyR20 = 160;
    this->DutyL10 = 160 + (20 * i);
    this->DutyL20 = 160 + (20 * i);
    this->DutyR11 = 0;
    this->DutyR21 = 0;
    this->DutyL11 = 0;
    this->DutyL21 = 0;
    this->PWMOUT();
}

void car::Left(uint16_t i) //向左偏移
{
    this->DutyR10 = 160 + (20 * i);
    this->DutyR20 = 160 + (20 * i);
    this->DutyL10 = 160;
    this->DutyL20 = 160;
    this->DutyR11 = 0;
    this->DutyR21 = 0;
    this->DutyL11 = 0;
    this->DutyL21 = 0;
    this->PWMOUT();
}

void car::TurnLeft() //右转
{
    this->DutyR10 = 170;
    this->DutyR20 = 170;
    this->DutyL10 = 0;
    this->DutyL20 = 0;
    this->DutyR11 = 0;
    this->DutyR21 = 0;
    this->DutyL11 = 170;
    this->DutyL21 = 170;
    this->PWMOUT();
    HAL_Delay(750);
    this->DutyR10 = 0;
    this->DutyR20 = 0;
    this->DutyL10 = 0;
    this->DutyL20 = 0;
    this->DutyR11 = 0;
    this->DutyR21 = 0;
    this->DutyL11 = 0;
    this->DutyL21 = 0;
    this->PWMOUT();
}

void car::TurnRight() //左转
{
    this->DutyR10 = 0;
    this->DutyR20 = 0;
    this->DutyL10 = 170;
    this->DutyL20 = 170;
    this->DutyR11 = 170;
    this->DutyR21 = 170;
    this->DutyL11 = 0;
    this->DutyL21 = 0;
    this->PWMOUT();
    HAL_Delay(750);
    this->DutyR10 = 0;
    this->DutyR20 = 0;
    this->DutyL10 = 0;
    this->DutyL20 = 0;
    this->DutyR11 = 0;
    this->DutyR21 = 0;
    this->DutyL11 = 0;
    this->DutyL21 = 0;
    this->PWMOUT();
}

void car::TurnBack() // 180转
{
    this->DutyR10 = 0;
    this->DutyR20 = 0;
    this->DutyL10 = 170;
    this->DutyL20 = 170;
    this->DutyR11 = 170;
    this->DutyR21 = 170;
    this->DutyL11 = 0;
    this->DutyL21 = 0;
    this->PWMOUT();
    HAL_Delay(1450);
    this->DutyR10 = 0;
    this->DutyR20 = 0;
    this->DutyL10 = 0;
    this->DutyL20 = 0;
    this->DutyR11 = 0;
    this->DutyR21 = 0;
    this->DutyL11 = 0;
    this->DutyL21 = 0;
    this->PWMOUT();
}
