#ifndef AD7792_H
#define AD7792_H
#include "stm32f3xx.h"
void ad7792_reset(void);
uint8_t ad7792_ID(void);
uint16_t ad7792_data_get(void);
uint8_t ad7792_init(void);
#define AD_CR_DR_R 0x58
#define AD_CR_CR_W 0x10
#define AD_CR_MR_W 0x8







#endif //AD7792_H
