#include "stm32f10x.h"                  // Device header
#include "gp_drive.h"
#include "stm32f10x.h"
#include "uart_drive.h"
#include "str_lib.h"
#include "sim800_drive.h"
#include "main.h"
#include "adc_drive.h"
#include "systick_lib.h"

void init_BAK_reg(void);
void write_BAK_reg(uint8_t addr,uint16_t data);
uint16_t read_BAK_reg(uint8_t addr);
/*
UART manager:
0-count		incrementing each itme we receive msg
1-signal	variable to detreermine wheather msg is ready
2-bridge	vairable to confirm bridge condition or not
3-Terminator should 1:Termianator /0:Interrupt		flag to decide wheather to choose terminaton character or interrupt timer termination for msg
4-terminator char		specified char vales as terminator charactewr
5-time const 		value time to wait after interr
6-time counter 		this value will be decremeted

this should really be done using the  structure but he wants to keep it simple C so no structs here, only arays
so this mgr arrays should beported in the future to using struct variables or bitfields even in struct
*/
unsigned short uart_1_mgr[7]={0,0,0,0,0,0,0};
static char USART_2_msg[250];
//static unsigned short USART_2_cnt=0;
//static unsigned short USART_2_sig=0;
//static unsigned short USART_2_bdg=0;
unsigned short uart_2_mgr[7]={0,0,0,0,0,0,0};

static char USART_3_msg[2000];
//static unsigned short USART_3_cnt=0;
//static unsigned short USART_3_sig=0;
//static unsigned short USART_3_bdg=0;
unsigned short uart_3_mgr[7]={0,0,0,0,0,0,0};

Readouts ReadoutsHolder={0,0,0};

