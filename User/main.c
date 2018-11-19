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
uint16_t Analog_Result[2];
uint8_t pt03_result;
uint8_t di_result;
uint8_t ai_result;
extern uint8_t PCCommand[PCCOMMAND_LENGTH];	// 接收上位机命令数组
uint16_t baseaddr;
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
		switch( state )
		{
			case 0:
				pt03_result = ModbusMaster_readInputRegisters(PCCommand[2],0x0000,0x0003);//使用Read Input Registers功能，读取温度采集器数据，从机地址0xFE，寄存器地址0x0000，连续读3个温度
				if (pt03_result == 0x00)
				{
					Tamperature_Result[0] = ModbusMaster_getResponseBuffer(0x00);
					Tamperature_Result[1] = ModbusMaster_getResponseBuffer(0x01);
					Tamperature_Result[2] = ModbusMaster_getResponseBuffer(0x02);
				}
				HAL_Delay(100);
				di_result = ModbusMaster_readDiscreteInputs(PCCommand[3],0x0000,0x0002);//使用Read Discrete Input功能，读取接近开关状态，从机地址0xFE，寄存器地址0x00，读2个bit
				if (di_result == 0x00)
				{
					Switch_Result = ModbusMaster_getResponseBuffer(0x00);
				}
				HAL_Delay(100);
				ai_result = ModbusMaster_readInputRegisters(PCCommand[3],0x0000,0x0002);//使用Read Input Registers功能，读取模拟量，从机地址0xFE，寄存器地址0x00，读2个模拟量数量
				if (ai_result == 0x00)
				{
					Analog_Result[0] = ModbusMaster_getResponseBuffer(0x00);
					Analog_Result[1] = ModbusMaster_getResponseBuffer(0x01);
				}
				break;
			case 1:
				Read_BaseAddr(PCCommand[2]);
				Send_Read_Address(PCCommand[2]);
				state = 0;
				LED_COM_OFF();
				break;
			case 2:
				Read_BaseAddr(PCCommand[3]);
				Send_Read_Address(PCCommand[3]);
				state = 0;
				LED_COM_OFF();
				break;
			case 3:
				baseaddr = (uint16_t)(0xFF00 & PCCommand[4]<<8)+(uint16_t)(0x00FF & PCCommand[5]);
				Set_BaseAddr(PCCommand[2],baseaddr);
				Read_BaseAddr(PCCommand[5]);
				Send_Set_Address(PCCommand[2]);
				state = 0;
				LED_COM_OFF();
				break;
			case 4:
				baseaddr = (uint16_t)(0xFF00 & PCCommand[6]<<8)+(uint16_t)(0x00FF & PCCommand[7]);
				Set_BaseAddr(PCCommand[3],baseaddr);
				Read_BaseAddr(PCCommand[7]);
				Send_Set_Address(PCCommand[3]);
				state = 0;
				LED_COM_OFF();
				break;
		}
		HAL_Delay(1000);
  }
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
