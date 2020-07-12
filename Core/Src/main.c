#include "stm32f3xx.h"
#include "systick.h"
#include "spi.h"
#include "ad7792.h"
#include "ssd1322.h"
#include "screen.h"
#include "adc.h"
float32_t i;
int main(void){
systick_init();
spi_init();
ad7792_init();
adc_init();
lcd_init();
gui_init();

	while(1){
	i=vbat();
screen();
		update_display();
	}
return 0;
}
