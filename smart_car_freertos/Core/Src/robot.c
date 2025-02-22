#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"  // FreeRTOS 头文件
#include "Key.h"
#include "robot.h"

// 定时器句柄
extern TIM_HandleTypeDef htim4;

// 机器人初始化
void robot_Init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);  // 启动 PWM 通道 1
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);  // 启动 PWM 通道 2
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);  // 启动 PWM 通道 3
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);  // 启动 PWM 通道 4
}

// 四路 PWM 控制速度调节
void robot_speed(uint8_t left1_speed, uint8_t left2_speed, uint8_t right1_speed, uint8_t right2_speed)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, left1_speed);  // 设置通道 1 占空比
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, left2_speed);  // 设置通道 2 占空比
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, right1_speed); // 设置通道 3 占空比
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, right2_speed); // 设置通道 4 占空比
}

// 机器人前进
void makerobo_run(uint8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(speed, 0, speed, 0);                         // 设置速度
    osDelay(time);                                           // FreeRTOS 延时
    robot_speed(0, 0, 0, 0);                                 // 停止
}

// 刹车
void makerobo_brake(uint16_t time)
{
    robot_speed(0, 0, 0, 0);  // 电机停止
    osDelay(time);            // FreeRTOS 延时
}

// 左转
void makerobo_Left(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(0, 0, speed, 0);                             // 设置速度
    osDelay(time);                                           // FreeRTOS 延时
    robot_speed(0, 0, 0, 0);                                 // 停止
}

// 左旋转
void makerobo_Spin_Left(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(0, speed, speed, 0);                         // 设置速度
    osDelay(time);                                           // FreeRTOS 延时
    robot_speed(0, 0, 0, 0);                                 // 停止
}

// 右转
void makerobo_Right(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(speed, 0, 0, 0);                             // 设置速度
    osDelay(time);                                           // FreeRTOS 延时
    robot_speed(0, 0, 0, 0);                                 // 停止
}

// 右旋转
void makerobo_Spin_Right(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(speed, 0, 0, speed);                         // 设置速度
    osDelay(time);                                           // FreeRTOS 延时
    robot_speed(0, 0, 0, 0);                                 // 停止
}

// 后退
void makerobo_back(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(0, speed, 0, speed);                         // 设置速度
    osDelay(time);                                           // FreeRTOS 延时
    robot_speed(0, 0, 0, 0);                                 // 停止
}
void robot_Run(void){
    if(Key_GetNum() == 1)
    {
     makerobo_run(70,5000);//?‰?è??1S
     makerobo_brake(500);//??????0.5S
   }
}
