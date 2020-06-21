/*
 * ssd1322.c
 *
 *  Created on: 13 июн. 2020 г.
 *      Author: rd3tal
 */

#include "spi.h"
#include "systick.h"
#include "ssd1322.h"
void lcd_WriteCMD(uint8_t dt)
{
	Delay_ms(1);
  PIN_COM;
  spi2_tx(dt);
}

void lcd_WriteData(uint8_t dt)
{

  PIN_DATA;
  spi2_tx(dt);
}


void lcd_init(void)
{
	spi2_init();
	PIN_RES_ON;
	Delay_ms(2);
	PIN_RES_OFF;


  	 lcd_WriteCMD(0xAB); /** 0xAB - внутренний регулятор Vdd, 0x01 - разрешить работу внутреннего регулятора для VDD */
     lcd_WriteData(0x01);

     lcd_WriteCMD(0xFD); /** Set Command Lock */
     lcd_WriteData(0x12); /** разблокировка */

     lcd_WriteCMD(0xB3); // Set Front Clock Divider / Oscillator Frequency
     lcd_WriteData(0xD0); // = reset / 1100b

     lcd_WriteCMD(0xCA); // Set MUX Ratio
     lcd_WriteData(0x3F); // = 63d = 64MUX

     lcd_WriteCMD(0xA2); // Set Display Offset
     lcd_WriteData(0x00); // = RESET

     lcd_WriteCMD(0xA1); // Set Display Start Line
     lcd_WriteData(0x00); // = register 00h

     lcd_WriteCMD(0xA0); // Set Re-map and Dual COM Line mode
     lcd_WriteData(0x14); // 10= Reset except Disable Nibble Re-map, Scan from COM[N-1] to COM0, where N is the Multiplex ratio
     lcd_WriteData(0x11); // 01= Reset except Disable Dual COM mode (MUX = 63)

     lcd_WriteCMD(0xB5); // Set GPIO
     lcd_WriteData(0x00); // = GPIO0, GPIO1 = HiZ, Input Disabled

     lcd_WriteCMD(0xB4); // Display Enhancement A
     lcd_WriteData(0xA0); // = Enable external VSL
     lcd_WriteData(0xB5); // = Normal (reset)

     lcd_WriteCMD(0xC1); // Set Contrast Current
     lcd_WriteData(0x9F); // = 0x7F - default

     lcd_WriteCMD(0xC7); // Master Contrast Current Control
     lcd_WriteData(0x0F);

     lcd_WriteCMD(0xB9); /** Сбрасывает таблицу градаций яркости в состояние по умолчанию (линейная таблица с нарастанием яркости от GS0 до GS1) */

     lcd_WriteCMD(0xB1); // Set Phase Length
     lcd_WriteData(0xE2); // 0xE2= Phase 1 period (reset phase length) = 5 DCLKs, Phase 2 period (first pre-charge phase length) = 14 DCLKs

     lcd_WriteCMD(0xD1); // Display Enhancement B
     lcd_WriteData(0xA2); // 0xA2 = Normal (default); 0x82 = reserved
     lcd_WriteData(0x20); // 0x20 = as-is

     lcd_WriteCMD(0xBB); // Set Pre-charge voltage
     lcd_WriteData(0x1F); // 0x17 = default; 0x1F = 0.60*Vcc (spec example)

     lcd_WriteCMD(0xB6); // Set Second Precharge Period
     lcd_WriteData(0x08); // 0x08 = 8 dclks (default)

     lcd_WriteCMD(0xBE); // Set VCOMH
     lcd_WriteData(0x07); // 0x04 = 0.80*Vcc (default); 0x07 = 0.86*Vcc (spec example)

     lcd_WriteCMD(CMD_SET_DISPLAY_MODE_NORMAL);

     lcd_WriteCMD(CMD_EXIT_PARTIAL_DISPLAY);

     lcd_WriteCMD(CMD_SET_DISPLAY_ON); // Set Display Mode = Normal Display
  }



uint16_t buff[256 * 32]; // 256×64 4BPP OLED
void ssd1322_pset(UG_S16 x, UG_S16 y, UG_COLOR c)
{
uint32_t p;
uint16_t b;
b = (c & 0x0F); // limit the color to 16 tones of gray (4 bits)
if((x >= 256) || (x < 0) || (y >= 64) || (y < 0)) return; // test if the point is withing boundaries (0, 0) to (256, 64)

p = y * 128 + (x >> 1); // point the memory location each line use 128 bytes

switch(x%2) // patch the nibble with the pixel color
{
case 0: buff[p] &= 0x0F; buff[p] |= (b << 4); break;
case 1: buff[p] &= 0xF0; buff[p] |= b; break;
}
}

void update_display( void )
{
   int i;

   lcd_WriteCMD(0x15);         // Set Column Address
   lcd_WriteData(0x1C+0);      //   Default => 0x00
   lcd_WriteData(0x1C+0x3F);   //   Default => 0x77
   lcd_WriteCMD(0x75);         // Set Row Address
   lcd_WriteData(0x00);        //   Default => 0x00
   lcd_WriteData(0x3F);        //   Default => 0x7F

   lcd_WriteCMD(0x5C);
   for(i=0;i<256*32;i++)
   {
	   lcd_WriteData( buff[i] );
   }
}

void update_img(unsigned char* p)
{

   int i;
   lcd_WriteCMD(0x15);         // Set Column Address
   lcd_WriteData(0x1C+0);      //   Default => 0x00
   lcd_WriteData(0x1C+0x3F);   //   Default => 0x77
   lcd_WriteCMD(0x75);         // Set Row Address
   lcd_WriteData(0x00);        //   Default => 0x00
   lcd_WriteData(0x3F);        //   Default => 0x7F
   lcd_WriteCMD(0x5C);
   for(i=0;i<256*32;i++)
   {
      lcd_WriteData( ~(*p++) );
   }
}


