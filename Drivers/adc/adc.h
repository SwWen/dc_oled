/*
 * ac.h
 *
 *  Created on: 28 мая 2020 г.
 *      Author: rd3tal
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_
#define  DMA_BUFF_SIZE 2
#include "fft.h"
int16_t adc_value(void);
void opamp_init(void);
void adc_init(void);
float32_t vbat(void);
int16_t  adc_buff[DMA_BUFF_SIZE];
#endif /* ADC_ADC_H_ */
