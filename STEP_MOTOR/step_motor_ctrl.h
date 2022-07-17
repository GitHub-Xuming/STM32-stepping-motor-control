#ifndef __STEP_MOTOR_CTRL_H
#define __STEP_MOTOR_CTRL_H
#include "sys.h"

#define     MOTOR_NUM   1  //电机数量
#define     DIR         PDout(1)  //电机IO定义（方向）
#define     STEP        PDout(3)  //脉冲
#define     ABS(x)      ((x > 0) ? (x) : (-(x))) 

/*电机枚举*/
typedef enum
{
    Motor_0 = 0,
    Motor_1,
    Motor_2,
    Motor_3,
    Motor_4,
    Motor_5,
}MOTOR_KIND;

void StepMotorInit();
void StepMotorSpeedTimeFunc();
void StepMotorCtrlTimeFunc();
void StepMotorSetGoalPos(MOTOR_KIND motor_kind, int pos);
float StepMotorGetSpeed(MOTOR_KIND motor_kind);
int StepMotorGetCurPos(MOTOR_KIND motor_kind);
float StepMotorGetMaxSpeed();

#endif
