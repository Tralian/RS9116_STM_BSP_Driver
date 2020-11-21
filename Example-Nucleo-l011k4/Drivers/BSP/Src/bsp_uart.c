/**
  ******************************************************************************
  * @file    bsp_uart.c 
  * @author  YZTek Software TEAM
  * @brief   bsp_uart module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020  YZTek</center></h2>
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_uart.h"
#include "stdio.h"
/* Private typedef -----------------------------------------------------------*/

#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	

/* Private define ------------------------------------------------------------*/
#ifdef	USE_UART
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**@brief  Private uart handles */
UART_HandleTypeDef huart;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
									
/**@brief  Private variables for tx/rx flasg */
static __IO bool m_is_tx_done;
static __IO bool m_is_rx_done;
	
	
//static uint16_t DMA_RX_length=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
	
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
//PUTCHAR_PROTOTYPE
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart, (uint8_t *)(&ch), 1, 0xFFFF); 

  return ch;
}	
	
	
/* Exported function implementation ------------------------------------------*/


/**
  * @brief	BSP uart initialization
  */
void BSP_UART_Init(void)
{

	huart.Instance = UARTx;
	huart.Init.BaudRate   = UART_BAUD;
  huart.Init.WordLength = UART_WORDLENGTH_8B;
  huart.Init.StopBits   = UART_STOPBITS_1;
  huart.Init.Parity     = UART_PARITY_NONE;
  huart.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  huart.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart)!=HAL_OK)
	{
	 while(1);
	}		
	


}

/**
  * @brief	BSP uart de-initialization
  */
void BSP_UART_DeInit(void)
{
	if(huart.Instance == UARTx)
	{
		HAL_UART_DeInit(&huart);
	}

}

/**
  * @brief	BSP uart transmit function, usining DMA
	* @param	byte_array, data to be transmit
	* @param	size, array size in byte
  */
void BSP_UART_TransmitDMA(uint8_t *byte_array, uint8_t size)
{
	HAL_UART_Transmit_DMA(&huart, byte_array, size);	
}

/**
  * @brief	BSP uart transmit function, blocking mode
	* @param	byte_array, data to be transmit
	* @param	size, array size in byte
	* @param	timeout_ms, timeout out value in msec
  */
void BSP_UART_TransmitBlocking(uint8_t *byte_array, uint8_t size, uint16_t timeout_ms)
{
	m_is_tx_done = false;
	HAL_UART_Transmit(&huart, byte_array, size, timeout_ms);
}

/**
  * @brief	BSP uart receive function, DMA mode
	* @param	byte_array, data to be transmit
	* @param	size, array size in byte
  */
void BSP_UART_ReceiveDMA(uint8_t *byte_array, uint8_t size)
{

	m_is_rx_done = false;
	__HAL_UART_CLEAR_FLAG(&huart,UART_CLEAR_OREF);//Clear  if overrun data
	HAL_UART_Receive_DMA(&huart, byte_array, size);
}

///**
//  * @brief	BSP uart receive function, DMA mode and UART Idel interrupt
//	* @param	byte_array, data to be transmit
//     buffer length reference  DMA_RECEIVE_IDEL_IT_BUFFER_SIZE
//  */
//void BSP_UART_Receive_Mode_DMA_IDEL_IT(uint8_t *byte_array)
//{
//	 m_is_rx_done = false;
//	__HAL_UART_ENABLE_IT(&huart,UART_IT_IDLE);
//	__HAL_UART_CLEAR_FLAG(&huart,UART_CLEAR_OREF);//Clear  if overrun data
//	HAL_Delay(10);
//  /**@TBD*/
//	HAL_UART_Receive_DMA(&huart, byte_array, DMA_RECEIVE_IDEL_IT_BUFFER_SIZE);
//}

/**
  * @brief	BSP Stop DMA mode
	* @param	byte_array, data to be transmit
	* @param	size, array size in byte
  */
void BSP_UART_StopDMA(void)
{
	m_is_rx_done = false;
	HAL_UART_DMAStop(&huart);

}
/**
  * @brief	DMA RX Status
	* @param	none
	* @retval	true if DMA done, false if DMA busy.
  * @author YZTEK Wilson
  * 
  */
bool DMA_RX_Status(void)
{
 return m_is_rx_done;

}


/**
  * @brief	BSP uart receive function, blocking mode
	* @param	byte_array, data to be transmit
	* @param	size, array size in byte
	* @param	timeout_ms, timeout out value in msec
	* @retval	true if received, false if error occurs.
  */
bool BSP_UART_ReceiveBlocking(uint8_t *byte_array, uint8_t size, uint16_t timeout_ms)
{	
	if(HAL_UART_Receive(&huart, byte_array, size, timeout_ms) == HAL_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/* Callback function implementation ------------------------------------------*/
/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle == &huart)
	{
		m_is_tx_done = true;
	}
}

/**
  * @brief  DMA Rx Transfer completed callback
  * @param  UartHandle: UART handle
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  if(UartHandle == &huart)
	{
     
		/**@  If Come here have some  Error */
	
	}
}
///**
//  * @brief  This Funciton is use to Get data form elasticity length
//  * @param  None
//  * @note   
//  * @retval None
//  */
//void DMA_IDEL_IT_Get_data(void)
//{

//   		m_is_rx_done = true;
//			static uint16_t temp=0;	
//       uint16_t BUFFER_SIZE=huart.RxXferSize;	//get RX buffer size

//			temp  = hdma_usart2_rx.Instance->CNDTR;// Get how many data didn't get
//			
//	    DMA_RX_length =  BUFFER_SIZE - temp; //Get how many data get
//			BSP_UART_RX_DMA_Handler(DMA_RX_length);
//}
/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
	/**@ToDo: */
	if(UartHandle->Instance == USART2)
	{
		if(UartHandle->ErrorCode == HAL_UART_ERROR_ORE)
		{
			__NOP();
		}
		else if(UartHandle->ErrorCode == HAL_UART_ERROR_DMA)
		{
			__NOP();
		}
	}

}

/* Weakly defined function to be implemented externally --------------------------*/
__weak void BSP_UART_RX_DMA_Handler(uint16_t data_len)
{	/* add your parsing routine here */
	
	/* check packet validity*/
	
	/* if not valid, restart UART/DMA*/
	
	/* if valid, notify app procedure a cmd/data is arrived*/
	__NOP();
	
}




#endif	/* USE_UART */

/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
