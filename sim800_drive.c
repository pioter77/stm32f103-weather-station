#include "sim800_drive.h"
#include "uart_drive.h"
#include "systick_lib.h"
#include "main.h"
#include "str_lib.h"

static char order_term_buf[]={'\r','\n','\0'};
static char sms_term_buf[]={0x1A,'\r','\n','\0'};
//static char new_lne[]={'\n','\0'};

void sim800_send_sms(uint8_t uart_no,Readouts *data_hldr)	//dodaj jeszcze wskaznik na typedef struct do przechowywania tych wszystkic parametrow co poleca po sms czyli odczytow
{//fajnie by byo numer tez jak oargument miec
	char buf_tmpe[5]={[4]='\0'};
	char buf_humi[5]={[4]='\0'};
	char buf_ligh[5]={[4]='\0'};
	char buf_batt[5]={[4]='\0'};
	char buf2_tmpe[10]="temp:";
	char buf2_humi[15]="soil humi:";
	char buf2_ligh[12]="light:";
	char buf2_batt[12]="battV:";

	int2str(data_hldr->temp_adc,buf_tmpe);		//to format (in chars) 4096\0
	int2str(data_hldr->light_adc,buf_ligh);
	int2str(data_hldr->humi_adc,buf_humi);
	int2str(data_hldr->batt_adc,buf_batt);
	
	str_concat(buf_tmpe,buf2_tmpe);		//to format tmep:4095
	str_concat(buf_humi,buf2_humi);
	str_concat(buf_ligh,buf2_ligh);
	str_concat(buf_batt,buf2_batt);
	
		UART_send(uart_no,"AT+CMGF=1");//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
	delay_MS(100);		//time delay in order to seet the registers
	UART_send(uart_no,"AT+CMGS=\"+48731331644\"");//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
	delay_MS(100);		//time delay in order to seet the registers
	UART_send(uart_no,buf2_tmpe);//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
	UART_send(uart_no,buf2_humi);//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
	UART_send(uart_no,buf2_ligh);//adapter com10
	//	UART_send(uart_no,new_lne);//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
	UART_send(uart_no,buf2_batt);//adapter com10
	UART_send(uart_no,order_term_buf);//adapter com10
	delay_MS(100);
	UART_send(uart_no,sms_term_buf);//adapter com10
}
