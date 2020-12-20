#ifndef SIM800_DRIVE_H
#define SIM800_DRIVE_H

#include "stm32f10x.h"                  // Device header
#include "main.h"

void sim800_send_sms(uint8_t uart_no,Readouts *data_hldr);

#endif
