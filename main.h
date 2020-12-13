#ifndef MAIN_H
#define MAIN_H

#include "stm32f10x.h"                  // Device header

#define ADC_MY_LIGHT_CHAN 9				//fotoresitoir
#define ADC_MY_THERM_CHAN 1				//themroistor
#define ADC_MY_HUMI_CHAN 8					//humidity sensor capacitive

struct Readouts_struct{
	uint16_t humi_adc;
	uint16_t light_adc;
	uint16_t temp_adc;
};

typedef struct Readouts_struct Readouts;

extern Readouts ReadoutsHolder;

#endif
