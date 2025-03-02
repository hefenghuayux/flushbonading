#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"  // FreeRTOS ͷ�ļ�
#include "Key.h"
#include "robot.h"
#include "myUsart.h"
// ��ʱ�����
extern TIM_HandleTypeDef htim4;

// �����˳�ʼ��
void robot_Init(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);  // ���� PWM ͨ�� 1
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);  // ���� PWM ͨ�� 2
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);  // ���� PWM ͨ�� 3
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);  // ���� PWM ͨ�� 4
}

// ��· PWM �����ٶȵ���
void robot_speed(uint8_t left1_speed, uint8_t left2_speed, uint8_t right1_speed, uint8_t right2_speed, uint8_t time) {
    // �������/���ٵ�ʱ�䣬time ��ʮ��֮һ���ڼ��ٺͼ���
    uint16_t acceleration_time = time / 10;  // ����ʱ��
    uint16_t steady_time = time - 2 * acceleration_time;  // ʣ��ʱ������������ʻ

    uint8_t left1_current_speed = 0, left2_current_speed = 0, right1_current_speed = 0, right2_current_speed = 0;
    uint8_t left1_target_speed = left1_speed, left2_target_speed = left2_speed, right1_target_speed = right1_speed, right2_target_speed = right2_speed;
    
    uint8_t left1_increment = (left1_target_speed > left1_current_speed) ? 1 : -1;
    uint8_t left2_increment = (left2_target_speed > left2_current_speed) ? 1 : -1;
    uint8_t right1_increment = (right1_target_speed > right1_current_speed) ? 1 : -1;
    uint8_t right2_increment = (right2_target_speed > right2_current_speed) ? 1 : -1;

    // ���ٹ���
    for (uint16_t i = 0; i < acceleration_time; i++) {
        // ƽ�������ٶ�
        left1_current_speed += (left1_target_speed - left1_current_speed) / (acceleration_time - i);
        left2_current_speed += (left2_target_speed - left2_current_speed) / (acceleration_time - i);
        right1_current_speed += (right1_target_speed - right1_current_speed) / (acceleration_time - i);
        right2_current_speed += (right2_target_speed - right2_current_speed) / (acceleration_time - i);

        // ��ֹ����Ŀ���ٶ�
        if ( left1_current_speed > left1_target_speed) left1_current_speed = left1_target_speed;
        if ( left2_current_speed > left2_target_speed) left2_current_speed = left2_target_speed;
        if ( right1_current_speed > right1_target_speed ) right1_current_speed = right1_target_speed;
        if ( right2_current_speed > right2_target_speed ) right2_current_speed = right2_target_speed;

        // ���µ���ٶ�
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, left1_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, left2_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, right1_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, right2_current_speed);

        osDelay(10);  // ��ʱ10ms
    }

    // ���ٹ���
    for (uint16_t i = 0; i < steady_time; i++) {
        // ����Ŀ���ٶ�
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, left1_target_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, left2_target_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, right1_target_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, right2_target_speed);

        osDelay(10);  // ��ʱ10ms
    }

    // ���ٹ���
    for (uint16_t i = 0; i < acceleration_time; i++) {
        // ƽ�������ٶ�
        left1_current_speed -= (left1_current_speed - 0) / (acceleration_time - i);
        left2_current_speed -= (left2_current_speed - 0) / (acceleration_time - i);
        right1_current_speed -= (right1_current_speed - 0) / (acceleration_time - i);
        right2_current_speed -= (right2_current_speed - 0) / (acceleration_time - i);

        // ��֤�ٶȲ�С�� 0
        if (left1_current_speed < 0) left1_current_speed = 0;
        if (left2_current_speed < 0) left2_current_speed = 0;
        if (right1_current_speed < 0) right1_current_speed = 0;
        if (right2_current_speed < 0) right2_current_speed = 0;

        // ���µ���ٶ�
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, left1_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, left2_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, right1_current_speed);
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, right2_current_speed);

        osDelay(10);  // ��ʱ10ms
    }

    // ֹͣ���
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
}

// ������ǰ��
void makerobo_run(uint8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(speed, 0, speed, 0, time);                         // �����ٶ�
                                               // FreeRTOS ��ʱ
}

// ɲ��
void makerobo_brake(uint16_t time)
{
    robot_speed(0, 0, 0, 0,time);  // ���ֹͣ
                                              
}

// ��ת
void makerobo_Left(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(0, 0, speed, 0,time);                             // �����ٶ�
}

// ����ת
void makerobo_Spin_Left(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(0, speed, speed, 0,time);                         // �����ٶ�

}

// ��ת
void makerobo_Right(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(speed, 0, 0, 0,time);                             // �����ٶ�
}

// ����ת
void makerobo_Spin_Right(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(speed, 0, 0, speed,time);                         // �����ٶ�

}

// ����
void makerobo_back(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(0, speed, 0, speed,time);                         // �����ٶ�

}
void robot_Run(void){
    if(Key_GetNum() == 1)
    {
    
     makerobo_run(70,5000);//?��?��??1S
     s_printf("robot_Run\r\n");
     
   }
}
