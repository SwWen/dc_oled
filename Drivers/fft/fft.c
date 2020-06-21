#include "stm32f3xx.h"
#include "arm_math.h"
#include "ad7792.h"
#include "fft.h"
#include "math.h"
#include "arm_const_structs.h"
#include "adc.h"
#include "spi.h"
#include "systick.h"
float32_t fft_Sbuff[FFT_SIZE * 2] = { 0 };
//float32_t fft_Dbuff[FFT_SIZE] = { 0 };
uint32_t ifftFlag = 0;
uint32_t doBitReverse = 1;
float32_t dfreq = 0,test=0;
uint32_t max_index = 0;
float32_t max_result= 0;
 uint8_t amp=3;
void measuring (data *dt){
	static float32_t rms;
	spi_txrx(AD_CR_CR_W);
	spi_txrx(amp);
	spi_txrx(0x90);
	spi_txrx(AD_CR_MR_W);
	spi_txrx(0x00);
	spi_txrx(0x01);
	for (uint16_t i = 0; i < FFT_SIZE; i++) {

		fft_Sbuff[i * 2] = (ad7792_data_get()- 32768)*(1-cosf((2*PI*i)/FFT_SIZE));
		fft_Sbuff[i * 2 + 1] = 0;
	}
	arm_cfft_f32(&arm_cfft_sR_f32_len64, fft_Sbuff, ifftFlag, doBitReverse);
	arm_cmplx_mag_f32(fft_Sbuff, dt->fft_Dbuff, FFT_SIZE);
	arm_max_f32(dt->fft_Dbuff, FFT_SIZE / 2, &max_result, &max_index);
	dt->max=max_result;
if((max_result/FFT_SIZE>15000)&&(amp!=0)){
	amp--;
	return ;
}else if ((max_result/FFT_SIZE<200)&&(amp!=7)) {
	amp++;
	return ;
}

	dfreq = 2 * (dt->fft_Dbuff[max_index + 1] - dt->fft_Dbuff[max_index - 1]);
	dfreq = dfreq/((dt->fft_Dbuff[max_index - 1] + dt->fft_Dbuff[max_index + 1])+ (2 * dt->fft_Dbuff[max_index]));
	dt->freq = max_index + dfreq;
	dt->freq = (472 * dt->freq) / FFT_SIZE;
		rms=PI*dt->fft_Dbuff[max_index]*dfreq*(1-(dfreq*dfreq));
		rms=rms/sinf(PI*dfreq);

float32_t ma=170,md=9.0;
dt->Vpp=(rms/ma)/powf(2,amp);
dt->Vamp=dt->Vpp/2;
dt->Vrms=dt->Vpp/2/1.41;
spi_txrx(AD_CR_CR_W);
spi_txrx(amp);
spi_txrx(0x90);
spi_txrx(AD_CR_MR_W);
spi_txrx(0x00);
spi_txrx(0x0A);

dt->dc = (ad7792_data_get() - 32768)/powf(2,amp)/md;

	spi_txrx(AD_CR_CR_W);
	spi_txrx(0x16);
	spi_txrx(0x91);
	spi_txrx(AD_CR_MR_W);
	spi_txrx(0x00);
	spi_txrx(0x0A);
	dt->Trms = (ad7792_data_get()/121.36);


	dt->A=amp;
	dt->index=max_index;

//return dt;
}







