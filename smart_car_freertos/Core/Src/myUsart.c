#include "stm32f1xx_hal.h"
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "stdarg.h"
#include "myUsart.h"
#include <stm32f1xx_hal_usart.h>
#include "stm32f1xx_it.h"
#include "stm32f1xx_hal.h"
#include <string.h>
#include <robot.h>
#include "FreeRTOS.h"
#include "task.h"

char Serial_RxPacket1[100];    // 定义接收数据包数组，串口1
char Serial_RxPacket3[100];    // 定义接收数据包数组，串口3
uint8_t Serial_RxFlag1;        // 串口1接收数据包标志位
uint8_t Serial_RxFlag3;        // 串口3接收数据包标志位

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;

void hhSerialSendByte(uint8_t Byte, UART_HandleTypeDef *huart) {
    HAL_StatusTypeDef status = HAL_UART_Transmit(huart, &Byte, 1, HAL_MAX_DELAY);
}

void hhSerialSendArray(uint8_t *Array, uint16_t Length, UART_HandleTypeDef *huart) {
    for (uint16_t i = 0; i < Length; i++) {
        hhSerialSendByte(Array[i], huart);
    }
}

void hhSerialSendString(char *s, UART_HandleTypeDef *huart) {
    for (int i = 0; s[i] != '\0'; i++) {
        hhSerialSendByte(s[i], huart);
    }
}

void hhSerialSendNum(uint32_t Num, uint8_t Length, UART_HandleTypeDef *huart) {
    uint8_t Nums[Length];
    for (uint8_t i = 0; i < Length; i++) {
        Nums[i] = Num % 10;
        Num /= 10;
    }
    for (uint8_t i = Length - 1; i >= 0; i--) {
        hhSerialSendByte(Nums[i] + '0', huart);
    }
}

void s_printf(char *format, ...) {
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    hhSerialSendString(String, &huart3);  // 默认发送到 huart3
}

uint8_t Serial_GetRxFlag1(void) {
    if (Serial_RxFlag1 == 1) {
        Serial_RxFlag1 = 0;
        return 1;
    }
    return 0;
}

uint8_t Serial_GetRxFlag3(void) {
    if (Serial_RxFlag3 == 1) {
        Serial_RxFlag3 = 0;
        return 1;
    }
    return 0;
}

void HC_05_RUN(void) {
    
    //USB转TTL的串口接发数据
    // if (Serial_RxFlag1 == 1) {

    //     // s_printf("get In HC_05_RUN\r\n");
    //     if (strcmp(Serial_RxPacket1, "run") == 0) {
    //         makerobo_run(70, 5000);
    //         s_printf("makerobo_run_lanya\r\n");
            
    //     }
    //     Serial_RxFlag1 = 0;
    // }
    
    //蓝牙的串口接发数据
    if (Serial_RxFlag3 == 1) {
        
        char *x="get char in HC_05_RUN\r\n";
        
        hhSerialSendString(x,&huart3);
        if (strcmp(Serial_RxPacket3, "run") == 0) {
            makerobo_run(70, 5000);
            
        }
        Serial_RxFlag3 = 0;
    }
    else {

    }
}
uint8_t ByteRecv1;
uint8_t ByteRecv3;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  static uint8_t RxState1 = 0;
  static uint8_t pRxPacket1 = 0;
  
  static uint8_t RxState3 = 0;
  static uint8_t pRxPacket3 = 0;

  // USART3 接收
  if (huart==&huart3) {
    
      uint8_t RxData = ByteRecv3;
      
      if (RxState3 == 0) {
          if (RxData == '@' && Serial_RxFlag3 == 0) {
              RxState3 = 1;
              pRxPacket3 = 0;
          }
      } else if (RxState3 == 1) {
          if (RxData == '#') {
              RxState3 = 2;
          } else {
              Serial_RxPacket3[pRxPacket3] = RxData;
              pRxPacket3++;
          }
      } else if (RxState3 == 2) {
          if (RxData == '.') {
              RxState3 = 0;
              Serial_RxPacket3[pRxPacket3] = '\0';
              Serial_RxFlag3 = 1;
          }
      }
    //   if(RxData == 'A' )Serial_RxFlag3 = 1;
    //   hhSerialSendString("A",&huart3);
      HAL_UART_Receive_IT(&huart3, &ByteRecv3, 1);
  }

  // USART1 接收
  if (huart==&huart1) {
      uint8_t RxData = ByteRecv1;
      if (RxState1 == 0) {
          if (RxData == '@' && Serial_RxFlag1 == 0) {
              RxState1 = 1;
              pRxPacket1 = 0;
             
          }
      } else if (RxState1 == 1) {
          if (RxData == '#') {
              RxState1 = 2;
              
          } else {
              Serial_RxPacket1[pRxPacket1] = RxData;              
              pRxPacket1++;
          }
      } else if (RxState1 == 2) {
          if (RxData == '.') {
              RxState1 = 0;
              Serial_RxPacket1[pRxPacket1] = '\0';
              Serial_RxFlag1 = 1;
          }
      }

      HAL_UART_Receive_IT(&huart1, &ByteRecv1, 1);
  }
}
// void HC_05_Receive(void) {
   
// }
void myusart_init(void) {
  HAL_UART_Receive_IT(&huart1, &ByteRecv1, 1);
  HAL_UART_Receive_IT(&huart3, &ByteRecv3, 1);
  }