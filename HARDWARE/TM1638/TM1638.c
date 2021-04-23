#include "TM1638.h"
//#include	<REGX51.H>
#include "delay.h"


unsigned char tab[17]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                           0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40};


// **************************************** TM1638 ************************
//??????
void TM1638_Write(u8	DATA)			//写字节
{
	unsigned char i;
	SDA_OUT(); 
	delay_us(2); 
	for(i=0;i<8;i++)
	{
		TM1638_CLK=0;
		delay_us(2); 
		if(DATA&0X01)
			OUT_SDA=1;
		else
			OUT_SDA=0;
		delay_us(2); 
		DATA>>=1;
		TM1638_CLK=1;
		delay_us(2); 
	}
}

u8 TM1638_Read(void)					//读字节
{
	unsigned char i;
	unsigned char temp=0;

	SDA_IN();//SDA?????
	delay_us(2); 
	for(i=0;i<8;i++)
	{
		temp>>=1;
		TM1638_CLK=0;
		delay_us(2); 
		if(READ_SDA)
			temp|=0x80;
		TM1638_CLK=1;
		delay_us(2); 
	}
	return temp;
}

void Write_COM(unsigned char cmd)		//发送命令字
{
	TM1638_STB=0;
	TM1638_Write(cmd);
	TM1638_STB=1;
}

void Write_DATA(unsigned char add,unsigned char DATA)		//指定地址写入数据
{
	TM1638_STB=0;
	TM1638_Write(0xc0|add); //1100 0000 
	TM1638_Write(DATA);
	TM1638_STB=1;
}

u8 Read_key(void)  //读按键
{
	unsigned char c[4],i,key_value=0;
	TM1638_STB=0;
	delay_us(2);
	TM1638_Write(0x42);		  //读键扫数据
	for(i=0;i<4;i++)		
		c[i]=TM1638_Read();
	TM1638_STB=1;					           //4个数据合成一个字节
	
	for(i=0;i<4;i++)
		key_value|=c[i]<<i;
	for(i=0;i<8;i++)
		if((0x01<<i)==key_value)
			break;
	return i;  //返回键值，无键值返回0x08
		
   // 8?  col:K3  row:KS1~KS8
//  if(c[0]==0x01)  key_value=1;
//	if(c[0]==0x10)  key_value=5;
//	if(c[1]==0x01)  key_value=2;
//	if(c[1]==0x10)  key_value=6;
//	if(c[2]==0x01)  key_value=3;
//	if(c[2]==0x10)  key_value=7;
//	if(c[3]==0x01)  key_value=4;
//	if(c[3]==0x10)  key_value=8;   
//  return (key_value);         //返回键值
}

//单独控制一个LED函数
//num为需要控制的LED序号,flag为0时熄灭,为1点亮
void Write_oneLED(unsigned char num,unsigned char flag)	//????LED
{
	if(flag)
		Write_DATA(2*num+1,1);
	else
		Write_DATA(2*num+1,0);
}  	

//控制全部LED函数
//LED_flag表示各个LED状态
void Write_allLED(unsigned char LED_flag)	 //控制全部LED
{
	unsigned char i;
	for(i=0;i<8;i++)
		{
			if(LED_flag&(1<<i))
				//Write_DATA(2*i+1,3);
				Write_DATA(2*i+1,1);
			else
				Write_DATA(2*i+1,0);
		}
}

void TM1638_Clear(void)  //清除显示
{	
	u8 i;
	Write_COM(0x8f);       //亮度 (0x88-0x8f)8级亮度可调
  Write_COM(0x40);       //自动地址
	
	TM1638_STB=0;		  
	TM1638_Write(0xc0);    //设置起始地址
	for(i=0;i<16;i++)	   //传送16字节的数据
		TM1638_Write(0x00);
	TM1638_STB=1;
}

void NixieShow(void) //数码管显示01234567
{
	u8 i;
	Write_COM(0x8d);  //亮度(0x88-0x8f)8级亮度可调
	Write_COM(0x44);  //地址固定
	for(i=0;i<8;i++)
			Write_DATA(i<<1,tab[i]);	 //依次显示01234567
}

