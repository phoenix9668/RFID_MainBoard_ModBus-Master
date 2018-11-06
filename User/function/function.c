/**
  ******************************************************************************
  * @file    function.c 
  * @author  phoenix
  * @version V1.0.0
  * @date    25-September-2017
  * @brief   function program body
  ******************************************************************************
  * @attention
  *
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "./function/function.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
__IO uint32_t uwTick;
#define HAL_MAX_DELAY      0xFFFFFFFFU
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t i;
uint8_t PCCommand[PCCOMMAND_LENGTH];	// 接收上位机命令数组
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*===========================================================================
* 函数: System_Initial() => 初始化系统所有外设                             	*
============================================================================*/
void System_Initial(void)
{
		GPIO_Config();         			// 初始化GPIO
		RS485_DIR1_Config();
		MOD_GPRS_Config();					// 初始化GPRS	
		SysTick_Init();
//		COM1_USART_Config();  			// 初始化串口
		COM2_USART_Config();
		MOD_USART_Config();
    TIMx_Configuration();   		// 初始化定时器6，0.5s 一个事件
}

/**
  * @brief  GPIO_Initial function
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    /* Enable GPIOE clock */
    RCC_AHB1PeriphClockCmd(LED_GPIO_CLK, ENABLE);
	
    //configure LED3 GPIO
    GPIO_InitStructure.GPIO_Pin = LED_RUN_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
    
    //configure LED4 GPIO
    GPIO_InitStructure.GPIO_Pin = LED_STA_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

    //configure LED5 GPIO
    GPIO_InitStructure.GPIO_Pin = LED_COM_PIN;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

		LED_RUN_OFF();
		LED_STA_OFF();
		LED_COM_OFF();
}

void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		/* Capture error */ 
		while (1);
	}
}

/**
  * @brief This function is called to increment  a global variable "uwTick"
  *        used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *       in Systick ISR.
  * @note This function is declared as __weak to be overwritten in case of other 
  *       implementations in user file.
  * @retval None
  */
void IncTick(void)
{
  uwTick++;
}

/**
  * @brief  Provides a tick value in millisecond.
  * @note   This function is declared as __weak  to be overwritten  in case of other 
  *       implementations in user file.
  * @retval tick value
  */
uint32_t GetTick(void)
{
  return uwTick;
}

/**
  * @brief This function provides accurate delay (in milliseconds) based 
  *        on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *       It is used to generate interrupts at regular time intervals where uwTick
  *       is incremented.
  * @note ThiS function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @param Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
void HAL_Delay(__IO uint32_t Delay)
{
  uint32_t tickstart = GetTick();
  uint32_t wait = Delay;
  
  /* Add a period to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
     wait++;
  }
  
  while((GetTick() - tickstart) < wait)
  {
  }
}

/**
  * @brief USART3_IRQHandler detection callback.
  * @param None
  * @retval None
  */
void GPRS_Rece_Handler()
{
	for (i=0; i<PCCOMMAND_LENGTH; i++) // clear array
		{PCCommand[i] = 0;}
	Usart_RecArray(MOD_USART, PCCommand);/* 等待串口接收数据完毕 */
	for(i=0; i<PCCOMMAND_LENGTH; i++)// for test
	{
		printf("%x ",PCCommand[i]);
	}
	printf("\n");
	if(PCCommand[0] == 0xAB && PCCommand[1] == 0xCD)//begin index
	{
		Usart_SendString(MOD_USART,"start transfer\n");
		LED_STA_ON();
	}
	else if(PCCommand[0] == 0xE5 && PCCommand[1] == 0x5E)
	{
		Usart_SendString(MOD_USART,"fram transfer\n");
		LED_STA_OFF();
	}
}
	
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
