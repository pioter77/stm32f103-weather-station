#include "sim800_drive.h"
#include "uart_drive.h"
#include "systick_lib.h"

static char order_term_buf[]={'\r','\n','\0'};
static char sms_term_buf[]={0x1A,'\r','\n','\0'};
//static char new_lne[]={'\n','\0'};

void sim800_send_sms(uint8_t uart_no)	//dodaj jeszcze wskaznik na typedef struct do przechowywania tych wszystkic parametrow co poleca po sms czyli odczytow
{//fajnie by byo numer tez jak oargument miec
		UART_send(uart_no,"AT+CMGF=1");//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
	delay_MS(100);		//time delay in order to seet the registers
	UART_send(uart_no,"AT+CMGS=\"+\"");//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
	delay_MS(100);		//time delay in order to seet the registers
	UART_send(uart_no,"val1:100%");//adapter com10
	//UART_send(uart_no,new_lne);//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
		UART_send(uart_no,"val2:5.09%");//adapter com10
	//	UART_send(uart_no,new_lne);//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
		UART_send(uart_no,"val3:2076");//adapter com10
	//	UART_send(uart_no,new_lne);//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
	delay_MS(100);
	UART_send(uart_no,sms_term_buf);//adapter com10
}
