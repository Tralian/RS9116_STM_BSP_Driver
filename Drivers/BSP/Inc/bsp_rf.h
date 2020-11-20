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

/* Exported macro ------------------------------------------------------------*/


#define TX_BUFFER_SIZE		70
#define RX_BUFFER_SIZE	  150
/* Exported types ------------------------------------------------------------*/
/**@AT Command Struct*/
typedef struct 
{
	  char * AT_RSI_;
	  char * Newline;
		char * OK;
	  char * Error;
	  char FS;
		char U;
	  char _1;
	  char * rest;
	  char * OperMode;
	  char * Frame;
	  char * Band;
	  char * Init;
	  char * WiFI_Scan;
	  char * PSK;
		char * WIFI_Join;
		char * S_IPconfig;
		char * DnsGet;
		char * MQTT;
		
}RSI_AT_COMMAND_t;
/**@AT Command Parameter*/

typedef struct 
{
	  char * OperMode;
	  char * Frame;
	  char * Band;
	  char * WiFI_Scan;
	  char * PSK;
		char * WIFI_Join;
		char * S_IPconfig;
		char * DnsGet;
		char * S_MQTT_Con;
	  char * MQTT_Sub;
	  char * MQTT_Pub;
	  char * MQTT_unSub;
	  char * S_MQTT_disCon;
	  char * S_MQTT_delete;
}RSI_AT_COMMAND_PARAMETER_t;


typedef enum
{
	 DONE=  0x00,
	 BUSY,

}DMA_Status_t;
typedef enum
{
	 None= 0x00,
	 OK,
	 Error,

}AT_respond_t;
/**@RSI status */

typedef enum
{
		S_Power_off  =0x00,
	  S_Booting,
		S_Set_opermode,
	  S_Set_feat,
		S_Set_band,
		S_Init,
		S_Scan_WIFI,
		S_Set_psk,
		S_Join_WIFI,
		S_IPconfig,
		S_DNS_get,
		S_MQTT_Init,
		S_MQTT_Con,
		S_MQTT_disCon,
		S_MQTT_delete,
		S_LowerPowerMode,

}RS9116_State_t;
/**@RSI Error code */

typedef enum
{
	None_error=0x00,	
	Commmunication_Timeout,
	AT_Respond_Error,
}RS9116_ERROR_t;
/**@MQTT infor */

typedef struct
{
	uint8_t Num_SubScribe_Topic;
	char *  Sub_Topic_nmae;
	uint8_t Num_PubScribe_Topic;
	char *  Pub_Topic_nmae;
  /*Need add Topic ,Can Export here*/
}MQTT_t;

typedef struct
{	
	RS9116_State_t rs_state;
	MQTT_t Topic;
	RS9116_ERROR_t error_code;
	AT_respond_t AT_respond;
	char   m_tx_buf[TX_BUFFER_SIZE] ;
	char   m_rx_buf[RX_BUFFER_SIZE] ;
}RF_Ctrl_t;

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/**For Application use*/
bool BSP_RF_RS9116_Init(void);
bool BSP_RF_RS9116_WIFI_Connect(void);
bool BSP_RF_RS9116_MQTT_Connect(void);
bool BSP_RF_RS9116_MQTT_DisConnect(void);
bool BSP_RF_RS9116_MQTT_Subcribe(void);
bool BSP_RF_RS9116_MQTT_UnSubcribe(void);
bool BSP_RF_RS9116_MQTT_Publish(void);


void BSP_RF_set_status(RS9116_State_t stage);
RS9116_State_t BSP_RF_get_module_status(void);


/* Linker functions ------------------------------------------------------- */

/**@brief External linker function to be implemented */



#endif /* __BSP_RF_H */
/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
