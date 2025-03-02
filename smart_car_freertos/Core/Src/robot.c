#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"  // FreeRTOS 头文件
#include "Key.h"
#include "robot.h"
#include "myUsart.h"
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
void robot_speed(uint8_t left1_speed, uint8_t left2_speed, uint8_t right1_speed, uint8_t right2_speed, uint8_t time) {
    // 计算加速/减速的时间，time 的十分之一用于加速和减速
    uint16_t acceleration_time = time / 10;  // 加速时间
    uint16_t steady_time = time - 2 * acceleration_time;  // 剩余时间用于匀速行驶

    uint8_t left1_current_speed = 0, left2_current_speed = 0, right1_current_speed = 0, right2_current_speed = 0;
    uint8_t left1_target_speed = left1_speed, left2_target_speed = left2_speed, right1_target_speed = right1_speed, right2_target_speed = right2_speed;
    
    uint8_t left1_increment = (left1_target_speed > left1_current_speed) ? 1 : -1;
    uint8_t left2_increment = (left2_target_speed > left2_current_speed) ? 1 : -1;
    uint8_t right1_increment = (right1_target_speed > right1_current_speed) ? 1 : -1;
    uint8_t right2_increment = (right2_target_speed > right2_current_speed) ? 1 : -1;

    // 加速过程
    for (uint16_t i = 0; i < acceleration_time; i++) {
        // 平滑增加速度
        left1_current_speed += (left1_target_speed - left1_current_speed) / (acceleration_time - i);
        left2_current_speed += (left2_target_speed - left2_current_speed) / (acceleration_time - i);
        right1_current_speed += (right1_target_speed - right1_current_speed) / (acceleration_time - i);
        right2_current_speed += (right2_target_speed - right2_current_speed) / (acceleration_time - i);

        // 防止超过目标速度
        if ( left1_current_speed > left1_target_speed) left1_current_speed = left1_target_speed;
        if ( left2_current_speed > left2_target_speed) left2_current_speed = left2_target_speed;
        if ( right1_current_speed > right1_target_speed ) right1_current_speed = right1_target_speed;
        if ( right2_current_speed > right2_target_speed ) right2_current_speed = right2_target_speed;

        // 更新电机速度
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, left1_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, left2_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, right1_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, right2_current_speed);

        osDelay(10);  // 延时10ms
    }

    // 匀速过程
    for (uint16_t i = 0; i < steady_time; i++) {
        // 保持目标速度
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, left1_target_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, left2_target_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, right1_target_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, right2_target_speed);

        osDelay(10);  // 延时10ms
    }

    // 减速过程
    for (uint16_t i = 0; i < acceleration_time; i++) {
        // 平滑减少速度
        left1_current_speed -= (left1_current_speed - 0) / (acceleration_time - i);
        left2_current_speed -= (left2_current_speed - 0) / (acceleration_time - i);
        right1_current_speed -= (right1_current_speed - 0) / (acceleration_time - i);
        right2_current_speed -= (right2_current_speed - 0) / (acceleration_time - i);

        // 保证速度不小于 0
        if (left1_current_speed < 0) left1_current_speed = 0;
        if (left2_current_speed < 0) left2_current_speed = 0;
        if (right1_current_speed < 0) right1_current_speed = 0;
        if (right2_current_speed < 0) right2_current_speed = 0;

        // 更新电机速度
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, left1_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, left2_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, right1_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, right2_current_speed);

        osDelay(10);  // 延时10ms
    }

    // 停止电机
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
}

// 机器人前进
void makerobo_run(uint8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(speed, 0, speed, 0, time);                         // 设置速度
                                               // FreeRTOS 延时
}

// 刹车
void makerobo_brake(uint16_t time)
{
    robot_speed(0, 0, 0, 0,time);  // 电机停止
                                              
}

// 左转
void makerobo_Left(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(0, 0, speed, 0,time);                             // 设置速度
}

// 左旋转
void makerobo_Spin_Left(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(0, speed, speed, 0,time);                         // 设置速度

}

// 右转
void makerobo_Right(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(speed, 0, 0, 0,time);                             // 设置速度
}

// 右旋转
void makerobo_Spin_Right(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(speed, 0, 0, speed,time);                         // 设置速度

}

// 后退
void makerobo_back(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // 限幅
    robot_speed(0, speed, 0, speed,time);                         // 设置速度

}
void robot_Run(void){
    if(Key_GetNum() == 1)
    {
    
     makerobo_run(70,5000);//?‰?è??1S
     s_printf("robot_Run\r\n");
     
   }
}
