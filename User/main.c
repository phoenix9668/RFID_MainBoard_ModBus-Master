/**
  ******************************************************************************
  * @file    main.c 
  * @author  phoenix
  * @version V1.0.0
  * @date    20-October-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
uint8_t state=0;
uint16_t Tamperature_Result[3];
uint16_t Switch_Result;
uint8_t result;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*===========================================================================
* 函数 : main() => 主函数，程序入口                                         *
* 说明 ：每1s发送一包数据，每包数据长度为10个字节，数据内容为0-9            *
         接收方反馈(应答)的数据长度为10个字节，数据内容为10-19              *
============================================================================*/
int main(void)
{
	Delay(0xffffff);
	System_Initial();
	ModbusMaster_begin();
	#ifdef UART_DEBUG
		Show_Message();
	#endif

  while (1)
  {
		//使用Read Input Registers功能，读取温度采集器数据
		//从机地址0xFE，寄存器地址0x00，连续读3个温度
		result = ModbusMaster_readInputRegisters(0xFE,0x00,0x03);
		if (result == 0x00)
		{
			Tamperature_Result[0] = ModbusMaster_getResponseBuffer(0x00);
			Tamperature_Result[1] = ModbusMaster_getResponseBuffer(0x01);
			Tamperature_Result[2] = ModbusMaster_getResponseBuffer(0x02);
		}
		//使用Read Discrete Input功能，读取接近开关状态
		//从机地址0xFE，寄存器地址0x00，读1个字节
		result = ModbusMaster_readDiscreteInputs(0xFE,0x00,0x01);
		if (result == 0x00)
		{
			Switch_Result = ModbusMaster_getResponseBuffer(0x00);
		}
			HAL_Delay(1000);
  }

//	while(1)
//	{
//		for (i=0; i<PCCOMMAND_LENGTH; i++) // clear array
//			{PCCommand[i] = 0;}
//		Usart_RecArray(COM2_USART, PCCommand);/* 等待串口接收数据完毕 */
//		for(i=0; i<PCCOMMAND_LENGTH; i++)// for test
//		{
//			printf("%x ",PCCommand[i]);
//		}
//		printf("\n");
//		if(PCCommand[0] == 0xAB && PCCommand[1] == 0xCD)//begin index
//		{
//			Usart_SendString(COM2_USART,"start transfer\n");
//			LED_STA_ON();
//		}
//		else if(PCCommand[0] == 0xE5 && PCCommand[1] == 0x5E)
//		{
//			Usart_SendString(COM2_USART,"fram transfer\n");
//			LED_STA_OFF();
//		}
//	}
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

#ifdef UART_DEBUG
/**
  * @brief  打印指令输入提示信息
  * @param  无
  * @retval 无
  */
static void Show_Message(void)
{
	printf("\r\nmain program running\n");
	printf("using USART6,configuration:%d 8-N-1\n",COM2_USART_BAUDRATE);
}
#endif

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
