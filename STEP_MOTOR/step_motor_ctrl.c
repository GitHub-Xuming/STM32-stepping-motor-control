#include "step_motor.h" 
#include "step_motor_ctrl.h" 
#include "timer.h" 

stepMotor test_motor[MOTOR_NUM];

/**
  * @brief  电机IO初始化
  * @param  
  * @param 
  */
static void StepMotorIO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIO时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO

	GPIO_SetBits(GPIOD, GPIO_Pin_1 | GPIO_Pin_3);
}

/**
  * @brief  
  * @param  
  * @param 
  */
void StepMotorInit()
{
	StepMotorIO_Init();
	RegistMotorCtrlCB(StepMotorCtrlTimeFunc);  //定时器“电机控制”回调函数注册
	RegistSpeedComputCB(StepMotorSpeedTimeFunc); //定时器“电机速度计算”回调函数注册

	test_motor[Motor_0].gpio_dir = &DIR;  
	test_motor[Motor_0].gpio_pluse = &STEP;
}

/**
  * @brief  获取对应电机的速度
  * @param  
  * @param 
  */
 float StepMotorGetSpeed(MOTOR_KIND motor_kind)
 {
	return test_motor[motor_kind].speed;
 }

/**
  * @brief  获取最大的速度，因为定时器频率在时刻调整，取最大值
  * @param  
  * @param 
  */
 float StepMotorGetMaxSpeed()
 {
	float max_speed = 0;
	for(int i = 0; i < MOTOR_NUM; i++)
	{
		if(ABS(test_motor[i].speed) > max_speed)
		{
			max_speed = test_motor[i].speed;
		}
	}
	return max_speed;
 }

/**
  * @brief  
  * @param  
  * @param 
  */
void StepMotorSpeedTimeFunc()
{
	for(int i = 0; i < MOTOR_NUM; i++)
	{
		StepMotorSpeedComput(&test_motor[i]);
	}
}

/**
  * @brief  
  * @param  
  * @param 
  */
void StepMotorCtrlTimeFunc()
{
	for(int i = 0; i < MOTOR_NUM; i++)
	{
		StepMotorCtrl(&test_motor[i]);
	}
}

/**
  * @brief  设置电机目标位置
  * @param  
  * @param 
  */
void StepMotorSetGoalPos(MOTOR_KIND motor_kind, int pos)
{
	StepMotorSetGoalPosition(&test_motor[motor_kind], pos);
}

/**
  * @brief  
  * @param  
  * @param 
  */
int StepMotorGetCurPos(MOTOR_KIND motor_kind)
{
	return StepMotorGetCurPosition(&test_motor[motor_kind]);
}

