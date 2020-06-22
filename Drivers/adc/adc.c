/*
 * adc.c
 *
 *  Created on: 28 мая 2020 г.
 *      Author: rd3tal
 */

#include"systick.h"
#include"adc.h"

#define x0 0x7ff
#define x16 140
void tim6_adc_init(void);
void dma_init(void);
void adc_init(void)
{
   RCC->AHBENR|=RCC_AHBENR_GPIOAEN;
   GPIOA->MODER|=(GPIO_MODER_MODER0|GPIO_MODER_MODER1|GPIO_MODER_MODER2|GPIO_MODER_MODER3|GPIO_MODER_MODER4);
  RCC->CFGR2 |=RCC_CFGR2_ADCPRE12_DIV1;
  Delay_ms(1);
  RCC->AHBENR|=RCC_AHBENR_ADC12EN;
  ADC1->CR &= ~(uint32_t)0xFF;
  ADC1->CR &= ~ADC_CR_ADVREGEN;
  ADC1->CR |= ADC_CR_ADVREGEN_1;
  Delay_ms(1);
//  ADC2->CR &= ~(uint32_t)0xFF;
//  ADC2->CR &= ~ADC_CR_ADVREGEN;
//  ADC2->CR |= ADC_CR_ADVREGEN_1;
//  Delay_ms(1);
 // ADC1->DIFSEL |= ADC_DIFSEL_DIFSEL_3;
if((ADC1->CR&ADC_CR_ADEN)==0)
{
//	ADC1->CR |= ADC_CR_ADCALDIF;
  ADC1->CR |= ADC_CR_ADCAL;
  while((ADC1->CR & ADC_CR_ADCAL));
}
  ADC1->CR |= ADC_CR_ADEN;
  while(!(ADC1->ISR & ADC_ISR_ADRDY));

//  if((ADC2->CR&ADC_CR_ADEN)==0)
//  {
//    ADC2->CR |= ADC_CR_ADCAL;
//    while((ADC2->CR & ADC_CR_ADCAL));
//  }
//    ADC2->CR |= ADC_CR_ADEN;
//    while(!(ADC2->ISR & ADC_ISR_ADRDY));


  ADC1->SQR1 &=~ (ADC_SQR1_L_Msk);
 ADC1->SQR1 |= (ADC_SQR1_SQ1_0|ADC_SQR1_SQ1_1);
 ADC1->SMPR1|=ADC_SMPR1_SMP3;
 ADC1->CFGR |=(ADC_CFGR_DMACFG|ADC_CFGR_DMAEN|ADC_CFGR_EXTEN_0|ADC_CFGR_EXTSEL_0 \
		 |ADC_CFGR_EXTSEL_2|ADC_CFGR_EXTSEL_3);
 ADC1->CR |= ADC_CR_ADSTART;
  RCC->APB1ENR|=RCC_APB1ENR_DAC1EN;
  DAC->CR |= DAC_CR_BOFF1|DAC_CR_EN1;
  DAC->DHR12R1=450;
 // dma_init();
}

void dma_init(void){
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  DMA1_Channel1->CPAR = (uint32_t) &ADC1->DR;
  DMA1_Channel1->CMAR = (uint32_t)adc_buff;
  DMA1_Channel1->CCR &= ~DMA_CCR_DIR;
  DMA1_Channel1->CNDTR = FFT_SIZE;
  DMA1_Channel1->CCR &= ~DMA_CCR_PINC;
  DMA1_Channel1->CCR |= DMA_CCR_MINC;
  DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;
  DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;
  DMA1_Channel1->CCR |= DMA_CCR_PL;
  DMA1_Channel1->CCR |= DMA_CCR_CIRC|DMA_CCR_TCIE;
  DMA1_Channel1->CCR |= DMA_CCR_EN;
NVIC_EnableIRQ(DMA1_Channel1_IRQn);
tim6_adc_init();

}
void tim6_adc_init(void)
{
   //разрешаем тактирование таймера
   RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
   TIM6->PSC = 64 - 1;//10KHz or 0.0001s
   TIM6->ARR  = 200;
   //разрешаем генерацию TRGO
   TIM6->CR2 |= TIM_CR2_MMS_1;
   //включаем таймер
   TIM6->CR1 |= TIM_CR1_CEN;
}


void DMA1_Channel1_IRQHandler(void){
	TIM6->CR1 &= ~TIM_CR1_CEN;
DMA1->IFCR|=DMA_IFCR_CTCIF1;
TIM6->CR1 |= TIM_CR1_CEN;
}


int16_t adc1_value,adc2_value;
int16_t adc3_value;
int16_t adc_value(void){
uint64_t adcB=0;

//    adc_value =(adc_buff[0])*0.12-100;
	for (uint16_t i = 0; i < FFT_SIZE;++i) {
	adcB+=adc_buff[i];
	}
	adcB=adcB/FFT_SIZE;
    adc1_value=(adcB)-2047-48;
    static long Adc0, AdcF;

    Adc0 = (long)adc1_value << 16;
    AdcF += (Adc0 - AdcF)*0.1; // или разделить на нужное число, если оно не степень двойки
    int Result = AdcF >> 16;

    return Result;

}

void opamp_init(void){
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    OPAMP1->CSR |=(OPAMP_CSR_VMSEL_1|OPAMP_CSR_VPSEL |OPAMP_CSR_PGGAIN_1|OPAMP_CSR_PGGAIN_0| OPAMP_CSR_OPAMPxEN) ;

 //   OPAMP2->CSR |=(OPAMP_CSR_VMSEL_1|OPAMP_CSR_VPSEL | OPAMP_CSR_OPAMPxEN|OPAMP_CSR_PGGAIN_1|OPAMP_CSR_PGGAIN_0) ;



}

