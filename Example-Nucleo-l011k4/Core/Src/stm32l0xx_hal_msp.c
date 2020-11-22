/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : stm32l0xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization
  *                      and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_uart.h"


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/
/**@brief extern DMA handles */
extern DMA_HandleTypeDef hdma_adc;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart;

/* Private function prototypes -----------------------------------------------*/

/* External functions --------------------------------------------------------*/



/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{


  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/


}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA15     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = UARTx_TX_PIN|UARTx_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = UARTx_RX_AF;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		
		

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = UARTx_RX_DMA_CHANNEL;
    hdma_usart2_rx.Init.Request = UARTx_RX_DMA_REQUEST;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
			while(1)
			{
			}
      //Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2_TX Init */
    hdma_usart2_tx.Instance = UARTx_TX_DMA_CHANNEL;
    hdma_usart2_tx.Init.Request = UARTx_TX_DMA_REQUEST;
    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_tx) != HAL_OK)
    {
			while(1)
			{
			}
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart2_tx);
			/* NVIC configuration for DMA transfer complete interrupt (USART2_TX) */
//		HAL_NVIC_SetPriority(UARTx_DMA_TX_IRQn, UARTx_DMA_IRQ_PRI, 0);
//		HAL_NVIC_EnableIRQ(UARTx_DMA_TX_IRQn);
    
		/* NVIC configuration for DMA transfer complete interrupt (USART2_RX) */
		HAL_NVIC_SetPriority(UARTx_DMA_RX_IRQn, UARTx_DMA_IRQ_PRI, 0);
		HAL_NVIC_EnableIRQ(UARTx_DMA_RX_IRQn);
  
		/* NVIC for USART, to catch the TX complete */
		HAL_NVIC_SetPriority(UARTx_IRQn, UARTx_IRQ_PRI, 0);
		HAL_NVIC_EnableIRQ(UARTx_IRQn);
		/*UART Character Match Interrupt*/
		USART2->CR2 |= 0x0A000000; //  \n 0x0A
    __HAL_UART_ENABLE_IT(uartHandle,UART_IT_CM);
		
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA15     ------> USART2_RX
    */
    HAL_GPIO_DeInit(UARTx_TX_GPIO_PORT, UARTx_TX_PIN);
    HAL_GPIO_DeInit(UARTx_RX_GPIO_PORT, UARTx_RX_PIN);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
