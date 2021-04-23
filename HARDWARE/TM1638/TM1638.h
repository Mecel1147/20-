#ifndef	_TM1638_H
#define	_TM1638_H
#include "sys.h"


#ifndef __TM1638_H
#define __TM1638_H
#include "sys.h" 
						  
// 	
   	   		   
//?????????
extern  unsigned char tab[17];
						 
						 
//DIO??????
#define SDA_IN()  {GPIOE->MODER&=~(3<<(9*2));GPIOE->MODER|=0<<9*2;}	//PE9????
#define SDA_OUT() {GPIOE->MODER&=~(3<<(9*2));GPIOE->MODER|=1<<9*2;} //PE9????

//IO????	 
#define TM1638_CLK    PEout(8) //CLK
#define OUT_SDA       PEout(9) //DIO	 
#define READ_SDA      PEin(9)  //??DIO
#define TM1638_STB    PEout(10)//????STB

//#define LED    PCout(4) //LED

//TM1638
void TM1638_Write(unsigned char	DATA);			//?????
unsigned char TM1638_Read(void);					//?????	
void Write_COM(unsigned char cmd);//?????
unsigned char Read_key(void);
void Write_DATA(unsigned char add,unsigned char DATA);		//????????
void Write_oneLED(unsigned char num,unsigned char flag);	//??????LED??,num??????led??,flag?0???,??0???
void Write_allLED(unsigned char LED_flag);					//????LED??,LED_flag????LED??

void TM1638_Clear(void);  //????
void NixieShow(void); //?????
void NixieShowOne(u8 num,u8 data); //???????
void NixieShowNum(u32 data); //???????
	
void TM1638_init(void);

void LED_init(void);
#endif


/*
//TM1638ģ�����Ŷ���
#define DIO PEout(2)
#define	CLK PEout(3)
#define	STB PEout(4)


void GPIO1_Init(void);	//GPIO��ʼ��
void TM1638_Write(unsigned char	DATA);			//д���ݺ���
unsigned char TM1638_Read(void);					//�����ݺ���
void Write_COM(unsigned char cmd);		//����������
unsigned char Read_key(void);
void Write_DATA(unsigned char add,unsigned char DATA);		//ָ����ַд������
void Write_allLED(unsigned char LED_flag);				//����ȫ��LED������LED_flag��ʾ����LED״̬
//TM1638��ʼ������
void init_TM1638(void);

*/
#endif