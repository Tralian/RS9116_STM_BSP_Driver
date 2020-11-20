/**
  ******************************************************************************
  * @file    bsp_uart.h 
  * @author  YZTek Software TEAM
  * @brief   Header for bsp_uart.h module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020  YZTek</center></h2>
  *
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_UART_H
#define __BSP_UART_H

#ifdef	USE_UART
/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include <stdbool.h>
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Definition for UARTx and clock */
#define UARTx														USART2	
#define UART_BAUD												115200
#define	UARTx_TX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define	UARTx_RX_GPIO_CLK_ENABLE()			__HAL_RCC_GPIOA_CLK_ENABLE()
#define UARTx_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
#define UARTx_CLK_DISABLE()            	__HAL_RCC_USART2_CLK_DISABLE()
#define UARTx_DMA_CLK_ENABLE()          __HAL_RCC_DMA1_CLK_ENABLE()
#define UARTx_DMA_CLK_DISABLE()         __HAL_RCC_DMA1_CLK_DISABLE()
#define UARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define UARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define UARTx_TX_PIN		                GPIO_PIN_2
#define UARTx_TX_GPIO_PORT              GPIOA
#define UARTx_TX_AF                     GPIO_AF4_USART2
#define UARTx_RX_PIN                    GPIO_PIN_15
#define UARTx_RX_GPIO_PORT              GPIOA
#define UARTx_RX_AF                     GPIO_AF4_USART2

/* Definition for USARTx's DMA */
#define UARTx_TX_DMA_CHANNEL            DMA1_Channel2
#define UARTx_RX_DMA_CHANNEL            DMA1_Channel5
#define UARTx_TX_DMA_REQUEST            DMA_REQUEST_4
#define UARTx_RX_DMA_REQUEST          	DMA_REQUEST_4

/* Definition for USARTx DMA's NVIC */
#define UARTx_DMA_TX_IRQn             	DMA1_Channel2_3_IRQn
#define UARTx_DMA_RX_IRQn               DMA1_Channel2_3_IRQn
#define UARTx_DMA_IRQ_PRI								0	

/* Definition for USARTx's NVIC */
#define UARTx_IRQn                      USART2_IRQn
#define UARTx_IRQ_PRI										2




#define WIFI_IOx_CLK_ENABLE()			do{ __HAL_RCC_GPIOA_CLK_ENABLE(); \
																	}while(0)

																	
#define WIFI_CC_EN_Pin 							GPIO_PIN_3
#define WIFI_CC_EN_GPIO_Port 			  GPIOA
																											
#define WIFI_STreq_Pin 							GPIO_PIN_4
#define WIFI_STreq_GPIO_Port 				GPIOA
										
#define WIFI_CCreq_Pin 							GPIO_PIN_5
#define WIFI_CCreq_GPIO_Port 				GPIOA
#define WIFI_CCreq_IRQn							EXTI4_15_IRQn
#define WIFI_CCreq_IRQ_PRI					1					
																	
																	

/* Exported functions ------------------------------------------------------- */
void BSP_UART_Init(void);
void BSP_UART_DeInit(void);
void BSP_UART_TransmitDMA(uint8_t *byte_array, uint8_t size);
void BSP_UART_TransmitBlocking(uint8_t *byte_array, uint8_t size, uint16_t timeout_ms);
void BSP_UART_ReceiveDMA(uint8_t *byte_array, uint8_t size);
void BSP_UART_Receive_Mode_DMA_IDEL_IT(uint8_t *byte_array);

//bool BSP_UART_ReceiveDMACplt(void);
bool BSP_UART_ReceiveBlocking(uint8_t *byte_array, uint8_t size, uint16_t timeout_ms);
void BSP_UART_StopDMA(void);


void DMA_IDEL_IT_Get_data(void);

bool DMA_RX_Status(void);


/* Linker functions ------------------------------------------------------- */

/**@brief External linker function to be implemented */
void BSP_UART_RX_DMA_Handler(uint16_t data_len);
void BSP_UART_Error_Handler(void);

#endif	/* USE_UART */

#endif /* __BSP_UART_H */
/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
