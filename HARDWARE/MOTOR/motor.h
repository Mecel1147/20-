#ifndef __MOTOR_H
#define __MOTOR_H 
#include "sys.h"   

//电机方向控制IO宏定义
#define Motor_A_2   PEout(0)
#define Motor_A_1   PEout(1)
#define Motor_B_2   PEout(2)
#define Motor_B_1   PEout(3)
#define Motor_C_1   PEout(4)
#define Motor_C_2   PEout(5)
#define Motor_D_1   PEout(6)
#define Motor_D_2   PCout(13)



//电机速度PI的时间基准
void TIM7_Init(u16 arr,u16 psc);


//电机的PWM信号初始化函数
void TIM1_Pwm_Init(u16 arr,u16 psc); 

//电机方向控制IO初始化
void Motor_IO_Init(void);       

//电机硬件底层初始化
void Motor_Init(void);
#endif
