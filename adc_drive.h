#ifndef ADC_DRIVE_H
#define ADC_DRIVE_H

#include "stm32f10x.h"                  // Device header
#include "main.h"

uint16_t single_readout_ADC1_regu(uint8_t channel);
void update_adc_readouts(Readouts *data_hldr);

#endif
