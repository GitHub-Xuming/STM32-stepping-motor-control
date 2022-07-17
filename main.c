#include "timer.h" 
#include "math.h" 
#include "step_motor.h"
#include "step_motor_ctrl.h"

int main(void)
{ 
	StepMotorInit();
	ENCODER_TIMER_Init();
	TIMER_Init(1000, 84 - 1, 100, 84 - 1); //SYS_CLOCK_TIMER 1k; STEP_CTRL_TIMER 10k
	
	while(1)
	{	

	}
}