int main(void)
{
	PWR->CR |= PWR_CR_CWUF;			/* Clear Wake-up flag */
	PWR->CR |=PWR_CR_CSBF;		//clear standby flag
	PWR->CSR &= 0xFEFF;		//disnable wakeup pin 
	//PWR->CSR &= ~PWR_CSR_EWUP;
	//NVIC_SystemReset();
	
	RCC->APB2ENR |=0x04;	//enable clock for gpio a input
	GPIOA->CRL &= 0xFFFFFFF0;	//CLEAR a0 state config
	GPIOA->CRL |=0x00000008;	//set as input pullupdown
	__disable_irq();		//diable interrupts
	//here we set up the interrupt:
	AFIO->EXTICR[0]=0x00;		//pa0 as interrupt
	EXTI->IMR |=0x1;	//interrupt enable on line 0
	EXTI->RTSR |=0x1;		//ENABLE rising triger on interrupt line0 
	NVIC_EnableIRQ(EXTI0_IRQn); 		//enables interrupt set above at line 0
	__enable_irq();		//ENABLE interrupts on the core
	
	RCC->APB2ENR |=0x10;	//enable apb2 clock for gpioc output

	//GPIOC->CRH &=0xFF0FFFFF;	//clear settings for c13
//	GPIOC->CRH |=0x00300000;	//c13 as output push pull 50mhz
//	GPIOC->ODR |=0x00002000;	
	init_GP(PC,13,OUT50,O_GP_PP);
	write_GP(PC,13,HIGH);
	GPIOC->CRH &= 0xF0FFFFFF;	//CLEAR_BIT c14 status settings
	GPIOC->CRH |=0x08000000;	//input push pullup pulldown c14
	
	init_GP(PA,8,OUT50,O_GP_PP);
	init_GP(PA,9,OUT50,O_GP_PP);
	write_GP(PA,8,LOW);
	write_GP(PA,9,LOW);
	
	init_BAK_reg();
		if(read_BAK_reg(1) < 4 && read_BAK_reg(1)>0)//check rtc memm
	{
		//write_BAK_reg(1,1);
		if((read_BAK_reg(1)%2)==1)
		{
			write_GP(PA,8,HIGH);	//nieparzysa
		}else{
			write_GP(PA,9,HIGH);	//parzusta
		}
	}else{
		write_BAK_reg(1,1);
	}
	
	
	systick_init();
	systick_inter_start();
	UART_init(2,9600);
	UART_init(3,9600);
	delay_MS(100);		//time delay in order to seet the registers

	//init adc
	RCC->APB2ENR |=RCC_APB2ENR_IOPBEN | RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN;
	init_GP(PB,0,IN,I_AN);
	init_GP(PB,1,IN,I_AN);
	init_GP(PA,1,IN,I_AN);	//thermostor can be plugged for now with photoresisitr
	ADC1->CR2=ADC_CR2_ADON;		//wybudz adc ze sleepa zwieksza to zuzycie pradu
delay_MS(2);	//adc needs 1us of wait to initialize properly
	//delay_MS(100);		//time delay in order to seet the registers
	//	UART_send(2,"MSG SMS SENT");//nucle com6
	//UART_send(2,order_term_buf);//adapter com10
	update_adc_readouts(&ReadoutsHolder);
	sim800_send_sms(3,&ReadoutsHolder);
	while(1)
	{
			if(uart_2_mgr[1]==1)
		{
		//	UART_send(3,USART_2_msg);
			uart_2_mgr[1]=0;
			str_empty(USART_2_msg);		//CLEAR_ the string for sending no errors when shorter msg than previous
		}
		if(uart_3_mgr[1]==1)
		{
		//	UART_send(2,USART_3_msg);
			uart_3_mgr[1]=0;
			str_empty(USART_3_msg);		//CLEAR_BIT the string for sending
		}
		
		//if((GPIOC->IDR & 0x00004000)>>14)
		if(read_GP(PC,14)==1)
		{
			__disable_irq();
			SysTick->CTRL &=0x8;	//we need to disable systick otherwise will be active in sleep!!!!
			__enable_irq();
			GPIOC->ODR |=0x00002000;//off
			write_BAK_reg(1,read_BAK_reg(1)+1);	//on cycles count max value 4 when 5 get back to 1 (startiung val)
			//GPIOC->CRH &= 
			write_GP(PA,8,LOW);
			write_GP(PA,9,LOW);
			SCB->SCR |= SCB_SCR_SLEEPDEEP;
			PWR->CR |= PWR_CR_PDDS;			// standby register
			PWR->CR |= PWR_CR_CWUF;	
			PWR->CSR |= PWR_CSR_EWUP;		//enable wakeup pin 
			PWR->CR |= PWR_CR_LPDS;			//DISABLE internal regulator during sleep time

			__force_stores();
			__WFI();
		}else{
		//	GPIOC->ODR &=0xFFFFDFFF;	//set low so enable c13 led
			write_GP(PC,13,LOW);
			//led_info(read_BAK_reg(1));
						}
		
	}
	
}

void EXTI0_IRQHandler()
{
		EXTI->PR |=0x1;		//delete pending irq
}

void init_BAK_reg(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;		//enable clocks to allow to interface memory, memory worsk withouth that but we wont be able to access it without htis
	PWR->CR |= PWR_CR_DBP; 		//ENABLE access do bak regs and rtc
	
}

void write_BAK_reg(uint8_t addr,uint16_t data)
{
	BKP->DR1=data;
}

uint16_t read_BAK_reg(uint8_t addr)
{
	return (uint16_t)(BKP->DR1);
}

void USART2_IRQHandler()
{
	UART_isr(2,uart_2_mgr,USART_2_msg);
}
void USART3_IRQHandler()
{
	UART_isr(3,uart_3_mgr,USART_3_msg);
}

void SysTick_Handler(void)
{
	//to nie jest najlepsze rozwiazanie bo trzeba tracic miejsce na strukture do usarta1 mimo ze go sie nie uzywa i on sprawdza potem i probuje go odczytywac?
	//odczytu nie ma chyba bo flaga [0] zawsze na 0
	//systick_inter(uart_1_mgr,uart_2_mgr,uart_3_mgr);
}

