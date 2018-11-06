#ifndef __FUNCTION_H
#define __FUNCTION_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "./usart/bsp_com_usart.h"
#include "./tim/bsp_basic_tim.h"
#include "./gprs/bsp_gprs.h"
#include "./Modbus_Master/modbus_master.h"
#include "trans_recieve_buff_control.h"

//#define	ETHERNET_ENABLE

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void System_Initial(void);
void GPIO_Config(void);
void SysTick_Init(void);
void IncTick(void);
uint32_t GetTick(void);
void HAL_Delay(__IO uint32_t Delay);
void GPRS_Rece_Handler(void);

#define LED_RUN_PIN     		GPIO_Pin_4
#define LED_STA_PIN      		GPIO_Pin_3
#define LED_COM_PIN        	GPIO_Pin_2
#define	LED_GPIO_PORT       GPIOE
#define	LED_GPIO_CLK        RCC_AHB1Periph_GPIOE

// LED操作函数，(ON)打开, (OFF)关闭，(TOG)翻转
#define LED_RUN_ON()   			GPIO_ResetBits(LED_GPIO_PORT,LED_RUN_PIN)        
#define LED_RUN_OFF()    		GPIO_SetBits(LED_GPIO_PORT,LED_RUN_PIN)
#define LED_RUN_TOG()   		GPIO_ToggleBits(LED_GPIO_PORT,LED_RUN_PIN)

#define LED_STA_ON()    		GPIO_ResetBits(LED_GPIO_PORT,LED_STA_PIN)        
#define LED_STA_OFF()     	GPIO_SetBits(LED_GPIO_PORT,LED_STA_PIN)
#define LED_STA_TOG()    		GPIO_ToggleBits(LED_GPIO_PORT,LED_STA_PIN)

#define LED_COM_ON()      	GPIO_ResetBits(LED_GPIO_PORT,LED_COM_PIN)        
#define LED_COM_OFF()      	GPIO_SetBits(LED_GPIO_PORT,LED_COM_PIN)
#define LED_COM_TOG()      	GPIO_ToggleBits(LED_GPIO_PORT,LED_COM_PIN)

#endif /* __FUNCTION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