//num数码管数据    data显示的数字
void NixieShowOne(u8 num,u8 data) //单个数码管显示
{
	Write_COM(0x88);  //亮度(0x88-0x8f)8级亮度可调
	Write_COM(0x44);  //地址固定
	Write_DATA(num<<1,tab[data]);	
}


void NixieShowNum(u32 data) //???????
{
//		NixieShowOne(0,data/10000000%10); //???????
//		NixieShowOne(1,data/1000000%10); //???????
//		NixieShowOne(2,data/100000%10); //???????
//		NixieShowOne(3,data/10000%10); //???????
		NixieShowOne(4,data/1000%10); //???????
		NixieShowOne(5,data/100%10); //???????
		NixieShowOne(6,data/10%10); //???????
		NixieShowOne(7,data%10); //???????
}

void TM1638_init(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//??GPIOE??

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//??GPIOE??



  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	TM1638_CLK=1;
	OUT_SDA=1;
	PEout(11)=1;
}




/*
#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0

//共阴数码管显示代码
unsigned char tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};
//TM1638模块引脚定义
//sbit	DIO=P1^0;
//sbit	CLK=P1^1;
//sbit	STB=P1^2;

void GPIO1_Init(void)	//GPIO初始化
{
	  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOF时钟

  //GPIOE 2 3 4初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3|GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	
	GPIO_ResetBits(GPIOE,GPIO_Pin_2 | GPIO_Pin_3|GPIO_Pin_4);

}




void TM1638_Write(unsigned char	DATA)			//写数据函数
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		CLK=0;
		if(DATA&0X01)
			DIO=1;
		else
			DIO=0;
		DATA>>=1;
		CLK=1;
	}
}
unsigned char TM1638_Read(void)					//读数据函数
{
	unsigned char i;
	unsigned char temp=0;
	DIO=1;	//设置为输入
	for(i=0;i<8;i++)
	{
		temp>>=1;
		CLK=0;
		if(DIO)
			temp|=0x80;
		CLK=1;
	}
	return temp;
}
void Write_COM(unsigned char cmd)		//发送命令字
{
	STB=0;
	TM1638_Write(cmd);
	STB=1;
}
unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
	STB=0;
	TM1638_Write(0x42);		           //读键扫数据 命令
	for(i=0;i<4;i++)		
		c[i]=TM1638_Read();
	STB=1;					           //4个字节数据合成一个字节
	for(i=0;i<4;i++)
		key_value|=c[i]<<i;
	for(i=0;i<8;i++)
		if((0x01<<i)==key_value)
			break;
	return i;
}
void Write_DATA(unsigned char add,unsigned char DATA)		//指定地址写入数据
{
	Write_COM(0x44);
	STB=0;
	delay_us(2);
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
	STB=1;
	delay_us(2);
}*/
/*
void Write_oneLED(unsigned char num,unsigned char flag)	//单独控制一个LED函数，num为需要控制的led序号，flag为0时熄灭，不为0时点亮
{
	if(flag)
		Write_DATA(2*num+1,1);
	else
		Write_DATA(2*num+1,0);
}  	*/
/*
void Write_allLED(unsigned char LED_flag)					//控制全部LED函数，LED_flag表示各个LED状态
{
	unsigned char i;
	for(i=0;i<8;i++)
		{
			if(LED_flag&(1<<i))
				//Write_DATA(2*i+1,3);
				Write_DATA(2*i+1,1);
			else
				Write_DATA(2*i+1,0);
		}
}

//TM1638初始化函数
void init_TM1638(void)
{
	unsigned char i;
	Write_COM(0x8b);       //亮度 (0x88-0x8f)8级亮度可调
	Write_COM(0x40);       //采用地址自动加1
	STB=0;		           //
	TM1638_Write(0xc0);    //设置起始地址

	for(i=0;i<16;i++)	   //传送16个字节的数据
		TM1638_Write(0x00);
	STB=1;
}
*/
