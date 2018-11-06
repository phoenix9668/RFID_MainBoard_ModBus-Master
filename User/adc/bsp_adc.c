/**
  ******************************************************************************
  * @file    bsp_bsp_adc.c
  * @author  phoenix
  * @version V1.0
  * @date    10-November-2017
  * @brief   adc driver
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */ 
#include "./adc/bsp_adc.h"

__IO uint16_t ADC_ConvertedValue[3];

static void MMA7361L_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(MMA7361L_ADC1_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MMA7361L_ADC2_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MMA7361L_ADC3_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MMA7361L_SL_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MMA7361L_GS_GPIO_CLK, ENABLE);
		
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = MMA7361L_ADC1_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(MMA7361L_ADC1_GPIO_PORT, &GPIO_InitStructure);

	// ���� IO
	GPIO_InitStructure.GPIO_Pin = MMA7361L_ADC2_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(MMA7361L_ADC2_GPIO_PORT, &GPIO_InitStructure);
	
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = MMA7361L_ADC3_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	    
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //������������
	GPIO_Init(MMA7361L_ADC3_GPIO_PORT, &GPIO_InitStructure);

	// ���� IO
	GPIO_InitStructure.GPIO_Pin = MMA7361L_SL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MMA7361L_SL_GPIO_PORT, &GPIO_InitStructure);
	
	// ���� IO
	GPIO_InitStructure.GPIO_Pin = MMA7361L_GS_GPIO_PIN;
	GPIO_Init(MMA7361L_GS_GPIO_PORT, &GPIO_InitStructure);
}

static void MMA7361L_ADC_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
  // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
  // ����DMAʱ��
  RCC_AHB1PeriphClockCmd(MMA7361L_ADC_DMA_CLK, ENABLE); 
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = MMA7361L_ADC_CDR_ADDR;	
  // �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;  
  // ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������СΪ��ָһ�δ����������
	DMA_InitStructure.DMA_BufferSize = 3;	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  // �洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO ��С��FIFOģʽ��ֹʱ�������������	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// ѡ�� DMA ͨ����ͨ������������
  DMA_InitStructure.DMA_Channel = MMA7361L_ADC_DMA_CHANNEL;
  // ��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(MMA7361L_ADC_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  DMA_Cmd(MMA7361L_ADC_DMA_STREAM, ENABLE);
	
	
	// ����ADCʱ��
	RCC_APB2PeriphClockCmd(MMA7361L_ADC1_CLK , ENABLE);
  RCC_APB2PeriphClockCmd(MMA7361L_ADC2_CLK , ENABLE);
	RCC_APB2PeriphClockCmd(MMA7361L_ADC3_CLK , ENABLE);
	
  // -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
  ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_RegSimult;
  // ʱ��Ϊfpclk x��Ƶ	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // ��ֹDMAֱ�ӷ���ģʽ	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
  // ����ʱ����	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
	ADC_StructInit(&ADC_InitStructure);
  // ADC �ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // ��ֹɨ��ģʽ����ͨ���ɼ�����Ҫ	
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
  // ����ת��	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  // ��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  // �ⲿ����ͨ����������ʹ�������������ֵ��㸳ֵ����
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  // �����Ҷ���	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  // ת��ͨ�� 1��
  ADC_InitStructure.ADC_NbrOfConversion = 1;
	
  //---------------------------------------------------------------------------	
  ADC_Init(MMA7361L_ADC1, &ADC_InitStructure);	
   // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
  ADC_RegularChannelConfig(MMA7361L_ADC1, MMA7361L_ADC1_CHANNEL, 1, ADC_SampleTime_3Cycles);   
  //---------------------------------------------------------------------------
	
	ADC_Init(MMA7361L_ADC2, &ADC_InitStructure);
  // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
  ADC_RegularChannelConfig(MMA7361L_ADC2, MMA7361L_ADC2_CHANNEL, 1, ADC_SampleTime_3Cycles);   
  //---------------------------------------------------------------------------
	
	ADC_Init(MMA7361L_ADC3, &ADC_InitStructure);
  // ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
  ADC_RegularChannelConfig(MMA7361L_ADC3, MMA7361L_ADC3_CHANNEL, 1, ADC_SampleTime_3Cycles);   
  //---------------------------------------------------------------------------	
		
  // ʹ��DMA���� after last transfer (multi-ADC mode)
  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
	// ʹ��ADC DMA
  ADC_DMACmd(MMA7361L_ADC1, ENABLE);
	
  // ʹ��ADC
  ADC_Cmd(MMA7361L_ADC1, ENABLE);  
  ADC_Cmd(MMA7361L_ADC2, ENABLE);
	ADC_Cmd(MMA7361L_ADC3, ENABLE);	
  
  //��ʼadcת�����������
  ADC_SoftwareStartConv(MMA7361L_ADC1);
  ADC_SoftwareStartConv(MMA7361L_ADC2);
	ADC_SoftwareStartConv(MMA7361L_ADC3);
}


void MMA7361L_Init(void)
{
	MMA7361L_ADC_GPIO_Config();
	MMA7361L_ADC_Mode_Config();
}



