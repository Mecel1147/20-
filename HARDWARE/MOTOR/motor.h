#ifndef __MOTOR_H
#define __MOTOR_H 
#include "sys.h"   

//����������IO�궨��
#define Motor_A_2   PEout(0)
#define Motor_A_1   PEout(1)
#define Motor_B_2   PEout(2)
#define Motor_B_1   PEout(3)
#define Motor_C_1   PEout(4)
#define Motor_C_2   PEout(5)
#define Motor_D_1   PEout(6)
#define Motor_D_2   PCout(13)



//����ٶ�PI��ʱ���׼
void TIM7_Init(u16 arr,u16 psc);


//�����PWM�źų�ʼ������
void TIM1_Pwm_Init(u16 arr,u16 psc); 

//����������IO��ʼ��
void Motor_IO_Init(void);       

//���Ӳ���ײ��ʼ��
void Motor_Init(void);
#endif
