/**
  ******************************************************************************
  * @file    bsp_rtc.c
  * @author  phoenix
  * @version V1.0
  * @date    29-January-2018
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
#include "./rtc/bsp_rtc.h"
#include "./usart/bsp_com_usart.h"

/**
  * @brief  ����ʱ�������
  * @param  ��
  * @retval ��
  */
void RTC_TimeAndDate_Set(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	// ��ʼ��ʱ��
	RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;
	RTC_TimeStructure.RTC_Hours = HOURS;        
	RTC_TimeStructure.RTC_Minutes = MINUTES;      
	RTC_TimeStructure.RTC_Seconds = SECONDS;      
	RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
  // ��ʼ������	
	RTC_DateStructure.RTC_WeekDay = WEEKDAY;       
	RTC_DateStructure.RTC_Date = DATE;         
	RTC_DateStructure.RTC_Month = MONTH;         
	RTC_DateStructure.RTC_Year = YEAR;        
	RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure);
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
}

/**
  * @brief  ����ʱ�������
  * @param  �꣬�£��գ����ڣ�ʱ���֣���
  * @retval ��
  */
void RTC_TimeAndDate_Reset(uint8_t year , uint8_t month, uint8_t date, uint8_t weekday, uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	// ��ʼ��ʱ��
	RTC_TimeStructure.RTC_H12 = RTC_H12_AMorPM;
	RTC_TimeStructure.RTC_Hours = hours;
	RTC_TimeStructure.RTC_Minutes = minutes;
	RTC_TimeStructure.RTC_Seconds = seconds;
	if (RTC_SetTime(RTC_Format_BINorBCD, &RTC_TimeStructure) == ERROR)
	{
		printf("RTC SetTime configure error\r\n");
	}
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	
  // ��ʼ������
	RTC_DateStructure.RTC_WeekDay = weekday;
	RTC_DateStructure.RTC_Date = date;
	RTC_DateStructure.RTC_Month = month;
	RTC_DateStructure.RTC_Year = year;
	if (RTC_SetDate(RTC_Format_BINorBCD, &RTC_DateStructure) == ERROR)
	{
		printf("RTC SetDate configure error\r\n");
	}
	RTC_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
}

/**
  * @brief  ��ȡʱ�������
  * @param  ��
  * @retval ��
  */
void RTC_TimeAndDate_Access(RTC_TimeTypeDef*  RTC_TimeStructure, RTC_DateTypeDef* RTC_DateStructure)
{
	// ��ȡ����
	RTC_GetTime(RTC_Format_BIN, RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, RTC_DateStructure);
}

/**
  * @brief  ��ʾʱ�������
  * @param  ��
  * @retval ��
  */
void RTC_TimeAndDate_Show(void)
{
	uint8_t Rtctmp=0;
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	
	while(1)
	{
		// ��ȡ����
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
		
		// ÿ���ӡһ��
  	if(Rtctmp != RTC_TimeStructure.RTC_Seconds)
    {
			// ��ӡ����
      printf("The Date:Y:20%0.2d - M:%0.2d - D:%0.2d - W:%0.2d\r\n", 
			RTC_DateStructure.RTC_Year,
			RTC_DateStructure.RTC_Month, 
			RTC_DateStructure.RTC_Date,
			RTC_DateStructure.RTC_WeekDay);
			// ��ӡʱ��
      printf("The Time:%0.2d:%0.2d:%0.2d\r\n", 
			RTC_TimeStructure.RTC_Hours, 
			RTC_TimeStructure.RTC_Minutes, 
			RTC_TimeStructure.RTC_Seconds);
      (void)RTC->DR;
    }
    Rtctmp = RTC_TimeStructure.RTC_Seconds;
	}	
}

/**
  * @brief  RTC���ã�ѡ��RTCʱ��Դ������RTC_CLK�ķ�Ƶϵ��
  * @param  ��
  * @retval ��
  */
void RTC_Config(void)
{  
	RTC_InitTypeDef RTC_InitStructure;
	
	/* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
  PWR_BackupAccessCmd(ENABLE);
	/* Reset RTC Domain */
//	RCC_BackupResetCmd(ENABLE);
//  RCC_BackupResetCmd(DISABLE);

#if defined (RTC_CLOCK_SOURCE_LSI) 
  /* ʹ��LSI��ΪRTCʱ��Դ�������
	 * Ĭ��ѡ��LSE��ΪRTC��ʱ��Դ
	 */
  /* Enable the LSI OSC */
  RCC_LSICmd(ENABLE);
  /* Wait till LSI is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET){}
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

#elif defined (RTC_CLOCK_SOURCE_LSE)

  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);    

#endif /* RTC_CLOCK_SOURCE_LSI */

  /* ʹ��RTCʱ�� */
  RCC_RTCCLKCmd(ENABLE);

  /* �ȴ� RTC APB �Ĵ���ͬ�� */
  RTC_WaitForSynchro();
   
  /* Configure the RTC data register and RTC prescaler */
  /* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
	/* �����첽Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_AsynchPrediv = ASYNCHPREDIV;
	/* ����ͬ��Ԥ��Ƶ����ֵ */
	RTC_InitStructure.RTC_SynchPrediv = SYNCHPREDIV;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24; 
	/* ��RTC_InitStructure�����ݳ�ʼ��RTC�Ĵ��� */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		printf("RTC configure error\r\n");
	}
}

/**
  * @brief  RTC���ü�飺 ���������ù�RTCʱ��֮��������ݼĴ���0д��һ�����������
	* 																						����ÿ�γ����������е�ʱ���ͨ����ⱸ�ݼĴ���0��ֵ���ж�
	* 																						RTC �Ƿ��Ѿ����ù���������ù��Ǿͼ������У����û�����ù�
	* 																						�ͳ�ʼ��RTC������RTC��ʱ��
  * @param  ��
  * @retval ��
  */
void RTC_Config_Check(void)
{
	if (RTC_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
  {
    /* ����ʱ������� */
		RTC_TimeAndDate_Set();
  }
  else
  {
    /* ����Ƿ��Դ��λ */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("power reset\r\n");
    }
    /* ����Ƿ��ⲿ��λ */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("extern reset\r\n");
    }
    printf("no need to configure RTC\r\n");
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    /* PWR_CR:DBF��1��ʹ��RTC��RTC���ݼĴ����ͱ���SRAM�ķ��� */
    PWR_BackupAccessCmd(ENABLE);
    /* �ȴ� RTC APB �Ĵ���ͬ�� */
    RTC_WaitForSynchro();   
  } 
}


/**********************************END OF FILE*************************************/
