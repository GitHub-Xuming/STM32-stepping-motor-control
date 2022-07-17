#include "timer.h"
#include "math.h"
#include "step_motor_ctrl.h" 
#include "encoder.h" 

/*系统时间*/
volatile u32 g_sys_time = 0;
u8 controlPeriodFlag = 0;

void (*SpeedComputCB)() = 0;
void (*MotorCtrlCB)() = 0;

float test_pos = 0, test_vel = 0, test_amplitude = 200, test_time = 0;
uint32_t Autoreload = 0; //定时器重载值，动态调整

/**
  * @brief  注册速度计算回调
  * @param  
  * @param 
  */
void RegistSpeedComputCB(void (*CallBack)())
{
	SpeedComputCB = CallBack;
}

/**
  * @brief  注册电机控制回调
  * @param  
  * @param 
  */
void RegistMotorCtrlCB(void (*CallBack)())
{
	MotorCtrlCB = CallBack;
}

/**
  * @brief  getControlPeriodFlag
  * @param  
  * @param 
  */
u8 getControlPeriodFlag()
{
	return controlPeriodFlag;
}

/**
  * @brief  setControlPeriodFlag
  * @param  
  * @param 
  */
 void setControlPeriodFlag(u8 val)
 {
	controlPeriodFlag = val;
 }

/**
  * @brief 通用定时器中断初始化
  * @param arr：自动重装值 
  * @param psc：时钟预分频数
  */
void TIMER_Init(u16 sys_arr,u16 sys_psc, u16 speed_arr,u16 speed_psc)
{
	/*SYS_CLOCK_TIMER*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SYS_CLOCK_TIMER,ENABLE);  ///使能SYS_CLOCK_TIMER时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = sys_arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=sys_psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(SYS_CLOCK_TIMER,&TIM_TimeBaseInitStructure);//初始化SYS_CLOCK_TIMER
	
	TIM_ITConfig(SYS_CLOCK_TIMER,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(SYS_CLOCK_TIMER,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=SYS_CLOCK_TIMER_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/*STEP_CTRL TIMER*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_STEP_CTRL_TIMER,ENABLE);  ///使能SYS_CLOCK_TIMER时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = speed_arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler= speed_psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(STEP_CTRL_TIMER,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(STEP_CTRL_TIMER,TIM_IT_Update,ENABLE); 
	TIM_Cmd(STEP_CTRL_TIMER,ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=STEP_CTRL_TIMER_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  get全局系统运行时间
  * @param  
  * @param 
  */
u32 getSysTime()
{
	return g_sys_time;
}
 

/**
  * @brief  定时器中断服务函数，在此函数中周期性Set Motor Position
  * @param  
  */
void SYS_CLOCK_TIMER_IRQHandler(void)
{	
	if(TIM_GetITStatus(SYS_CLOCK_TIMER,TIM_IT_Update)==SET) //溢出中断
	{
		g_sys_time ++;
		if(g_sys_time % 5 == 0) //5分频计算速度，达到滤波效果
		{
			if(SpeedComputCB) //速度计算回调
			{
				SpeedComputCB();
			}
		}

		test_time += 0.003f;
		test_pos = test_amplitude * sin(test_time); //正弦位置测试
		//test_vel = test_amplitude * cos(test_time); //电机速度

		//test_pos = getEncoderCount();  //使用编码器控制电机测试
		StepMotorSetGoalPos(Motor_0, test_pos);
		Autoreload = ABS(1000000.0 / (StepMotorGetMaxSpeed() * 3 * 200.0)); //1M：定时器频率，3： 与StepMotorCtrl中状态机执行逻辑有关，200：5分频计算速度，达到滤波效果（1000 / 5 = 200）
		if(Autoreload > 1000)  //频率下限
		{
			Autoreload = 1000;
		}
		TIM_SetAutoreload(STEP_CTRL_TIMER,Autoreload); //设定重载值，达到动态调整频率效果，减小CPU带宽占用
        TIM_SetCounter(STEP_CTRL_TIMER,0);   //清空计数器
	}
	
	TIM_ClearITPendingBit(SYS_CLOCK_TIMER,TIM_IT_Update);  //清除中断标志位
}

/**
  * @brief  定时器中断服务函数
  * @param  
  */
void STEP_CTRL_TIMER_IRQHandler(void)
{
	if(TIM_GetITStatus(STEP_CTRL_TIMER,TIM_IT_Update)==SET) //溢出中断
	{
		if(MotorCtrlCB)
		{
			MotorCtrlCB();  //电机控制回调
		}
	}
	TIM_ClearITPendingBit(STEP_CTRL_TIMER,TIM_IT_Update);  //清除中断标志位
}