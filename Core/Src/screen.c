/*
 * screen.c
 *
 *  Created on: 21 июн. 2020 г.
 *      Author: rd3tal
 */
#include "ugui.h"
#include "fft.h"
#include "ssd1322.h"
#include "stdio.h"
	UG_GUI gui;
	data dt;
	char buf[100];


void gui_init(void){
	UG_Init(&gui, ssd1322_pset,256,64);
	UG_FontSelect(&FONT_10X16) ;
	UG_SetBackcolor(C_BLACK);
	UG_SetForecolor(C_WHITE);
	UG_FillScreen(C_BLACK);
}
void spectum(const data *dt) {
	for (uint16_t i = 0; i < FFT_SIZE / 2; ++i) {
		float32_t div=0;
		div = dt->max / 44;
		UG_DrawLine(i+230, 64 - (dt->fft_Dbuff[i] / div), i+230, 64, C_WHITE);
	}
}


void main_screen(const data *dt){
static uint8_t mod;

	UG_FillScreen(C_BLACK);



//	sprintf(buf, "F=%.2f", dt.freq);
//	UG_PutString(120, 36, buf);
	UG_FontSelect(&FONT_22X36);
	if(dt->index==0){
		mod=0;
	if((dt->dc<-1000)||(dt->dc)>1000){
						sprintf(buf, "%.1f", dt->dc);
					}
					else if((dt->dc<-100)||(dt->dc>100)){
					sprintf(buf, "%.2f", dt->dc);}
					else{
					sprintf(buf, "%.3f", dt->dc);
					}
//		sprintf(buf, "%.2f", dt->dc);



	}else if(dt->Trms>10){
		mod=1;

		if(dt->Trms<10){
					sprintf(buf, "%.4f", dt->Trms);
				}
				else if((dt->Trms>10)&&(dt->Trms<100)){
				sprintf(buf, "%.3f", dt->Trms);}
				else{
				sprintf(buf, "%.2f", dt->Trms);
				}
	}else{
		mod=2;
		if(dt->Vrms<10){
					sprintf(buf, "%.4f", dt->Vrms);
				}
				else if((dt->Vrms>10)&&(dt->Vrms<100)){
				sprintf(buf, "%.3f", dt->Vrms);}
				else{
				sprintf(buf, "%.2f", dt->Vrms);
				}
	}

	UG_PutString(0, 5, buf);


	UG_FontSelect(&FONT_10X16);
	sprintf(buf, "A%d", dt->A);
	UG_PutString(220, 0, buf);

	UG_FontSelect(&FONT_6X10);
	sprintf(buf, "\n%.2f", dt->Trms);
	UG_PutString(0, 44, buf);
	UG_FontSelect(&FONT_8X14);
	sprintf(buf, "Trms");
	if(mod==1){
	UG_SetForecolor(C_BLACK);
	UG_SetBackcolor(C_WHITE);
	}
	UG_PutString(0, 40, buf);
	UG_SetForecolor(C_WHITE);
	UG_SetBackcolor(C_BLACK);
//***********************************************
	UG_FontSelect(&FONT_6X10);
	sprintf(buf, "\n%.2f", dt->Vrms);
	UG_PutString(50, 44, buf);
	UG_FontSelect(&FONT_8X14);
	sprintf(buf, "Vac");
		if(mod==2){
		UG_SetForecolor(C_BLACK);
		UG_SetBackcolor(C_WHITE);
		}
		UG_PutString(50, 40, buf);
		UG_SetForecolor(C_WHITE);
		UG_SetBackcolor(C_BLACK);



	UG_FontSelect(&FONT_6X10);
	sprintf(buf, "\n%.2f", dt->dc);
	UG_PutString(100, 44, buf);
	UG_FontSelect(&FONT_8X14);
	sprintf(buf, "Vdc");
		if(mod==0){
		UG_SetForecolor(C_BLACK);
		UG_SetBackcolor(C_WHITE);
		}
		UG_PutString(100, 40, buf);
		UG_SetForecolor(C_WHITE);
		UG_SetBackcolor(C_BLACK);


	UG_FontSelect(&FONT_6X10);
	sprintf(buf, "\n%.2f", dt->Vamp);
	UG_PutString(150, 44, buf);
	UG_FontSelect(&FONT_8X14);
	sprintf(buf, "Va");
	UG_PutString(150, 40, buf);


	UG_FontSelect(&FONT_12X16);
	sprintf(buf, "  Hz\n%.2f", dt->freq);
	UG_PutString(150, 4, buf);

}

void screen(void){
	measuring(&dt);
	main_screen(&dt);
	spectum(&dt);

}






