#include "adc_drive.h"
#include "main.h"

uint16_t single_readout_ADC1_regu(uint8_t channel)
{
			ADC1->SQR3=channel;	//jako 1 wregular sekwencji konwertuje channel 9 czyliu pb1
	ADC1->CR2 = ADC_CR2_ADON;// zrob 1 konwersje doane wrejestreze beda
	while(!(ADC1->SR & ADC_SR_EOC))
		;	//czekaj dopuki nie zapisze flagi ze odczyt sokjoczony
	  return (uint16_t)(ADC1->DR);
}

void update_adc_readouts(Readouts *data_hldr)
{
	data_hldr->humi_adc=single_readout_ADC1_regu(ADC_MY_HUMI_CHAN);
	data_hldr->light_adc=single_readout_ADC1_regu(ADC_MY_LIGHT_CHAN);
	data_hldr->temp_adc=single_readout_ADC1_regu(ADC_MY_THERM_CHAN);
}
