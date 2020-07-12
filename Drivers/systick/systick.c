#include"stm32f3xx.h"
#include"systick.h"
//#include"gpio.h"
//#include"main.h"
//#include"adc.h"
//#include"input.h"
__IO uint32_t __pid;
__IO uint32_t __ms;
__IO uint32_t timestamp;

extern uint32_t SystemCoreClock;
extern uint32_t SysTick_Config(uint32_t ticks);

void systick_init(void){
  FLASH->KEYR = 0x45670123;
  FLASH->KEYR = 0xCDEF89AB;
//  SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2)); /* set CP10 and CP11 Full Access */
  unsigned long int TimeOut = 10000;

  RCC->CR   |=  RCC_CR_HSEON;
  while((RCC->CR & RCC_CR_HSERDY)==0)
    if(TimeOut) TimeOut--;
  if(TimeOut==0) return;
  RCC->CR   |=  RCC_CR_CSSON;
  RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;

  FLASH->ACR|=  FLASH_ACR_PRFTBE;
  FLASH->ACR&= ~FLASH_ACR_LATENCY;
  FLASH->ACR |= FLASH_ACR_LATENCY_1;

  RCC->CFGR  |= RCC_CFGR_PLLSRC;
  RCC->CR   &= ~RCC_CR_PLLON;
  RCC->CFGR &= ~RCC_CFGR_PLLMUL;
  RCC->CFGR |=  RCC_CFGR_PLLMUL8;
  RCC->CR   |=  RCC_CR_PLLON;
  while((RCC->CR & RCC_CR_PLLRDY)==0) {}

  RCC->CFGR &= ~RCC_CFGR_SW;
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while(!(RCC->CFGR&RCC_CFGR_SWS_PLL));

  RCC->CFGR &= ~RCC_CFGR_PPRE1;
  RCC->CFGR |=  RCC_CFGR_PPRE1_DIV2;

  RCC->CFGR &= ~RCC_CFGR_PPRE2;
  RCC->CFGR |=  RCC_CFGR_PPRE2_DIV2;

  SystemCoreClockUpdate();
  while(SysTick_Config(SystemCoreClock/1000));
}


void Delay_ms(uint32_t nTime)
{
  uint32_t start = __ms;
  while((__ms-start) < nTime);
}

uint32_t TimingDelay_Increment(void)
{

    __ms ++;
  __pid++;
       timestamp++;
  return __ms;
}


void SysTick_Handler(void)
{
  TimingDelay_Increment();


}
