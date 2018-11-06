 /**
  ******************************************************************************
  * @file    bsp_COM1_usart.c
  * @author  phoenix
  * @version V1.0.0
  * @date    23-October-2017
  * @brief   �ض���c��printf������usart�˿ڣ��жϽ���ģʽ
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_com_usart.h"
#include "./gprs/bsp_gprs.h"

uint8_t aTxBuffer[BUFFERSIZE] = "USART DMA Example: Communication between two USART using DMA";
uint8_t aRxBuffer [BUFFERSIZE];

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
void NVIC_Configuration(uint8_t NVIC_IRQChannel, uint8_t NVIC_IRQChannelPreemptionPriority, uint8_t NVIC_IRQChannelSubPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
  
    /* Ƕ�������жϿ�������ѡ�� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    /* ����USARTΪ�ж�Դ */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC_IRQChannel;
    /* �������ȼ�Ϊ1 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;
    /* �����ȼ�Ϊ1 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;
    /* ʹ���ж� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* ��ʼ������NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  Configures the USART Peripheral.
  * @param  None
  * @retval None
  */
void COM1_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(COM1_USART_RX_GPIO_CLK | COM1_USART_TX_GPIO_CLK,ENABLE);

	/* Enable USART clock */
	RCC_APB1PeriphClockCmd(COM1_USART_CLK, ENABLE);

  /* USARTx GPIO configuration -----------------------------------------------*/   
  /* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = COM1_USART_TX_PIN  ;  
	GPIO_Init(COM1_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = COM1_USART_RX_PIN;
	GPIO_Init(COM1_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
  /* Connect USART pins to AF */
	GPIO_PinAFConfig(COM1_USART_RX_GPIO_PORT, COM1_USART_RX_SOURCE, COM1_USART_RX_AF);
	GPIO_PinAFConfig(COM1_USART_TX_GPIO_PORT, COM1_USART_TX_SOURCE, COM1_USART_TX_AF);

  /* USARTx configuration ----------------------------------------------------*/
  /* USARTx configured as follows:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - one Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = COM1_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* When using Parity the word length must be configured to 9 bits */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM1_USART, &USART_InitStructure); 
	
	/* ʹ�ܴ��ڽ����ж� */
//	USART_ITConfig(COM1_USART, USART_IT_TC, DISABLE);	
//	USART_ITConfig(COM1_USART, USART_IT_RXNE, DISABLE);
//	USART_ITConfig(COM1_USART, USART_IT_TXE, DISABLE);
//	USART_ITConfig(COM1_USART, USART_IT_IDLE, ENABLE);
	
	/* Ƕ�������жϿ�����NVIC���� */
//	NVIC_Configuration(COM1_USART_IRQ,0,0);
	/* ʹ�ܴ��ڽ����ж� */
//	USART_ITConfig(COM1_USART, USART_IT_RXNE, ENABLE);	
	
  /* Enable USART DMA TX Requsts */
//	USART_DMACmd(COM1_USART,USART_DMAReq_Tx,ENABLE);  
  /* Enable USART DMA RX Requsts */
//	USART_DMACmd(COM1_USART,USART_DMAReq_Rx,ENABLE); 
	/* Enable USART */
	USART_Cmd(COM1_USART, ENABLE);
}

void COM1_DMA_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	/* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd(USART5_DMAx_CLK, ENABLE);
	DMA_DeInit(USART5_TX_DMA_STREAM);
	DMA_DeInit(USART5_RX_DMA_STREAM);
	
  /* Configure DMA controller to manage USART TX and RX DMA request ----------*/ 
  /* Configure DMA Initialization Structure */
  DMA_InitStructure.DMA_BufferSize = BUFFERSIZE ;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(COM1_USART->DR)) ;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  /* Configure TX DMA */
  DMA_InitStructure.DMA_Channel = USART5_TX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aTxBuffer ;
  DMA_Init(USART5_TX_DMA_STREAM,&DMA_InitStructure);
  /* Configure RX DMA */
  DMA_InitStructure.DMA_Channel = USART5_RX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aRxBuffer ; 
  DMA_Init(USART5_RX_DMA_STREAM,&DMA_InitStructure);
	
	DMA_ITConfig(USART5_TX_DMA_STREAM, DMA_IT_TC, ENABLE);
	/* Enable DMA USART TX Stream */
  DMA_Cmd(USART5_TX_DMA_STREAM,ENABLE);
	NVIC_Configuration(USART5_DMA_TX_IRQn,2,0);

