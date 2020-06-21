#include "stm32f3xx.h"
#include "systick.h"
#include "spi.h"
#include "fft.h"
#include "ad7792.h"
#include "adc.h"
#include "ugui.h"
#include "stdio.h"
#include "ssd1322.h"
#include "screen.h"
char buf[200];


int32_t i;


int main(void){

systick_init();
spi_init();
ad7792_init();
lcd_init();
gui_init();
	while(1){
screen();
		update_display();
	}
return 0;
}
