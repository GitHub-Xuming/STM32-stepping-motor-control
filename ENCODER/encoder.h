#ifndef _ENCODER_H
#define _ENCODER_H
#include "sys.h"

#define ENCODER_TIMER               	TIM3
#define RCC_APB1Periph_ENCODER_TIMER   	RCC_APB1Periph_TIM3
#define ENCODER_TIMER_IRQHandler   	    TIM3_IRQHandler
#define ENCODER_TIMER_IRQn   			TIM3_IRQn
#define ENCODER_RESOLUTION				8001


void ENCODER_TIMER_Init();
int getEncoderCount();
int setEncoderCount(int count);

#endif


