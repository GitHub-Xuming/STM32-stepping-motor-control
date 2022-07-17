#include "step_motor.h" 

/**
  * @brief  计算电机速度
  * @param  motor 被控对象
  * @param 
  */
void StepMotorSpeedComput(stepMotor *motor)
{
	motor->speed = motor->goal_position - motor->last_position;
	motor->last_position = motor->goal_position;
}

/**
  * @brief  设置目标位置
  * @param  motor 被控对象
  * @param  pos 
  */
void StepMotorSetGoalPosition(stepMotor *motor, int pos)
{
	motor->goal_position = pos;
}
 
/**
  * @brief  获取当前位置
  * @param  motor 被控对象
  * @param  
  */
int StepMotorGetCurPosition(stepMotor *motor)
{
	return motor->cur_position;
}

/**
 * @brief  放在中断函数执行
 * @param  motor 被控对象
 * @param 
 */
void StepMotorCtrl(stepMotor *motor)
{
	switch(motor->status)
	{
		case 0:
		if(motor->goal_position != motor->cur_position)  //扫描
		{
			motor->pos_bias = motor->goal_position - motor->cur_position; //得到偏差
			motor->status = 1;
		}			
		break;
		
		case 1:
		if(motor->pos_bias > 0)
		{
			motor->pluse_count ++;
			*(motor->gpio_dir) = 1;  //正方向
			*(motor->gpio_pluse) = !*(motor->gpio_pluse);
			if(motor->pluse_count == (motor->pos_bias * 2))
			{
				motor->cur_position += motor->pos_bias;
				motor->pluse_count = 0;
				motor->status = 0;
			}
		}
		else
		{
			motor->pluse_count ++;
			*(motor->gpio_dir) = 0;  //负方向
			*(motor->gpio_pluse) = !*(motor->gpio_pluse);
			if(motor->pluse_count == ((-motor->pos_bias) * 2))
			{
				motor->cur_position += motor->pos_bias;
				motor->pluse_count = 0;
				motor->status = 0;
			}					
		}		
		break;

		default:
		break;
	}
}


