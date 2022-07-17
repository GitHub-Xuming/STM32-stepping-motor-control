#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

#define SYS_CLOCK_TIMER                 	TIM2
#define RCC_APB1Periph_SYS_CLOCK_TIMER   	RCC_APB1Periph_TIM2
#define SYS_CLOCK_TIMER_IRQn   			 	TIM2_IRQn
#define SYS_CLOCK_TIMER_IRQHandler 	    	TIM2_IRQHandler

#define STEP_CTRL_TIMER                 	TIM4
#define RCC_APB1Periph_STEP_CTRL_TIMER   	RCC_APB1Periph_TIM4
#define STEP_CTRL_TIMER_IRQn   			 	TIM4_IRQn
#define STEP_CTRL_TIMER_IRQHandler 	    	TIM4_IRQHandler

void TIMER_Init(u16 sys_arr,u16 sys_psc, u16 speed_arr,u16 speed_psc);
void RegistMotorCtrlCB(void (*CallBack)());
void RegistSpeedComputCB(void (*CallBack)());
u32 getSysTime();
void setControlPeriodFlag(u8 val);
u8 getControlPeriodFlag();


#endif


