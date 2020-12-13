#include "uart_drive.h"
#include "gp_drive.h"
#include "systick_lib.h"
#include "str_lib.h"
void UART_init(unsigned short usart,unsigned long BR)
{
	//if using USArt1 clk speed is 7Mmhz, else for susart2/3 only 36Mhz (apb1 clk slower)
	
	uint16_t BRR_Cal=USART_BRR(usart,BR);
	
		RCC->APB2ENR |=0x1;			//enable alternate function for ports tx1 tx2 to sa piny na zegarze jeszcze apb2
	if(usart==1)
	{//Usart1: (tx) pa9 (rx) pa10
		__disable_irq();
		RCC->APB2ENR |=0x4000;
		init_GP(PA,9,OUT50,O_AF_PP); //tx
		init_GP(PA,10,IN,I_PP); //rx
		USART1->BRR=BRR_Cal;
		USART1->CR1 |=0x8;
		USART1->CR1 |=0x4;
		USART1->CR1 |=0x2000;
		USART1->CR1 |=0x20;	//ENABLE rx interrupt
		NVIC_EnableIRQ(USART1_IRQn);
		__enable_irq();
	}else if(usart==2)
	{//init usart 2  (tx) pa2 (rx) pa3
		__disable_irq();
		RCC->APB1ENR |=0x20000;		//enable uart2:
		init_GP(PA,2,OUT50,O_AF_PP); //tx				//setup pins:
		init_GP(PA,3,IN,I_PP); //rx
		USART2->BRR=BRR_Cal;				//setup baud rate for 9600bps
		USART2->CR1 |=0x8;		//enable uart transmit	
		USART2->CR1 |=0x4;		//enable uart receive
		USART2->CR1 |=0x2000;				//enable uart:
		USART2->CR1 |=0x20;	//ENABLE rx interrupt
		NVIC_EnableIRQ(USART2_IRQn);
		__enable_irq();
	}else if(usart==3)
	{//Usart3: (tx) pb10 (rx) pb11
		__disable_irq();
		RCC->APB1ENR |=0x40000;
		init_GP(PB,10,OUT50,O_AF_PP); //tx
		init_GP(PB,11,IN,I_PP); //rx
		USART3->BRR=BRR_Cal;
		USART3->CR1 |=0x8;
		USART3->CR1 |=0x4;
		USART3->CR1 |=0x2000;
		USART3->CR1 |=0x20;	//ENABLE rx interrupt
		NVIC_EnableIRQ(USART3_IRQn);
		__enable_irq();
	}

	
}

uint16_t USART_BRR(unsigned short usart,unsigned long BR)
{
	unsigned long div=36000000;
	unsigned long dec;
	double frac=36000000.00;
	double frac2=1.00;
	unsigned long final;
	
	if(usart==1)
	{
		div=72000000;
		frac=72000000.00;
	}
	div=div/(BR*16);	//this will give us the integer value of the integer
	frac=16*((frac/(BR*16))-div);	//this will give us fractional part
	//those numbers separated are needed for input for BRR register
	dec=(unsigned long)frac;	//cutoff the fraction number
	frac2=100*(frac-dec);		//convert to decimal to add to register input	skoki beda co 0.125 bo 16 bitow to 1/8 wiec co tyle ulamki beda leciec
	if(frac2>50)	//rounding up, bigger than 0.5 fraction
	{
		dec++;
		if(dec==16)	//overflow of fraction register part, carry overflow przy 16 bo tylko 16 wartosci mozemy zakodowac na 4bit, wiec to beda dosc duze skoki wartosci miedzy kolejnymi miejscami
		{
			dec=0;	//fraction part zero
			div++;	//carry to integer part
		}	
	}
	final=(div<<4);
	final+=dec;
return (uint16_t)final;
}

char UART_rx(unsigned short uart)
{
	char c=0;
	if(uart==1)
	{
			while((USART1->SR & 0x20) == 0x00);
			c=(char)(USART1->DR);
		
	}else if(uart==2){
			while((USART2->SR & 0x20) == 0x00);
			c=(char)(USART2->DR);
		
	}else if(uart==3)
	{
			while((USART3->SR & 0x20) == 0x00);
			c=(char)(USART3->DR);
	}
	return c;
}

void UART_tx(unsigned short uart,char c)
{

	if(uart==1)
	{
		while((USART1->SR & (1<<6))==0x00);
		USART1->DR = c;
		
	}else if(uart==2){
		while((USART2->SR & (1<<6))==0x00);
		USART2->DR = c;
		
	}else if(uart==3)
	{
		while((USART3->SR & (1<<6))==0x00);
		USART3->DR = c;
	}
}
/*
1-define my uart
2-is it a bridge or process or both
3-if proces: string tu fullfill, count,signal_ready
*/
void UART_isr(unsigned short uart,unsigned short uart_mgr[], char str[])
{
	if(uart_mgr[2]==0)	//we are not sending data immediatelly, we neeed to process it
	{
		str[uart_mgr[0]]=UART_rx(uart);	//collect the data for processsing into a prepared string
		if(uart_mgr[3])	//use terminator strategy
		{
				if(str[uart_mgr[0]]==uart_mgr[4])	//if the last char received
					{
						uart_mgr[0]=0;		//zero in
						uart_mgr[1]=1;	//finished processing received data into string raise flag
					}else{
						uart_mgr[0]++;	//receiving another char
					}
		}else{
			//use timer stategy
			uart_mgr[0]++; //increase char read count
			uart_mgr[6]=uart_mgr[5]; //value passed to 6 6 will be decremented
			systick_inter_start();
		}
	}
	else{
		//if we are brigding communication only
		UART_tx(uart_mgr[2],UART_rx(uart));		//bridge is the usart to which redirect the transmission
	}
}
//send the whole string
void UART_send(unsigned short uart_sel,char str[])
{
	int i=0;
	while(str[i] != '\0')
	{
		UART_tx(uart_sel,str[i]);
		i++;
	}
}


//fcn for sending msg to the user and keep ewaiting
void UART_msg(unsigned short uart, char str[],unsigned short str_mgr[])
{
	unsigned long timeout=7200000;	//if timeout reached
	UART_send(uart,str);
	while(str_mgr[1]==0 & timeout != 0)
	{/*we just wait*/
	timeout--;
	}
	str_mgr[1]=0;
}


