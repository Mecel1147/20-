#include "grayscale.h"

u16 ADC_Data[10] = {0};	

u16 boundary = 3900;
//u16 grayscale1 = 0;
//u16 grayscale2 = 0;
//u16 grayscale3 = 0;
//u16 grayscale4 = 0;
//u16 grayscale5 = 0;
//u16 grayscale6 = 0;
//u16 grayscale7 = 0;
//u16 grayscale8 = 0;
//u16 grayscale9 = 0;
//u16 grayscale10 = 0;
//u16 grayscale11 = 0;
//u16 grayscale12 = 0;

void ADC_DMA_Init(void)
{    
	//�ṹ�嶨��
	DMA_InitTypeDef DMA_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;	
	GPIO_InitTypeDef  GPIO_InitStructure;

	//DMA2ʱ��ʹ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 
	//ADC1ʱ��ʹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	//GPIOA/B/Cʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//PA4/5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//PB0/1 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);//PC0/1/2/3/4/5 


	//DMA2_Stream4
	DMA_DeInit(DMA2_Stream4);
	//�ȴ�DMA������
	while (DMA_GetCmdStatus(DMA2_Stream4) != DISABLE);
	//ͨ��ѡ��
  DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
	//DMA�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&ADC1->DR);	
	//DMA�洢��0��ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_Data;  
  // //dma���䷽��---���赽�洢��ģʽ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	//����DMA�ڴ���ʱ�������ĳ���  
	DMA_InitStructure.DMA_BufferSize = 12;	
	//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  //����DMA���ڴ����ģʽ  
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  //���������ֳ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //�ڴ������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	//����DMA�Ĵ���ģʽ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  //����DMA�����ȼ��� 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������:��ʹ��FIFOģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  //ָ����FIFO��ֵˮƽ 
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  //ָ����Burstת�������ڴ洫��  
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  //ָ����Burstת��������Χת�� */ 
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	//����DMA��ͨ��   
	DMA_Init(DMA2_Stream4, &DMA_InitStructure);
	//ʹ��ͨ��
  DMA_Cmd(DMA2_Stream4, ENABLE);

	//ADC1��λ�͸�λ����
  ADC_DeInit();

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//����Ϊ12λ 
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���
	ADC_InitStructure.ADC_NbrOfConversion = 12;//����ת����ͨ����
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_Init(ADC1, &ADC_InitStructure);//��ʼ��	

	//����ָ��ADC�Ĺ���ͨ��ͨ����һ�����У�����ʱ�� 
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_2,1,ADC_SampleTime_15Cycles);   //PA2     ADC_Data[0]
  //ADC_RegularChannelConfig(ADC1, ADC_Channel_3,2,ADC_SampleTime_15Cycles);    //PA3
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4,1,ADC_SampleTime_15Cycles);    //PA4      ADC_Data[0]  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5,2,ADC_SampleTime_15Cycles);		//PA5			 ADC_Data[1]
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8,3,ADC_SampleTime_15Cycles);    //PB0			 ADC_Data[2]
  ADC_RegularChannelConfig(ADC1, ADC_Channel_9,4,ADC_SampleTime_15Cycles);    //PB1			 ADC_Data[3]
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10,5,ADC_SampleTime_15Cycles); 	//PC0
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11,6,ADC_SampleTime_15Cycles); 	//PC1
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12,7,ADC_SampleTime_15Cycles); 	//PC2
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13,8,ADC_SampleTime_15Cycles);	//PC3
  ADC_RegularChannelConfig(ADC1, ADC_Channel_14,9,ADC_SampleTime_15Cycles); 	//PC4
  ADC_RegularChannelConfig(ADC1, ADC_Channel_15,10,ADC_SampleTime_15Cycles); 	//PC5

  //ʹ��ADC��DMA  
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE); 
  ADC_DMACmd(ADC1, ENABLE);
	//����ADת����  
  ADC_Cmd(ADC1, ENABLE);
	//ʹ��ָ����ADC���
	ADC_SoftwareStartConv(ADC1);
}	

//PC0
int grayscale1(void)
{
	if(ADC_Data[4] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}
//PC1
int grayscale2(void)
{
	if(ADC_Data[5] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}

//PC2
int grayscale3(void)
{
	if(ADC_Data[6] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}
//PC3
int grayscale4(void)
{
	if(ADC_Data[7] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}
//PC4
int grayscale5(void)
{
	if(ADC_Data[8] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}
//PC5
int grayscale6(void)
{
	if(ADC_Data[9] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}

//pA2
/*int grayscale7(void)
{
	if(ADC_Data[0] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}
//pA3
int grayscale8(void)
{
	if(ADC_Data[1] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}*/


//pA4
int grayscale7(void)
{
	if(ADC_Data[0] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}

//PA5
int grayscale8(void)
{
	if(ADC_Data[1] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}

//PB0
int grayscale9(void)
{
	if(ADC_Data[2] > boundary)//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}

//PB1
int grayscale10(void)
{
	if(ADC_Data[3] > (boundary))//��ɫ
	{
		return 1;
	}
	else//��ɫ
	{
		return 0;
	}
}