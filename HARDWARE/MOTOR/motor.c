#include "motor.h"
#include "delay.h"
//配置TIM7
void TIM7_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);///使能TIM7时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);//允许定时器7更新中断
	TIM_Cmd(TIM7,ENABLE);//使能定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //定时器7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//配置TIM1_PWM四路通道 
void TIM1_Pwm_Init(u16 arr,u16 psc) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//使能PORTE时钟	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//TIM1时钟使能 
	         
	//GPIO_PinAFConfig(GPIOE,GPIO_PinSource9 ,GPIO_AF_TIM1);//GPIOE9复用为定时器1
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11| GPIO_Pin_13 | GPIO_Pin_14; //GPIO_Pin_9 |
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOE,&GPIO_InitStructure);
  	
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInitStructure.TIM_Prescaler = psc;   
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInitStructure.TIM_Period = arr;    
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	 
  //TIM_OC1Init(TIM1,&TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC2Init(TIM1,&TIM_OCInitStructure);
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC3Init(TIM1,&TIM_OCInitStructure);
  
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OC4Init(TIM1,&TIM_OCInitStructure);
  
  TIM_Cmd(TIM1,ENABLE);
  TIM_CtrlPWMOutputs(TIM1,ENABLE);
}



//配置Motor_Dir
void Motor_IO_Init(void)   
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	
	Motor_A_1 = 0; 
	Motor_A_2 = 1;   
	Motor_B_2 = 0;    
	Motor_B_1 = 1;    
	Motor_C_1 = 0;    
	Motor_C_2 = 1;  
	Motor_D_1 = 0;    
	Motor_D_2 = 1;    
}


//定时器7中断服务函数
void TIM7_IRQHandler(void)
{

	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
	{	
		TIM_SetCompare1(TIM1,500 );//Motor_D
    TIM_SetCompare2(TIM1,500 );//Motor_C
		TIM_SetCompare3(TIM1,500 );//Motor_B
		TIM_SetCompare4(TIM1,500 );//Motor_A		
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //清除中断标志位
}


void Motor_Init(void)
{
  TIM1_Pwm_Init(1000-1,8-1);
  TIM7_Init(1000-1,84-1);	//84Mhz/840=100khz 100k /2k=50 
	Motor_IO_Init();
}
