#ifndef __MYUSART_H
#define __MYUSART_H



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