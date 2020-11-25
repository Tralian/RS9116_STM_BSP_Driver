/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "bsp_rf.h"

#include "stm32l0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef huart;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern void BSP_UART_RX_DMA_Character_Martch_IT_Handler(void);
extern void DMA_IDEL_IT_Get_data(void);

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{

}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{

  while (1)
  {

  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/* STM32L0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{

//HAL_DMA_IRQHandler(&hdma_usart2_tx);
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
	/**@TBD Temp solutiob */

		/*Character match interrupt*/
			if(__HAL_UART_GET_IT(&huart, UART_IT_CM))
			{
				BSP_UART_StopDMA();/*Stop DMA*/
				BSP_UART_RX_DMA_Character_Martch_IT_Handler();
			  __HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_CMF);
      	__HAL_UART_CLEAR_FLAG(&huart,UART_CLEAR_OREF);//Clear Flag

			}		
	
//	if(BSP_RF_Get_DMA_mode()==DMA_IDEL_IT_Mode)
//	{
//		/*Idle interrupt*/
//		if(__HAL_UART_GET_FLAG(&huart,UART_FLAG_IDLE)== SET)
//		{ 
//				/*Stop DMA*/
//				BSP_UART_StopDMA();/*Stop DMA*/
//				DMA_IDEL_IT_Get_data();	
//	      __HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_IDLEF);

//		}	 
//	}  

  HAL_UART_IRQHandler(&huart);

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
