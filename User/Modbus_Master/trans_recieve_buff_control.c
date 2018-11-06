#include "trans_recieve_buff_control.h"
#include "ringbuffer.h"
#include "./usart/bsp_com_usart.h"

RingBuffer  m_Modbus_Master_RX_RingBuff;
uint8_t     m_Modbus_Master_RX_Buff[200];

extern uint32_t GetTick(void);
/**
  * @brief  ��ʼ���жϽ��յĵ�ringbuffer���ζ�������,�жϽ��յ��ֽڶ���m_Modbus_Master_RX_RingBuff�ýṹ��ָ����й���
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_RB_Initialize(void)
{
	/*��ʼ��ringbuffer��ص�����*/
	rbInitialize(&m_Modbus_Master_RX_RingBuff, m_Modbus_Master_RX_Buff, sizeof(m_Modbus_Master_RX_Buff));
	return 1 ;
}


/**
  * @brief  ������ζ���
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
void Modbus_Master_Rece_Flush(void)
{
  rbClear(&m_Modbus_Master_RX_RingBuff);
}
/**
  * @brief  �ж�GPS��ringbuffer�����Ƿ�����δ������ֽ�
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Rece_Available(void)
{
	/*������ݰ�buffer��������ˣ������㣬���¼���*/
		if(m_Modbus_Master_RX_RingBuff.flagOverflow==1)
		{
			rbClear(&m_Modbus_Master_RX_RingBuff);
		}
	return !rbIsEmpty(&m_Modbus_Master_RX_RingBuff);
}

/****************************************************************************************************/
/*��������Ӳ���ӿڲ�������Ĳ��֣����ݲ�ͬ�������Ĵ���ʽ������ֲ*/
/**
  * @brief  ��ȡ���ռĴ����������ֵ
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
void Modbus_Master_GetByte(uint8_t *getbyte)
{
		*getbyte = Usart_RecByte(COM2_USART);
//  if(HAL_UART_Receive (&huart2 ,(uint8_t *)getbyte,1,0x01) != HAL_OK )
//  {
//		return HAL_ERROR;
//	}
//	else
//	{
//	  return HAL_OK;
//	}
}

/**
  * @brief  �жϴ��������ڴ��ڽ����ж��е��ã����Ĵ�������ֵѹ�˻�����
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
void Modbus_Master_Rece_Handler(void)
{
		uint8_t byte;
		//��ȡ�Ĵ�����������ݣ����ҽ�����ѹ�뻷�ζ���
		Modbus_Master_GetByte(&byte);
		rbPush(&m_Modbus_Master_RX_RingBuff, (uint8_t)(byte & (uint8_t)0xFFU));
}

/**
  * @brief  ����������������
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint8_t Modbus_Master_Read(void)
{
	uint8_t cur =0xff;
	if( !rbIsEmpty(&m_Modbus_Master_RX_RingBuff))
	{
		  cur = rbPop(&m_Modbus_Master_RX_RingBuff);
	}
	return cur;
}

/**
  * @brief  �����ݰ����ͳ�ȥ
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
void Modbus_Master_Write(uint8_t *buf,uint8_t length)
{
	uint8_t i;
//	RS485_DIR1_ON();
	for(i=0;i<length;i++)
	{
		USART_SendData(COM2_USART, *(buf + i));	
		while (USART_GetFlagStatus(COM2_USART, USART_FLAG_TXE) == RESET);
	}
//	RS485_DIR1_OFF();
// if(HAL_UART_Transmit(&huart2 ,(uint8_t *)buf,length,0xff))
// {
//   return HAL_ERROR;
// }
// 	else
//	{
//	  return HAL_OK;
//	}
}

/**
  * @brief  1ms���ڵĶ�ʱ��
  * @param
  * @note
  * @retval void
  * @author xiaodaqi
  */
uint32_t Modbus_Master_Millis(void)
{
  return GetTick();
}
