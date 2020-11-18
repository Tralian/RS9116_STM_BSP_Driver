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
#include "bsp_rf.h"
#include <stdio.h>
#include <string.h>
#include "bsp_uart.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#ifdef RF_MODULE



RF_Ctrl_t rf;
/*For RS9116 AT Command*/
const char * RSI_AT_rsi     = "at+rsi_";
const char * RSI_AT_newline = "\r\n";
const char RSI_AT_FS=0x1C;
const char RSI_AT_U='U';
const char RSI_AT_1='1';

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  *@brief  UART RX DMA Complie Handler
  *@param  data_len : length of data receive
	*@retval None
  *@author YZTEK Wilson
  *
  */
void BSP_UART_RX_DMA_Handler(uint16_t data_len)
{

	if(data_len==0)
	{		
		return;
	}
	if(rf.m_rx_buf[1]=='a')
	{
	
	}		
	printf("aaaaa\n");
}
/**
  *@brief  ask for DMA RX Status
  *@param  None
	*@retval true if DMA done, false if DMA busy.
  *@author YZTEK Wilson
  *
  */
DMA_Status_t BSP_RF_Get_DMA_RX_Status(void)
{
		DMA_Status_t a=(DMA_Status_t)DMA_RX_Status();
		return a;
}




/* Exported function implementation ------------------------------------------*/








//void Set_RS9116_Status(RS9116_State_t stage)
//{

//	 rf.cc_state=stage;

//}

//RS9116_State_t Get_RS9116_Status(void)
//{

//	return rf.cc_state;

//}
/**
  *@brief  API for UART DMA receive
  *@param  none
  *@retval False If DMA is busy
  *				 True  If DMA start receive
  *@author YZTEK Wilson
					 
  */
bool BSP_RF_Start_dma_receive(void)
{
	  bool status=BSP_RF_Get_DMA_RX_Status();
    BSP_UART_Receive_Mode_DMA_IDEL_IT(rf.m_rx_buf);
		return status;
}
/**

   @brief read pin function 

  */
//uint8_t BSP_READ_CCreq_status(void)
//{		
//	return (((WIFI_CCreq_GPIO_Port->IDR & WIFI_CCreq_Pin) != (uint32_t)GPIO_PIN_RESET) ? 1 : 0);
//}
/**

   @brief read pin function 

  */
//void BSP_WIFI_GPIO_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	WIFI_IOx_CLK_ENABLE();
//	/*CC3200 Enable pin :PA3*/
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;		
//	GPIO_InitStruct.Pin  = WIFI_CC_EN_Pin;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

//	HAL_GPIO_Init(WIFI_CC_EN_GPIO_Port, &GPIO_InitStruct);
//	
//	/*ST request pin:			PA4*/
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_NOPULL;		
//	GPIO_InitStruct.Pin  = WIFI_STreq_Pin;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(WIFI_STreq_GPIO_Port, &GPIO_InitStruct);
//	
//	/*CC3220 request pin :PA5*/
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLDOWN;		
//	GPIO_InitStruct.Pin  = WIFI_CCreq_Pin;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(WIFI_CCreq_GPIO_Port, &GPIO_InitStruct);
//	
//}
//void BSP_RF_Power_ON(void)
//{ 
//	//HAL_GPIO_WritePin(WIFI_CC_EN_GPIO_Port,WIFI_CC_EN_Pin,GPIO_PIN_SET);
//	WIFI_CC_EN_GPIO_Port->BSRR = WIFI_CC_EN_Pin; 
//}
//void BSP_RF_Power_OFF(void)
//{ 
//	WIFI_CC_EN_GPIO_Port->BRR = WIFI_CC_EN_Pin; 
//}
//void BSP_ST_Req_HIGH(void)
//{
//	WIFI_STreq_GPIO_Port->BSRR = WIFI_STreq_Pin; 
//}
//void BSP_ST_Req_LOW(void)
//{
//	WIFI_STreq_GPIO_Port->BRR = WIFI_STreq_Pin; 
//}



/**
  *@brief This Function using UART Send AT command to RS9116 WI-FI module
  *       Add "rsi+" at headline, and add <CR><LF> at the end ,
  *				Must Init UART before using it
  *@param  Const chat (string)
 	*@retval None
  *@author YZTEK Wilson

  */
void BSP_RF_AT_Command_Send(const char * command)
{


	BSP_UART_TransmitBlocking((uint8_t *)RSI_AT_rsi,7,10);
	
	BSP_UART_TransmitBlocking((uint8_t *)command,strlen(command),10);

  BSP_UART_TransmitBlocking((uint8_t *)RSI_AT_newline,2,10);
}
/**
  *@brief This Function using UART Send AT command to RS9116 WI-FI module        
  *				Must Init UART before using it
  *@param  None
 	*@retval None
  *@author YZTEK Wilson
  *
  */
void BSP_RF_AT_Send_FS(void)
{
  BSP_UART_TransmitBlocking((uint8_t *)&RSI_AT_FS,1,10);
}
/**
  *@brief This Function using UART Send char to RS9116 WI-FI module
					 Must Init UART before using it
  *@param  char
 	*@retval None
  *@author YZTEK Wilson

  */
void BSP_RF_AT_Send_Char(char ch)
{
	BSP_UART_TransmitBlocking((uint8_t *)&ch,1,10);
}
#endif
/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
