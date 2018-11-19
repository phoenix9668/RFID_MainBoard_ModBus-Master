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
uint8_t result_read;
uint8_t result_write;
uint8_t PCCommand[PCCOMMAND_LENGTH];	// 接收上位机命令数组
uint8_t SendBuffer[SEND_LENGTH] = {0};// 发送数据包
extern uint8_t state;
extern uint16_t Tamperature_Result[3];
extern uint16_t Switch_Result;
extern uint16_t Analog_Result[2];
extern uint8_t pt03_result;
extern uint8_t di_result;
extern uint8_t ai_result;
uint16_t BaseAddr_Result;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  System_Initial() => 初始化系统所有外设
  * @param  None
  * @retval None
  */
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
void GPRS_Rece_Handler(void)
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
		Usart_SendString(MOD_USART,"read data\n");
		LED_STA_ON();
		Send_Data();
		LED_STA_OFF();
	}
	else if(PCCommand[0] == 0xE5 && PCCommand[1] == 0x5E)
	{
		Usart_SendString(MOD_USART,"read base address 1\n");
		LED_COM_ON();
		state = 1;
	}
	else if(PCCommand[0] == 0xE5 && PCCommand[1] == 0xAE)
	{
		Usart_SendString(MOD_USART,"read base address 2\n");
		LED_COM_ON();
		state = 2;
	}	
	else if(PCCommand[0] == 0xE5 && PCCommand[1] == 0xA5)
	{
		Usart_SendString(MOD_USART,"configure base address 1\n");
		LED_COM_ON();
		state = 3;
	}
	else if(PCCommand[0] == 0xE5 && PCCommand[1] == 0xAA)
	{
		Usart_SendString(MOD_USART,"configure base address 2\n");
		LED_COM_ON();
		state = 4;
	}
}

/**
  * @brief Read_BaseAddr(uint8_t SlaveID)
  * @param uint8_t SlaveID
  * @retval None
  */
void Read_BaseAddr(uint8_t SlaveID)
{
		//使用Read Holding Registers功能，读设备基地址
		//从机地址0xFE，寄存器地址0x03EA，查询1个数量
		result_read = ModbusMaster_readHoldingRegisters(SlaveID,0x03EA,0x0001);
		if (result_read == 0x00)
		{
			BaseAddr_Result = ModbusMaster_getResponseBuffer(0x00);
		}
}

/**
  * @brief Set_BaseAddr(uint8_t SlaveID,uint16_t u16Value)
  * @param uint8_t SlaveID
  * @retval None
  */
void Set_BaseAddr(uint8_t SlaveID,uint16_t u16Value)
{
		//使用Write Multiple Registers功能，设置设备基地址
		//从机地址0xFE，寄存器地址0x03EA，写1个数量
		ModbusMaster_clearTransmitBuffer();
		ModbusMaster_setTransmitBuffer(0,u16Value);
		result_write = ModbusMaster_writeMultipleRegisters(SlaveID,0x03EA,0x0001);
}

/**
  * @brief Send_Data.
  * @param None
  * @retval None
  */
void Send_Data(void)
{
	
	for (i=0; i<SEND_LENGTH; i++) // clear array
		{SendBuffer[i] = 0;}
	
		SendBuffer[0] = 0xAB;
		SendBuffer[1] = 0xCD;
		SendBuffer[2] = pt03_result;
		SendBuffer[3] = di_result;
		SendBuffer[4] = ai_result;
		SendBuffer[5] = (uint8_t)(0xFF & Tamperature_Result[0]>>8);
		SendBuffer[6] = (uint8_t)(0xFF & Tamperature_Result[0]);
		SendBuffer[7] = (uint8_t)(0xFF & Tamperature_Result[1]>>8);
		SendBuffer[8] = (uint8_t)(0xFF & Tamperature_Result[1]);
		SendBuffer[9] = (uint8_t)(0xFF & Tamperature_Result[2]>>8);
		SendBuffer[10] = (uint8_t)(0xFF & Tamperature_Result[2]);
		SendBuffer[11] = (uint8_t)(0xFF & Switch_Result>>8);
		SendBuffer[12] = (uint8_t)(0xFF & Switch_Result);
		SendBuffer[13] = (uint8_t)(0xFF & Analog_Result[0]>>8);
		SendBuffer[14] = (uint8_t)(0xFF & Analog_Result[0]);
		SendBuffer[15] = (uint8_t)(0xFF & Analog_Result[1]>>8);
		SendBuffer[16] = (uint8_t)(0xFF & Analog_Result[1]);
	for(i=0; i<SEND_LENGTH; i++)
		{
			printf("%x ",SendBuffer[i]);
		}
	printf("\n");		
}

/**
  * @brief Send_Read_Address.
  * @param None
  * @retval None
  */
void Send_Read_Address(uint8_t SlaveID)
{
	
	for (i=0; i<SEND_LENGTH; i++) // clear array
		{SendBuffer[i] = 0;}
	
		SendBuffer[0] = 0xE5;
		SendBuffer[1] = 0x5E;
		SendBuffer[2] = result_read;
		SendBuffer[3] = SlaveID;
		SendBuffer[4] = (uint8_t)(0xFF & BaseAddr_Result>>8);
		SendBuffer[5] = (uint8_t)(0xFF & BaseAddr_Result);
	for(i=0; i<SEND_LENGTH; i++)
		{
			printf("%x ",SendBuffer[i]);
		}
	printf("\n");		
}

/**
  * @brief Send_Set_Address.
  * @param None
  * @retval None
  */
void Send_Set_Address(uint8_t SlaveID)
{
	
	for (i=0; i<SEND_LENGTH; i++) // clear array
		{SendBuffer[i] = 0;}
	
		SendBuffer[0] = 0xE5;
		SendBuffer[1] = 0xA5;
		SendBuffer[2] = result_write;
		SendBuffer[3] = result_read;
		SendBuffer[4] = SlaveID;
		SendBuffer[5] = (uint8_t)(0xFF & BaseAddr_Result>>8);
		SendBuffer[6] = (uint8_t)(0xFF & BaseAddr_Result);
	for(i=0; i<SEND_LENGTH; i++)
		{
			printf("%x ",SendBuffer[i]);
		}
	printf("\n");		
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
