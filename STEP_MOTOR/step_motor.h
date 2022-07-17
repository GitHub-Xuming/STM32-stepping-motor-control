#ifndef __STEP_MOTOR_H
#define __STEP_MOTOR_H
#include "sys.h"

/*步进电机控制类*/
typedef struct 
{
    volatile unsigned long  *gpio_dir; //电机方向控制GPIO
    volatile unsigned long  *gpio_pluse; //电机脉冲GPIO
    int pluse_count;
    int goal_position;
    int last_position;
    int cur_position;
    int pos_bias;
    int speed;
    uint8_t status;
}stepMotor;

void StepMotorSpeedComput(stepMotor *motor);
void StepMotorSetGoalPosition(stepMotor *motor, int pos);
void StepMotorCtrl(stepMotor *motor);
int StepMotorGetCurPosition(stepMotor *motor);

#endif
