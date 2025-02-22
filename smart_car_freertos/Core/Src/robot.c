#include "main.h"
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"  // FreeRTOS ͷ�ļ�
#include "Key.h"
#include "robot.h"

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
void robot_speed(uint8_t left1_speed, uint8_t left2_speed, uint8_t right1_speed, uint8_t right2_speed)
{
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, left1_speed);  // ����ͨ�� 1 ռ�ձ�
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, left2_speed);  // ����ͨ�� 2 ռ�ձ�
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, right1_speed); // ����ͨ�� 3 ռ�ձ�
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, right2_speed); // ����ͨ�� 4 ռ�ձ�
}

// ������ǰ��
void makerobo_run(uint8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(speed, 0, speed, 0);                         // �����ٶ�
    osDelay(time);                                           // FreeRTOS ��ʱ
    robot_speed(0, 0, 0, 0);                                 // ֹͣ
}

// ɲ��
void makerobo_brake(uint16_t time)
{
    robot_speed(0, 0, 0, 0);  // ���ֹͣ
    osDelay(time);            // FreeRTOS ��ʱ
}

// ��ת
void makerobo_Left(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(0, 0, speed, 0);                             // �����ٶ�
    osDelay(time);                                           // FreeRTOS ��ʱ
    robot_speed(0, 0, 0, 0);                                 // ֹͣ
}

// ����ת
void makerobo_Spin_Left(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(0, speed, speed, 0);                         // �����ٶ�
    osDelay(time);                                           // FreeRTOS ��ʱ
    robot_speed(0, 0, 0, 0);                                 // ֹͣ
}

// ��ת
void makerobo_Right(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(speed, 0, 0, 0);                             // �����ٶ�
    osDelay(time);                                           // FreeRTOS ��ʱ
    robot_speed(0, 0, 0, 0);                                 // ֹͣ
}

// ����ת
void makerobo_Spin_Right(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(speed, 0, 0, speed);                         // �����ٶ�
    osDelay(time);                                           // FreeRTOS ��ʱ
    robot_speed(0, 0, 0, 0);                                 // ֹͣ
}

// ����
void makerobo_back(int8_t speed, uint16_t time)
{
    speed = (speed > 100) ? 100 : ((speed < 0) ? 0 : speed);  // �޷�
    robot_speed(0, speed, 0, speed);                         // �����ٶ�
    osDelay(time);                                           // FreeRTOS ��ʱ
    robot_speed(0, 0, 0, 0);                                 // ֹͣ
}
void robot_Run(void){
    if(Key_GetNum() == 1)
    {
     makerobo_run(70,5000);//?��?��??1S
     makerobo_brake(500);//??????0.5S
   }
}
