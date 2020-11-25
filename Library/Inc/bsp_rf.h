/**
  ******************************************************************************
  * @file    bsp_rf.h 
  * @author  YZTek Wilson
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


#define TX_BUFFER_SIZE		100
#define RX_BUFFER_SIZE	  170
#define MQTT_DATA_BUFFER_SIZE 100
#define MQTT_CMD_BUFFER_SIZE 10
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
		char * IPconfig;
		char * DnsGet;
		char * MQTT;
		char * MQTT_DISCONNECT;
		char * MQTT_READ;
		
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
		char * IPconfig;
		char * DnsGet;
	  char * MQTT_Init;
		char * MQTT_Con;
	  char * MQTT_Sub;
	  char * MQTT_Pub;
	  char * MQTT_unSub;
	  char * MQTT_disCon;
	  char * MQTT_delete;
}RSI_AT_COMMAND_PARAMETER_t;
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
		S_is_WIFI_Connected,
		S_DNS_get,
		S_MQTT_Init,
		S_MQTT_Con,
		S_LowerPowerMode,

}RS9116_State_t;
/**@RSI Error code */

typedef enum
{
	None_error=0x00,	
	Commmunication_Timeout,
	AT_Respond_Error,
	WIFI_Join_Error,
	DHCP_Error,
	DNS_IP_lost,
	Subscribe_Timeout,
  Published_Timeout,

}RS9116_ERROR_t;
/**@CMD  User can add CMD here*/
typedef enum
{
	CMD_EMPTY=0X00,
	CMD_MQTT_On,
	CMD_MQTT_Off,
	
}MQTT_CMD_t;
/**@CMD  User can add CMD here*/
typedef enum
{
	MQTT_None=0X00,
	MQTT_Publishing,
	MQTT_Published,
	MQTT_Subscribing,
	MQTT_Subscribed,
}MQTT_Status;
/**@MQTT infor */

typedef struct
{
  uint8_t    DNS_IP_hex[4];
	char       DNS_IP_String[15];
  MQTT_Status mqtt_state;
	
	char        mqtt_recive_buf[MQTT_DATA_BUFFER_SIZE];
  MQTT_CMD_t  CMD_Buffer[MQTT_CMD_BUFFER_SIZE]; 
	uint8_t     read_index;
  uint8_t     write_index;

}MQTT_t;
/**@MQTT infor */
//typedef enum
//{
// DMA_Character_Match_Mode=0x00,
// DMA_IDEL_IT_Mode,

//}DMA_MODE_T;

typedef struct
{	
	RS9116_State_t rs_state;
	RS9116_ERROR_t error_code;
	char   m_tx_buf[TX_BUFFER_SIZE] ;
	char   m_rx_buf[RX_BUFFER_SIZE] ;
	
	char   string_data_length[3];
	MQTT_t MQTT;
	//DMA_MODE_T dma_mode;
	uint8_t     DMA_IT_Character;

}RF_Ctrl_t;

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
/**For Application use*/
bool BSP_RF_RS9116_Init(void);
bool BSP_RF_RS9116_WIFI_Connect(void);
bool BSP_RF_RS9116_MQTT_Connect(void);
bool BSP_RF_RS9116_MQTT_DisConnect(void);

bool BSP_RF_RS9116_MQTT_Publish(char * Topic,char * data);

bool BSP_RF_RS9116_MQTT_Subscribe(char * Topic);
MQTT_CMD_t BSP_RF_MQTT_CMD_Pop(void);

bool BSP_RF_RS9116_MQTT_UnSubscribe(char * Topic);


void BSP_RF_RS9116_JSON_Encode(char * JSON ,char * Object1,char * value1,char * Object2,char * value2);


void BSP_RF_set_status(RS9116_State_t stage);
RS9116_State_t BSP_RF_get_module_status(void);
//DMA_MODE_T BSP_RF_Get_DMA_mode(void);

uint8_t BSP_RF_Get_DMA_character_match_word(void);
void BSP_RF_Set_DMA_character_match(uint8_t ch);

/* Linker functions ------------------------------------------------------- */

/**@brief External linker function to be implemented */



#endif /* __BSP_RF_H */
/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
