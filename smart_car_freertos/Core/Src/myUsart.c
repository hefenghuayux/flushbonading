                // Device header
#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"
#include "stdarg.h"
#include "myUsart.h"
#include <stm32f1xx_hal_usart.h>
#include "stm32f1xx_it.h"
#include "stm32f1xx_hal.h"
#include <string.h>
char Serial_RxPacket[100];				//定义接收数据包数组，数据包格式"@MSG\r\n"
uint8_t Serial_RxFlag;					//定义接收数据包标志位
UART_HandleTypeDef huart3;
void hhSerialSendByte(uint8_t Byte){

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart3, &Byte, 1, HAL_MAX_DELAY);
    
    }
void hhSerialSendArray(uint8_t * Array,uint16_t Length){
    for(uint16_t i=0;i<Length;i++){
      hhSerialSendByte(Array[i]);
    }
  }
  void hhSerialSendString(char* s){
    for(int i=0;s[i]!='\0';i++){
      hhSerialSendByte(s[i]);
    }
  }
  void hhSerialSendNum(uint32_t Num,uint8_t Length){
    uint8_t Nums[Length];
    for(uint8_t i=0;i<Length;i++){
      Nums[i]=Num%10;
      Num/=10;
    }
    for(uint8_t i=Length-1;i>=0;i--){
      hhSerialSendByte(Nums[i]+'0');
    }
  
  
  }
  void s_printf(char * format,...){
    char String[100];
    va_list arg;
    va_start(arg,format);
    vsprintf(String,format,arg);
    va_end(arg);
    hhSerialSendString(String);
  }
  
  uint8_t Serial_GetRxFlag(void){
    if(Serial_RxFlag==1){
      Serial_RxFlag=0;
      return 1;
  
    }
    return 0;
  }
  void HC_05_RUN(void){
    // if (Serial_RxFlag == 1)		//如果接收到数据包
    // {
      
    //   /*将收到的数据包与预设的指令对比，以此决定将要执行的操作*/
    //   if (strcmp(Serial_RxPacket, "LED_ON") == 0)			//如果收到LED_ON指令
    //   {

    //     s_printf("LED_ON_OK\r\n");				//串口回传一个字符串LED_ON_OK
    //   }
    //   else if (strcmp(Serial_RxPacket, "LED_OFF") == 0)	//如果收到LED_OFF指令
    //   {

    //     s_printf("LED_OFF_OK\r\n");			//串口回传一个字符串LED_OFF_OK
    //   }
    //   else						//上述所有条件均不满足，即收到了未知指令
    //   {
    //     s_printf("ERROR_COMMAND\r\n");			//串口回传一个字符串ERROR_COMMAND
      
    //   }
      
    //   Serial_RxFlag = 0;			//处理完成后，需要将接收数据包标志位清零，否则将无法接收后续数据包
    // }
  }
  void HC_05_Receive(void){
    // static uint8_t RxState = 0;        // 定义表示当前状态机状态的静态变量
    // static uint8_t pRxPacket = 0;      // 定义表示当前接收数据位置的静态变量
  
    // if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)    // 判断是否是USART3的接收事件触发的中断
    // {
    //     uint8_t RxData = USART_ReceiveData(USART3);          // 读取数据寄存器，存放在接收的数据变量
  
    //     /* 使用状态机的思路，依次处理数据包的不同部分 */
        
    //     /* 当前状态为0，接收数据包包头 */
    //     if (RxState == 0)
    //     {
    //         if (RxData == '@' && Serial_RxFlag == 0)         // 如果数据确实是包头，并且上一个数据包已处理完毕
    //         {
    //             RxState = 1;            // 置下一个状态
    //             pRxPacket = 0;          // 数据包的位置归零
    //         }
    //     }
    //     /* 当前状态为1，接收数据包数据，同时判断是否接收到了第一个包尾 */
    //     else if (RxState == 1)
    //     {
    //         if (RxData == '\r')            // 如果收到第一个包尾
    //         {
    //             RxState = 2;              // 置下一个状态
    //         }
    //         else                            // 接收到了正常的数据
    //         {
    //             Serial_RxPacket[pRxPacket] = RxData;    // 将数据存入数据包数组的指定位置
    //             pRxPacket++;              // 数据包的位置自增
    //         }
    //     }
    //     /* 当前状态为2，接收数据包第二个包尾 */
    //     else if (RxState == 2)
    //     {
    //         if (RxData == '\n')            // 如果收到第二个包尾
    //         {
    //             RxState = 0;              // 状态归0
    //             Serial_RxPacket[pRxPacket] = '\0';        // 将收到的字符数据包添加一个字符串结束标志
    //             Serial_RxFlag = 1;        // 接收数据包标志位置1，成功接收一个数据包
    //         }
    //     }
  
    //     USART_ClearITPendingBit(USART3, USART_IT_RXNE);      // 清除标志位
    // }
  }