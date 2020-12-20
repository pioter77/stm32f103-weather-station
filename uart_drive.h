#ifndef UART_DRIVE_H
#define UART_DRIVE_H

#include "stm32f10x.h"                  // Device header

extern unsigned short uart_1_mgr[7];
extern unsigned short uart_2_mgr[7];
extern unsigned short uart_3_mgr[7];

void UART_init(unsigned short usart,unsigned long BR);
uint16_t USART_BRR(unsigned short usart,unsigned long BR);
char UART_rx(unsigned short uart);
void UART_tx(unsigned short uart,char c);
void UART_isr(unsigned short uart,unsigned short uart_mgr[], char str[]);
void UART_send(unsigned short uart_sel,char str[]);
void UART_msg(unsigned short uart, char str[],unsigned short str_mgr[]);
#endif
