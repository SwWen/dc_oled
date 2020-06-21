#include "spi.h"
#include "stm32f3xx.h"
#include "systick.h"
#include "ugui.h"
#define SPI1_DR_8bit         *(__IO uint8_t*)&(SPI1->DR)
#define SPI2_DR_8bit         *(__IO uint8_t*)&(SPI2->DR)
void spi_init(void) {
//Включаем тактирование порта B 
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	//распиновка
	//PB3 - SCL  -  Alternative func. push-pull - OUT
	//PB4 - MISO -  Alternative func. push-pull - OUT  - IN
	//PB5 - MOSI -  Alternative func.  push-pull - OUT

	//3, 4, 5 вывод - альтернативная функция,
	GPIOB->MODER |= GPIO_MODER_MODER3_1 | GPIO_MODER_MODER4_1
			| GPIO_MODER_MODER5_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR5;
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR3 | GPIO_PUPDR_PUPDR5);

	//назначаем выводам необходимые альтернативные ф-ции
	GPIOB->AFR[0] |= (0x05 << 3 * 4);
	GPIOB->AFR[0] |= (0x05 << 4 * 4);
	GPIOB->AFR[0] |= (0x05 << 5 * 4);
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR4_0;

	//конфигурируем SPI2 
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;     //Enable oscil SPI2
	SPI1->CR1 |=SPI_CR1_BR_1;               //Baud rate = Fpclk/256
	SPI1->CR1 |= SPI_CR1_CPOL;              //Polarity cls signal CPOL = 0;
	SPI1->CR1 |= SPI_CR1_CPHA;              //Phase cls signal    CPHA = 0;
	SPI1->CR1 &= ~SPI_CR1_LSBFIRST;         //MSB will be first
	SPI1->CR1 |= SPI_CR1_SSM;               //Program mode NSS
	SPI1->CR1 |= SPI_CR1_SSI;               //анналогично состоянию, когда NSS 1
	SPI1->CR1 |= SPI_CR1_MSTR;              //Mode Master

	SPI1->CR2 |= SPI_CR2_FRXTH;            //RXNE 8 bit
	SPI1->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;  		//Data_Size
	SPI1->CR1 |= SPI_CR1_SPE;               //Enable SPI2
}

uint8_t spi_txrx(uint8_t data) {
	SPI1_DR_8bit = data;
	while ((SPI1->SR & SPI_SR_RXNE) == 0);
	return SPI1_DR_8bit;
}

void spi2_init(void) {
	GPIOB->MODER |= GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_1
			| GPIO_MODER_MODER15_1;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13 | GPIO_OSPEEDER_OSPEEDR15;
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR13 | GPIO_PUPDR_PUPDR15);

	//назначаем выводам необходимые альтернативные ф-ции
	GPIOB->AFR[1] |= (0x05 << 5 * 4);
//	GPIOB->AFR[1] |= (0x05 << 6 * 4);
	GPIOB->AFR[1] |= (0x05 << 7 * 4);

	// 12 вывод - выход с подтяжкой к питанию
	GPIOB->MODER |= GPIO_MODER_MODER12_0;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;
	GPIOB->MODER |= GPIO_MODER_MODER11_0;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;
	//конфигурируем SPI2
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;     //Enable oscil SPI2
	  SPI2->CR1 &= ~SPI_CR1_BR;              //Baud rate = Fpclk/256
	  SPI2->CR1 |= SPI_CR1_CPOL;              //Polarity cls signal CPOL = 0;
	  SPI2->CR1 |= SPI_CR1_CPHA;              //Phase cls signal    CPHA = 0;
	  SPI2->CR1 &= ~SPI_CR1_LSBFIRST;         //MSB will be first
	  SPI2->CR1 |= SPI_CR1_SSM;               //Program mode NSS
	  SPI2->CR1 |= SPI_CR1_SSI;               //анналогично состоянию, когда NSS 1
	  SPI2->CR1 |= SPI_CR1_MSTR;              //Mode Master
	  SPI2->CR1 |=SPI_CR1_BIDIMODE;
	  SPI2->CR1 |=SPI_CR1_BIDIOE;
	  SPI2->CR2 |= SPI_CR2_FRXTH;            //RXNE 8 bit
	  SPI2->CR2 |= SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2;  		//Data_Size
	  SPI2->CR1 |= SPI_CR1_SPE;               //Enable SPI2
}

void spi2_tx(uint8_t data) {

SPI2_DR_8bit = data;
while ((SPI2->SR & SPI_SR_BSY));
}




