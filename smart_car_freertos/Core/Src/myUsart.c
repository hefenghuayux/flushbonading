#include "myUsart.h"


char Serial_RxPacket1[100]; // 定义接收数据包数组，串口1
char Serial_RxPacket3[100]; // 定义接收数据包数组，串口3
uint8_t Serial_RxFlag1;     // 串口1接收数据包标志位
uint8_t Serial_RxFlag3;     // 串口3接收数据包标志位

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;

void hhSerialSendByte(uint8_t Byte, UART_HandleTypeDef *huart) {
  HAL_StatusTypeDef status = HAL_UART_Transmit(huart, &Byte, 1, HAL_MAX_DELAY);
}

void hhSerialSendArray(uint8_t *Array, uint16_t Length,
                       UART_HandleTypeDef *huart) {
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
  hhSerialSendString(String, &huart3); // 默认发送到 huart3
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
cJSON *cJsonData = NULL;
cJSON *cJsonVlaue = NULL;
float p = 0.0, i = 0.0, d = 0.0, a = 0.0;
void HC_05_RUN(void) {
  // USB转TTL的串口接发数据
  //  if (Serial_RxFlag1 == 1) {

  //     // s_printf("get In HC_05_RUN\r\n");
  //     if (strcmp(Serial_RxPacket1, "run") == 0) {
  //         makerobo_run(70, 5000);
  //         s_printf("makerobo_run_lanya\r\n");

  //     }
  //     Serial_RxFlag1 = 0;
  // }

  // 蓝牙的串口接发数据

  if (Serial_RxFlag3 == 1) {

    
    cJsonData = cJSON_Parse((const char *)Serial_RxPacket3);
   
    if (cJsonData == NULL) {
      // 解析失败，获取错误位置
      const char *error_ptr = cJSON_GetErrorPtr();
      if (error_ptr != NULL) {
          s_printf("JSON解析失败,错误位置:%s\r\n", error_ptr);
      } else {
          s_printf("未能获取错误位置\r\n");
      }
      
  }
    if (cJsonData != NULL&&cJSON_GetObjectItem(cJsonData, "p") != NULL)

    {
      cJsonVlaue = cJSON_GetObjectItem(cJsonData, "p");
      p = cJsonVlaue->valuedouble;
    }
    else s_printf("没有p\r\n");


    if (cJsonData != NULL&&cJSON_GetObjectItem(cJsonData, "i") != NULL)

    {
      cJsonVlaue = cJSON_GetObjectItem(cJsonData, "i");
      i = cJsonVlaue->valuedouble;
    }
    else s_printf("没有i\r\n");

    if (cJsonData != NULL&&cJSON_GetObjectItem(cJsonData, "d") != NULL)

    {
      cJsonVlaue = cJSON_GetObjectItem(cJsonData, "d");
      d = cJsonVlaue->valuedouble;
    }
    else s_printf("没有d\r\n");
    if (cJsonData != NULL&&cJSON_GetObjectItem(cJsonData, "a") != NULL)

    {
      cJsonVlaue = cJSON_GetObjectItem(cJsonData, "a");
      a = cJsonVlaue->valuedouble;
    }
    else s_printf("没有a\r\n");

    if (cJsonData != NULL) {
      cJSON_Delete(cJsonData); // 释放空间、但是不能删除cJsonvlaue不然会 出现异常错误
    }

    s_printf("P:%.1f\r\n", p);
    s_printf("I:%.1f\r\n", i);
    s_printf("D:%.1f\r\n", d);
    s_printf("A:%.1f\r\n", a);
    // char *x = "get char in HC_05_RUN\r\n";

    hhSerialSendString(Serial_RxPacket3, &huart3);

    if (strcmp(Serial_RxPacket3, "run") == 0) {
      makerobo_run(70, 5000);
    }
    Serial_RxFlag3 = 0;
  } 
}

uint8_t ByteRecv1;
uint8_t ByteRecv3;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  static uint8_t RxState1 = 0;
  static uint8_t pRxPacket1 = 0;

  static uint8_t RxState3 = 0;
  static uint8_t pRxPacket3 = 0;

  // USART3 接收
  if (huart == &huart3) {

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
  if (huart == &huart1) {
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

void myusart_init(void) {
  HAL_UART_Receive_IT(&huart1, &ByteRecv1, 1);
  HAL_UART_Receive_IT(&huart3, &ByteRecv3, 1);
}