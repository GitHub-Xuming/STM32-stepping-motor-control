#include "encoder.h" 

int circle_count = 1;
int Encoder = 0;

/**                  
  * @brief  Encoder init
  * @param  
  * @param   
  */
void ENCODER_TIMER_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);			
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_ENCODER_TIMER, ENABLE);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  		
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3); 
	
	TIM_DeInit(ENCODER_TIMER);								
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);			
	TIM_TimeBaseStructure.TIM_Prescaler = 0;						
	TIM_TimeBaseStructure.TIM_Period = ENCODER_RESOLUTION - 1;		
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;   
	TIM_TimeBaseInit(ENCODER_TIMER, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(ENCODER_TIMER, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising ,TIM_ICPolarity_Rising); 

	TIM_ICStructInit(&TIM_ICInitStructure);	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1 | TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection =TIM_ICSelection_DirectTI;	
  	TIM_ICInitStructure.TIM_ICFilter = 0;	
  	TIM_ICInit(ENCODER_TIMER, &TIM_ICInitStructure);
	
	TIM_ITConfig(ENCODER_TIMER,TIM_IT_Update,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel=ENCODER_TIMER_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_SetCounter(ENCODER_TIMER, 0);
	TIM_Cmd(ENCODER_TIMER, ENABLE);
}

/**                  
  * @brief  setEncoderCount
  * @param  
  * @param   
  */
int setEncoderCount(int count)
{
	Encoder = count;
}

/**                  
  * @brief  getEncoderCount
  * @param  
  * @param   
  */
int getEncoderCount()
{
	Encoder = TIM_GetCounter(ENCODER_TIMER) + ENCODER_RESOLUTION * circle_count; 
	return Encoder;
}

/**                  
  * @brief  
  * @param  
  * @param   
  */
void ENCODER_TIMER_IRQHandler(void)
{
	if(TIM_GetITStatus(ENCODER_TIMER,TIM_IT_Update)==SET)
	{       
		if((ENCODER_TIMER->CR1>>4 & 0x01)==0) //DIR==0
			circle_count++;
		else if((ENCODER_TIMER->CR1>>4 & 0x01)==1)//DIR==1
			circle_count--;
	}
	TIM_ClearITPendingBit(ENCODER_TIMER,TIM_IT_Update); 
}
