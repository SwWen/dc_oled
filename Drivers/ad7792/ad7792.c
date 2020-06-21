#include "ad7792.h"
#include "spi.h"
#include "stm32f3xx.h"
#include "systick.h"

void ad7792_reset(void) {
  spi_txrx(0xff);
  spi_txrx(0xff);
  spi_txrx(0xff);
  Delay_ms(200);
}

uint8_t ad7792_ID(void) {

  return spi_txrx(0x60);
}

uint8_t ad7792_init(void) {

  ad7792_reset();
  //if(ad7792_ID()!=0x4A){return 0;}
  spi_txrx(AD_CR_CR_W);
//  spi_txrx(0x06);
//  spi_txrx(0x90);CH1
   spi_txrx(0x12);
    spi_txrx(0x91);
  spi_txrx(AD_CR_MR_W);
  spi_txrx(0x00);
  spi_txrx(0x0F);
  return 1;
}

uint16_t ad7792_data_get(void)
{
    uint16_t t = 0;
    while(READ_BIT(GPIOB->IDR,GPIO_IDR_4)){};
   
    spi_txrx(AD_CR_DR_R);
    *((uint8_t *)(&t) + 1) = spi_txrx(0xFF);
    *((uint8_t *)(&t)) = spi_txrx(0xFF);
    return t;
}
