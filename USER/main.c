#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "servor.h"
#include "remote.h"
#include "timer.h"
#include "TM1638.h"
#include "motor.h"
#include "ds18b20.h"
#include "grayscale.h"

/*
  MOTOR PWM PA7
				in  Pe11
	CLK 	PE8
	DIO		PE9
	STB		PE10
	DS18B20  PE5
	
*/
unsigned char num[8];		
int data=0;
int flag=1;
int status=0;
int main(void)
{ 
	unsigned char i;
	short temp;
	delay_init(168);
	
	TM1638_init();	//初始化TM1638
	NixieShow(); //显示01234567   
	TM1638_Clear();  //清零
	TIM3_Int_Init(10000-1,8410-1); //84m/8400=10k 10K/10000=1HZ
	//TIM1_Pwm_Init(1000-1,168-1);//168m/168=1m
	TIM14_PWM_Init(1000-1,86);//84M/84=1M  1M/1K=1K
	ADC_DMA_Init();
	//while(DS18B20_Init())	//DS18B20初始化	
		
		TIM_SetCompare1(TIM14,200);

	while(1)
	{
		PEout(11)=1;//电机转动
		Write_oneLED(1,1);//灯亮
		if(grayscale2()==1)
		{
			PEout(11)=0;//电机停转
			Write_oneLED(1,0);//灯灭
			delay_ms(1000);
		
		}
	
	
	}
	while(1)
	{	
		i=Read_key();
		if(i==0x04)
		{
			while(Read_key()!=0x08);
			data+=60;
		}
		if(i==0x02)
		{
			while(Read_key()!=0x08);
			flag++;
			if(flag>3)flag=1;
		}
		if(i==0x05)
		{
			while(Read_key()!=0x08);
			data=0;
			
		}
		if(i==0x06)
		{
			while(Read_key()!=0x08);
			status=!status;
			TM1638_Clear();
			
		}
		switch(flag)
		{
			case 1 :	TIM_SetCompare1(TIM14,200);
								Write_oneLED(1,0);
								Write_oneLED(2,0);break;
			case 2 :	TIM_SetCompare1(TIM14,300);
								Write_oneLED(0,0);
								Write_oneLED(2,0);break;
			case 3 :	TIM_SetCompare1(TIM14,700);
								Write_oneLED(0,0);
								Write_oneLED(1,0);break;
		}
		if(data==0)
		{
			TIM_Cmd(TIM14,DISABLE);
			PCout(4)=0;
			Write_allLED(0);
			//TIM_SetCompare1(TIM14,0);
		}
			else 
			{
				TIM_Cmd(TIM14,ENABLE);
				Write_oneLED(flag-1,1);
				PCout(4)=1;
			}
			temp=DS18B20_Get_Temp();
			if(status)//室温
			{
				NixieShowOne(5,temp/100);
				NixieShowOne(6,temp/10%10);
				NixieShowOne(7,12);
				NixieShowOne(0,16);
				NixieShowOne(2,16);
				NixieShowOne(1,4);
			}
			else
			{
				NixieShowOne(0,16);
				NixieShowOne(2,16);
				NixieShowOne(1,flag);
				//NixieShowOne(3,i);

		
				NixieShowNum(data);
			
			}	
	}	
}


/*

extern unsigned char tab[16];//={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

unsigned char num[8];		//各个数码管显示的值

int main(void)
{
	unsigned char i;
	delay_init(168);		//延时初始化 
	GPIO1_Init();
	init_TM1638();	                           //初始化TM1638
//	for(i=0;i<8;i++)
//	Write_DATA(i<<1,tab[0]);	               //初始化寄存器
	
	while(1)
	{
//		Write_DATA(0,tab[6]);
//		Write_allLED(1<<i);
		i=Read_key();                          //读按键值
		if(i<8)
		{
			num[i]++;
			while(Read_key()==i);		       //等待按键释放
			if(num[i]>15)
			num[i]=0;
			Write_DATA(i*2,tab[num[i]]);
			Write_allLED(1<<i);
		}
	}
}

*/












