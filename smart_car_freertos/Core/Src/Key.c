#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Key.h"
#include "cmsis_os.h"


uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET)
    {    
        osDelay(10);
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15) == GPIO_PIN_RESET);
        osDelay(10);
        KeyNum = 1;
    }
    return KeyNum;
}