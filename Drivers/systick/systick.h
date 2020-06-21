#ifndef SYSTICK_H
#define SYSTICK_H
#include"stm32f3xx.h"
//#include"main.h"
void systick_init(void);
void Delay_ms(uint32_t nTime);
extern __IO uint32_t __ms;
extern __IO uint32_t __pid;
#endif // SYSTICK_H