//	DMA_ITConfig(USART5_RX_DMA_STREAM, DMA_IT_HT, ENABLE);
	/* Enable DMA USART RX Stream */
  DMA_Cmd(USART5_RX_DMA_STREAM,ENABLE);
	NVIC_Configuration(USART5_DMA_RX_IRQn,3,0);	
	
}

 /**
  * @brief  Configures the USART Peripheral.
  * @param  None
  * @retval None
  */
void COM2_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(COM2_USART_RX_GPIO_CLK | COM2_USART_TX_GPIO_CLK,ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(COM2_USART_CLK, ENABLE);
  
  /* USARTx GPIO configuration -----------------------------------------------*/   
  /* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = COM2_USART_TX_PIN  ;  
	GPIO_Init(COM2_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = COM2_USART_RX_PIN;
	GPIO_Init(COM2_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
	/* Connect USART pins to AF */
	GPIO_PinAFConfig(COM2_USART_RX_GPIO_PORT, COM2_USART_RX_SOURCE, COM2_USART_RX_AF);
	GPIO_PinAFConfig(COM2_USART_TX_GPIO_PORT, COM2_USART_TX_SOURCE, COM2_USART_TX_AF);
  
  /* USARTx configuration ----------------------------------------------------*/
  /* USARTx configured as follows:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - one Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = COM2_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* When using Parity the word length must be configured to 9 bits */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(COM2_USART, &USART_InitStructure); 
	
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration(COM2_USART_IRQ,0,1);
  
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(COM2_USART, USART_IT_RXNE, ENABLE);
	
	/* Enable USART */
	USART_Cmd(COM2_USART, ENABLE);
}

void RS485_DIR1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    /* Enable GPIOD clock */
    RCC_AHB1PeriphClockCmd(RS485_DIR1_CLK, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = RS485_DIR1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RS485_DIR1_PORT, &GPIO_InitStructure);

		RS485_DIR1_ON();
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx, ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  �����ַ��� **********************/
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte(pUSARTx, *(str + k));
      k++;
  } while(*(str+k) != '\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET)
  {}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	/* ȡ���߰�λ */
	temp_h = (ch&0XFF00)>>8;
	/* ȡ���Ͱ�λ */
	temp_l = ch&0XFF;
	/* ���͸߰�λ */
	USART_SendData(pUSARTx, temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx, temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(MOD_USART, (uint8_t)ch);
	/* �ȴ�������� */
	while (USART_GetFlagStatus(MOD_USART, USART_FLAG_TXE) == RESET);		
	return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(MOD_USART, USART_FLAG_RXNE) == RESET);
	return (int)USART_ReceiveData(MOD_USART);
}

//����һ���ֽ�
uint8_t Usart_RecByte(USART_TypeDef *pUSARTx)
{
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_RXNE) == RESET);
	return (uint8_t)USART_ReceiveData(pUSARTx);
}

//���������ֽ�
uint16_t Usart_RecHalfWord(USART_TypeDef *pUSARTx)
{
	uint8_t temp_h, temp_l;
	/* ���յ�һ��byte */
	temp_h = Usart_RecByte(pUSARTx);
	/* ���յڶ���byte */
	temp_l = Usart_RecByte(pUSARTx);
	return ((uint16_t)(0xFF00 & temp_h<<8)+(uint16_t)(0x00FF & temp_l));
}

//�����ĸ��ֽ�
uint32_t Usart_RecWord(USART_TypeDef *pUSARTx)
{
	uint16_t temp_h, temp_l;
	/* ���յ�һ��HalfWord */
	temp_h = Usart_RecHalfWord(pUSARTx);
	/* ���յڶ���HalfWord */
	temp_l = Usart_RecHalfWord(pUSARTx);
	return ((uint32_t)(0xFFFF0000 & temp_h<<16)+(uint32_t)(0x0000FFFF & temp_l));
}

//��������
void Usart_RecArray(USART_TypeDef *pUSARTx, uint8_t *arr)
{
	unsigned int k;
  for(k=0;k<PCCOMMAND_LENGTH;k++)
  {
			*(arr+k) = Usart_RecByte(pUSARTx);
			if(k>1)
			{
				if(*(arr+k-1) == 0x0d && *(arr+k) == 0x0a)
				{break;}
			}
  }
}
/*********************************************END OF FILE**********************/
