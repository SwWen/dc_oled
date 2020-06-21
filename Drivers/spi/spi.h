#ifndef SPI_H
#define SPI_H
#include "stm32f3xx.h"
void spi_init(void);
uint8_t spi_txrx(uint8_t data);
void spi2_tx(uint8_t data);
void spi2_init(void);







#endif //SPI_H
