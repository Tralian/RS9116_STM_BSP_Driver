/**
  ******************************************************************************
  * @file    bsp_rf.h 
  * @author  YZTek Software TEAM
  * @brief   Header for bsp_rf.h module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020  YZTek</center></h2>
  *
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_RF_H
#define __BSP_RF_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include <stdbool.h>
#include <stdio.h>

#include "soft_timer.h"
#include "bsp_uart.h"

/* Exported types ------------------------------------------------------------*/
#define DEFAULT_WIFI_PACK_LEN				6
#define CMD_BUF_SIZE				        10
#define CMD_BLOCKING_TX_TIMEOUT_S	  10


typedef enum
{
	 DONE=  0x00,
	 BUSY=  0x01,

}DMA_Status_t;
//typedef struct
//{
//	RX_Command_t  cmd_buf[CMD_BUF_SIZE];
//	uint8_t    write_index;
//	uint8_t    read_index;

//}Command_buffer_t;




typedef enum
{
		RS9116_power_off  =0x00,
		RS9116_set_opermode,
	  RS9116_set_feat,
		RS9116_set_band,
		RS9116_init,
		RS9116_scan_WIFI,
		RS9116_set_psk,
		RS9116_join_WIFI,
		RS9116_IPconfig,
		RS9116_DNS_get,
		RS9116_MQTT_Init,
		RS9116_MQTT_Con,
		RS9116_MQTT_Sub,
		RS9116_MQTT_Pub,
		RS9116_MQTT_unSub,
		RS9116_MQTT_disCon,
		RS9116_MQTT_delete,
		RS9116_LPM,

}RS9116_State_t;

#ifdef RF_MODULE

typedef struct
{	
	RS9116_State_t rs_state;
	uint8_t  m_rx_buf[DMA_RECEIVE_IDEL_IT_BUFFER_SIZE] ;
}RF_Ctrl_t;
#endif
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/





			

/* Exported functions ------------------------------------------------------- */


/**For Application use*/


/**********************/
void BSP_RF_AT_Command_Send(const char * command);
void BSP_RF_AT_Send_FS(void);
void BSP_RF_AT_Send_Char(char ch);


bool BSP_RF_Start_dma_receive(void);
DMA_Status_t BSP_RF_Get_DMA_RX_Status(void);



/* Linker functions ------------------------------------------------------- */

/**@brief External linker function to be implemented */



#endif /* __BSP_RF_H */
/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
