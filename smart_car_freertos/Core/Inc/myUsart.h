#ifndef __MYUSART_H
#define __MYUSART_H
#include "cJSON.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "stdarg.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"
#include "task.h"
#include <robot.h>
#include <stm32f1xx_hal_usart.h>
#include <string.h>


void myusart_init(void);
extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;
void hhSerialSendByte(uint8_t Byte, UART_HandleTypeDef *huart);
void hhSerialSendArray(uint8_t *Array, uint16_t Length, UART_HandleTypeDef *huart);
void hhSerialSendString(char *s, UART_HandleTypeDef *huart) ;
void hhSerialSendNum(uint32_t Num, uint8_t Length, UART_HandleTypeDef *huart);
void s_printf(char * format,...);
void HC_05_RUN(void);
void HC_05_Receive(void);
#endif