/*
int flag=0;
		int buffstr=0;
		int buffangle=0;
		int x[3]=0;
		int str=0;
		int angle=0;
		int sure=1;
		int count=0;
		u8 mode=0;//连按标志
		int modeflag=0;
	 u8 i=0;
	u8 num[3]={1,2,3} ;
int main(void)
{ 
 	char a[]="Elevation";
	char b[]="Angle";
	char c[]="Mode  up";
	char d[]="Mode num";
	u8 t;
	u8 len;	
	void key_scan();
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	LED_Init();		  		//初始化与LED连接的硬件接口  	
	TIM14_PWM_Init(20000-1,84-1); //84m/84=1m	1000k/20k=	50
	IIC_Init();
	OLED_Init();//oled初始化
	Remote_Init();				//红外接收初始化		
	TIM3_Int_Init(500,8400-1); //定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms   	
	while(1)
	{
//			printf("#2P1400#3P1853#4P1300#5P1300#6P1382T500\r\n");delay_ms(600);
//			printf("#2P1735#3P1676#4P1670#5P1559#6P1382T500\r\n");delay_ms(600);
//			printf("#1P1670#2P1700#3P1350#4P1670#5P1559#6P1765T500\r\n");delay_ms(600);
		  key_scan();
			if(flag==0)
			{
				Servor_angle(str);
				OLED_ShowNum(70,1,str,6,10);
				OLED_ShowNum(70,3,angle,6,10);
				OLED_ShowString(10,1,a,0);
				OLED_ShowString(10,3,b,0);
				OLED_ShowString(10,5,c,0);
				//printf("#1PCSD\r\n");
				//printf("#1PULK\r\n");
//		printf("#1P542T100\r\n");delay_ms(700);
//		printf("#1P542T100\r\n");delay_ms(700);
				TIM_SetCompare1(TIM14,500+11.1*angle);
			}
			else
			{
				
				OLED_ShowString(10,5,d,0);
				OLED_ShowNum(70,1,buffstr,6,10);
				OLED_ShowNum(70,3,buffangle,6,10);
				OLED_ShowNum(70,5,count,6,10);
				OLED_ShowString(10,1,a,0);
				OLED_ShowString(10,3,b,0);
				//printf("#1PCSD\r\n");
				//printf("#1PULK\r\n");
//			printf("#1P542T100\r\n");delay_ms(700);
//			printf("#1P542T100\r\n");delay_ms(700);
				//if(sure==0)
				
				TIM_SetCompare1(TIM14,500+11.1*buffangle);
				Servor_angle(buffstr);
				
				//buffangle=0;
				}
			
	}
}

void key_scan()
{
		u8 key;
		
		key=Remote_Scan();	
		if(key&&mode)
		{	 
			delay_ms(120);
//			LCD_ShowNum(86,130,key,3,16);		//显示键值
//			LCD_ShowNum(86,150,RmtCnt,3,16);	//显示按键次数		  
			switch(key)
			{
				//case 0:str="ERROR";break;			   
				//case 162:str="POWER";break;	    
				case 98:
					{
						sure=1;
								str++;
					}
					break;	    
				case 2:
				{
					sure=0;
					count=0;
					x[0]=0;x[1]=0;x[2]=0;
				}break;		 
				case 226:flag=!flag;break;		  
				case 194:
					{
								angle++;
					}
					break;	   
				case 34:
					{	
					angle--;
					
				}
				break;		  
//				case 224:str="VOL-";break;		  
				case 168:
 			{	
					str--;
					
				}
			break;		   
//				case 144:str="VOL+";break;		    
				case 104:
				{
					sure=1;
					str=1;
					count++;
				}	
					break;		  
				case 152:
					{
						sure=1;
					str=2;
					count++;
				}	
					break;		     
				case 176:
					{
						sure=1;
						str=3;
						count++;
				}	
					break;		  
				case 48:
				{
					sure=1;
					str=4;
					count++;
				}	
					break;		  					
				case 24:
				{
					sure=1;
					str=5;
					count++;
				}	
					break;		  
				case 122:
				{
						sure=1;
						str=6;
						count++;
				}	
					break;		  
				case 16:
				{
					sure=1;
					str=7;
					count++;
				}	
					break;		  
				case 56:	
				{
					sure=1;
					str=8;
					count++;
				}	
					break;		  
				case 90:
				{
					sure=1;
					str=9;
					count++;
				}	
					break;		  
				case 66:
				{
					sure=1;
					str=0;
					count++;
				}	
					break;		  
				case 82:modeflag=!modeflag;break;		 
			}
		if(str>=31)str=31;
		if(str<=0)str=0;
		if(angle>=180)angle=180;
		if(angle<=0)angle=0;
		if(count==1)x[0]=str;
		if(count==2)x[1]=str;
		if(count==3)x[2]=str;
			if(!modeflag)buffangle=100*x[0]+10*x[1]+x[2];
			if(modeflag)buffstr=100*x[0]+10*x[1]+x[2];
		mode=0;
		}
	else 	mode=1;

}
*/
