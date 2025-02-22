#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"



uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET)
    {
        vTaskDelay(pdMS_TO_TICKS(20)); // 使用FreeRTOS的延时函数
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(20)); // 使用FreeRTOS的延时函数
        KeyNum = 1;
    }
    return KeyNum;
}