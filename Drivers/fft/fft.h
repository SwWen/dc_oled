#ifndef FFT_H
#define FFT_H
#include "stm32f3xx.h"
#include "arm_math.h"
void rfft_init(void);
#define FFT_SIZE 64
typedef struct {
	float32_t Vpp;
	float32_t freq;
	float32_t dc;
	float32_t Vrms;
	float32_t Vamp;
	float32_t Trms;
	float32_t max;
	float32_t fft_Dbuff[FFT_SIZE];
	uint8_t A;
	uint8_t index;
} data;
void measuring (data *dt);





#endif //FFT_H
