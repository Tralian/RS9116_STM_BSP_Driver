/**
  ******************************************************************************
  * @file    bsp_rf.c 
  * @author  YZTek Wilson
  * @brief   bsp for RSI9116 WIFI module ,Using AT Command , Connect to AWS
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
typedef enum
{
	 DONE=  0x00,
	 BUSY,
}DMA_Status_t;

/* Private define ------------------------------------------------------------*/
#define TIME_OUT_NORMAL_MS	        5000
#define TIME_OUT_WIFI_CONNECT_MS	  10000
#define TIME_OUT_MQTT_MS	  				10000
#define TIME_OUT_DHCP_MS	  				15000

#define WIFI_DHCP_Try	        3
#define WIFI_WIFI_Join_Try	  3
#define DNS_IP_Try	          3

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/**@For User Config ***********************************************************/
const char * SSID="myLoad";
const char * SSID_password="0912345678";
//const char * SSID="Wei";
//const char * SSID_password="50882056";
const char * AWS_endpoint="av9pn5o54ct6w-ats.iot.ap-northeast-1.amazonaws.com";


const char * MQTT_Server_Port="8883";
const char * Device_ID="NECCUIUaAZDC";

const char * MQTT_username="username";
const char * MQTT_password="password";
const char * MQTT_client_port="5503";

const char * MQTT_keep_alive_interval="80";
const char * MQTT_En_clean_session="1";
const char * MQTT_En_keep_alive_interval="1";
/*******************************************************************************/
const char * MQTT_DISCONNECT="mqtt=8";

const RSI_AT_COMMAND_t rsi_cmd=
{
	.AT_RSI_="at+rsi_",
	.Newline="\r\n",
	.FS=0x1C,
	.U='U',
	._1='1',
	.OK="OK",
	.Error="ERROR",
	.rest="reset",
	.OperMode="opermode=",
	.Frame="feat_frame=",
	.Band="band=",
	.Init="init",
	.WiFI_Scan="scan=",
	.PSK="psk=",
	.WIFI_Join="join=",
	.IPconfig="ipconf=",
	.DnsGet="dnsget=",
	.MQTT="mqtt=",
};

static RSI_AT_COMMAND_PARAMETER_t rsi_data=
{
	.OperMode="0,20,2147550596,2147483648,3670090,0,3221356616,0,0,1",
	.Frame="0,1,0,0,1,49",
	.Band="0",
	.WiFI_Scan="0,",
	.PSK="1,",
	.WIFI_Join=",0,2,2,2,1000,0,0",
	.IPconfig="1,0,0,0",
	.DnsGet=",1",
	.MQTT_Init="1,4,",
	.MQTT_Con="2,1,1,0,0,0,0,0,",
	.MQTT_Sub="3,",
	.MQTT_Pub="4,",
	.MQTT_unSub="4",
	.MQTT_disCon="8",
	.MQTT_delete="9",
};
#ifdef RF_MODULE



static RF_Ctrl_t rf={.MQTT.Publish_Topic="",.MQTT.Subcribe_Topic=""};
/*For RS9116 AT Command*/
/************************ Private functions Prototype************************/
bool BSP_RF_Decode_DNS(void);
DMA_Status_t BSP_RF_Get_DMA_RX_Status(void);
void BSP_RF_Clear_buffer(uint8_t *buffer,uint16_t length);

/************************ Private functions implementation************************/


/**
  *@brief  UART RX DMA Complie Handler
  *@param  RF_Ctrl_t: me pointer
	*@retval None
  *@author YZTEK Wilson
  *
  */
void BSP_UART_RX_DMA_Character_Martch_IT_Handler(void)
{
	/*Find RX buffer keyword*/
	for(uint8_t i=0;i<strlen(rf.m_rx_buf);i++)
	{
		if(*(rf.m_rx_buf+i)=='O'&&*(rf.m_rx_buf+i+1)=='K')
		{
	  	rf.error_code=None_error;		
			break;
		}
		else if(memcmp((void *)(rf.m_rx_buf+i),(void *)rsi_cmd.Error,5)==0)
		{
	  	rf.error_code=AT_Respond_Error;		
			break;
		}
			
	
	}
	if(	 rf.error_code==None_error)
	{
			switch(rf.rs_state)
			{
				case S_Power_off:
					break;
				case S_Booting:			
							BSP_RF_set_status(S_Set_opermode);			
					break;
				case S_Set_opermode:
							BSP_RF_set_status(S_Set_feat);			
					break;		
				case S_Set_feat:
							BSP_RF_set_status(S_Set_band);								
					break;
				case S_Set_band:
							BSP_RF_set_status(S_Init);			
					break;
				case S_Init:	
							BSP_RF_set_status(S_Scan_WIFI);			
					
					break;	
				case S_Scan_WIFI:
							BSP_RF_set_status(S_Set_psk);			
					break;
				case S_Set_psk:
							BSP_RF_set_status(S_Join_WIFI);			
					break;
				case S_Join_WIFI:	
							BSP_RF_set_status(S_is_WIFI_Connected);					
					break;	
				case S_is_WIFI_Connected:
							if(BSP_RF_Decode_DNS()==true)
							{
								BSP_RF_set_status(S_DNS_get);
							}
					break;
				case S_DNS_get:
							
								BSP_RF_set_status(S_MQTT_Init);
					break;
				case S_MQTT_Init:
							BSP_RF_set_status(S_MQTT_Con);
					break;	
				case S_MQTT_Con:
					    if(memcmp((void *)(rf.m_tx_buf),(void *)MQTT_DISCONNECT,6)==0)
							{
								BSP_RF_set_status(S_is_WIFI_Connected);
							}
					break;

		
				case S_LowerPowerMode:
					break;		
			}
			BSP_RF_Clear_buffer((uint8_t *)rf.m_rx_buf,strlen(rf.m_rx_buf));//clearerr rx buffer

  }
	else
	{
	 /**@TBD Need have error handler*/
	
	}
}


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
		BSP_UART_ReceiveDMA((uint8_t *)rf.m_rx_buf,RX_BUFFER_SIZE);
		return status;
}
/**
  *@brief This Function using UART Send AT command to RS9116 WI-FI module
  *       Add "rsi+" at headline, and add <CR><LF> at the end ,
  *				Must Init UART before using it
  *@param  command    : Const char (string)
 	*@retval None
  *@author YZTEK Wilson

  */
void BSP_RF_AT_Command_Send(const char * command)
{


	BSP_UART_TransmitBlocking((uint8_t *)rsi_cmd.AT_RSI_,7,10);
	
	BSP_UART_TransmitBlocking((uint8_t *)command,strlen(command),10);

  BSP_UART_TransmitBlocking((uint8_t *)rsi_cmd.Newline,2,10);

}
/**
  *@brief This Function using UART Send AT command to RS9116 WI-FI module
  *       Add "rsi+" at headline, and add <CR><LF> at the end ,
  *       And Start UART DMA Receive
  *				Must Init UART before using it
  *@param  command    : Const char (string)
  *@param  Next_State : RS9116_State_t If Sucess , go to next state
	*@param  timeout    : Time out for DMA receiving
 	*@retval None
  *@author YZTEK Wilson

  */
void BSP_RF_AT_Command_Communication(const char * command,RS9116_State_t Next_State ,uint32_t timeout)
{


	BSP_UART_TransmitBlocking((uint8_t *)rsi_cmd.AT_RSI_,7,10);
	
	BSP_UART_TransmitBlocking((uint8_t *)command,strlen(command),10);

  BSP_UART_TransmitBlocking((uint8_t *)rsi_cmd.Newline,2,10);
	BSP_RF_Start_dma_receive();
	uint32_t last_time=HAL_GetTick();
	while(1)
	{
		/**@TBD Can do something here while DMA wating */
		if(HAL_GetTick()-last_time>timeout)
		{
			rf.error_code=Commmunication_Timeout;
			break;

		}
		if(rf.rs_state==Next_State)
		{
			break;
		}
	}
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
  BSP_UART_TransmitBlocking((uint8_t *)&rsi_cmd.FS,1,10);
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
/**
  *@brief  Clear  buffer
  *@param  buffer
  *@param  buffer length
	*@retval none
  *@author YZTEK Wilson
  *
  */
void BSP_RF_Clear_buffer(uint8_t *buffer,uint16_t length)
{
		memset(buffer, '\0', length);	
}
/**
  *@brief  Ask for DMA RX Status
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
/**
  *@brief  Decode DNS Ip form AT Command , and store in MQTT_t
  *@param  Buffer : RX buffer 
	*@retval true if success
  *@author YZTEK Wilson
  *
  */
bool BSP_RF_Decode_DNS(void)
{
	uint16_t i=0;
  uint8_t check[4]={0,0,0,0};
	if( (rf.m_rx_buf[0]=='O') && (rf.m_rx_buf[1]=='K') )
	{
		if(rf.m_rx_buf[4]==2)
		{
			for(i=0;i<4;i++)
			{
				
				rf.MQTT.DNS_IP_hex[i]=rf.m_rx_buf[i+6];
			}
		}
	}
	/*Check DNS exsit*/
	if(  memcmp((void *)(rf.MQTT.DNS_IP_hex),(void *)check,4)==0  )
	{
		return false;
	}
	else
	{
		return true;
	}
}





/**
  *@brief  Return String length in ASCII
  *@param  Buffer :  buffer 
  *@retval string : Length (ASCII form) 
  *@author YZTEK Wilson
  *
  */
void BSP_RF_Get_String_Length(const char * Buffer)
{
		uint8_t length=0;
	  length=strlen(Buffer);
		sprintf(rf.string_data_length,"%d",length);
}
/************************ Exported function implementation************************/

/**
  *@brief  Initialize RS9116 WIFI module
  *@param  None
	*@retval True if successful Initialization
  *@author YZTEK Wilson
  *
  */
bool BSP_RF_RS9116_Init(void)
{
	//memset(TX_buffer, '\0', strlen(TX_buffer));	
	/*ReSet Module*/	
	BSP_RF_AT_Command_Send(rsi_cmd.rest);
	HAL_Delay(1000);
	BSP_RF_AT_Send_FS();
	HAL_Delay(1000);
	BSP_RF_AT_Send_Char('U');
	HAL_Delay(1000);
	BSP_RF_AT_Send_Char('1');
	HAL_Delay(2000);
	BSP_RF_set_status(S_Booting);
	/*Operation mode*/	   
	memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.OperMode);
	strcat(rf.m_tx_buf, rsi_data.OperMode);
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Set_opermode,TIME_OUT_NORMAL_MS);

  /*Feat */
	memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.Frame);
	strcat(rf.m_tx_buf, rsi_data.Frame);		
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Set_feat,TIME_OUT_NORMAL_MS);

  /*Band */
	memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.Band);
	strcat(rf.m_tx_buf, rsi_data.Band);		
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Set_band,TIME_OUT_NORMAL_MS);

  /*Init */
	memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.Init);
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Init,TIME_OUT_NORMAL_MS);


	if(rf.rs_state==S_Init)
	{
			return true;
	}
	else
	{
		 return false;

	}
}
/**
  *@brief  Connect to WIFI host 
  *@param  None
	*@retval True if successful Connect
  *@author YZTEK Wilson
  *
  */
bool BSP_RF_RS9116_WIFI_Connect(void)
{
    /*Check module status */
		if(BSP_RF_get_module_status()!=S_Init)
		{
			return false;
		}			
    /*WIFI_Scan */

		memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
		strcat(rf.m_tx_buf, rsi_cmd.WiFI_Scan);
		strcat(rf.m_tx_buf, rsi_data.WiFI_Scan);	
		strcat(rf.m_tx_buf, SSID);		
		BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Scan_WIFI,TIME_OUT_WIFI_CONNECT_MS);

		/*WIFI_PSK */
	  memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
		strcat(rf.m_tx_buf, rsi_cmd.PSK);
		strcat(rf.m_tx_buf, rsi_data.PSK);
		strcat(rf.m_tx_buf, SSID_password);	
	  BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Set_psk,TIME_OUT_WIFI_CONNECT_MS);
		/*WIFI_Join */
    while(BSP_RF_get_module_status()==S_Set_psk)
		{
			static uint8_t WIFI_Join_try=0;

			memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
			strcat(rf.m_tx_buf, rsi_cmd.WIFI_Join);
			strcat(rf.m_tx_buf, SSID);	
			strcat(rf.m_tx_buf, rsi_data.WIFI_Join);		
			BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Join_WIFI,TIME_OUT_WIFI_CONNECT_MS);
			WIFI_Join_try++;
			if(WIFI_Join_try==WIFI_WIFI_Join_Try)
			{
				rf.error_code=WIFI_Join_Error;
				break;
			}
		}		
		/*DCHP */
		while(BSP_RF_get_module_status()==S_Join_WIFI)
		{
			static uint8_t DCPH_try=0;
			memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
			strcat(rf.m_tx_buf, rsi_cmd.IPconfig);
			strcat(rf.m_tx_buf, rsi_data.IPconfig);		
			BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_is_WIFI_Connected,TIME_OUT_DHCP_MS);
			DCPH_try++;
			if(DCPH_try==WIFI_DHCP_Try)
			{
				rf.error_code=DHCP_Error;
			  break;
			}
		}
		if(rf.rs_state==S_is_WIFI_Connected)
		{
				return true;
		}
		else
		{
			 return false;

		}
}
/**
  *@brief  Connect to AWS MQTT broker
  *@param  None
	*@retval True if successful Connect
  *@author YZTEK Wilson
  *
  */
bool BSP_RF_RS9116_MQTT_Connect(void)
{				
    /*Check module status */
		if(BSP_RF_get_module_status()!=S_is_WIFI_Connected)
		{
			return false;			
		}
		while(BSP_RF_get_module_status()==S_is_WIFI_Connected)
		{
			static uint8_t DNS_IP_try=0;

			/*DNS_Get */
			memset(rf.m_tx_buf, '\0', strlen(rf.m_tx_buf));	
			strcat(rf.m_tx_buf, rsi_cmd.DnsGet);
			strcat(rf.m_tx_buf, AWS_endpoint);		
			strcat(rf.m_tx_buf, rsi_data.DnsGet);	
			BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_DNS_get,TIME_OUT_WIFI_CONNECT_MS);
			/*Transform from hex to string*/
			sprintf(rf.MQTT.DNS_IP_String, "%d.%d.%d.%d", rf.MQTT.DNS_IP_hex[0], rf.MQTT.DNS_IP_hex[1], rf.MQTT.DNS_IP_hex[2], rf.MQTT.DNS_IP_hex[3]);
			DNS_IP_try++;
			if(DNS_IP_try==DNS_IP_Try)
			{
				rf.error_code=DNS_IP_lost;
				break;
			}
		
		}
		/*MQTT_Init */

		memset(rf.m_tx_buf, '\0', strlen(rf.m_tx_buf));	
		strcat(rf.m_tx_buf, rsi_cmd.MQTT);
		strcat(rf.m_tx_buf, rsi_data.MQTT_Init);		
		strcat(rf.m_tx_buf, rf.MQTT.DNS_IP_String);
		strcat(rf.m_tx_buf, ",");
		strcat(rf.m_tx_buf, MQTT_Server_Port);
		strcat(rf.m_tx_buf, ",");
    BSP_RF_Get_String_Length(Device_ID);
		strcat(rf.m_tx_buf, rf.string_data_length);
		strcat(rf.m_tx_buf, ",");
		strcat(rf.m_tx_buf, Device_ID);
		strcat(rf.m_tx_buf, ",");
		strcat(rf.m_tx_buf, MQTT_keep_alive_interval);
		strcat(rf.m_tx_buf, ",");
    BSP_RF_Get_String_Length(MQTT_username);
		strcat(rf.m_tx_buf, rf.string_data_length);
		strcat(rf.m_tx_buf, ",");
		strcat(rf.m_tx_buf, MQTT_username);
		strcat(rf.m_tx_buf, ",");
    BSP_RF_Get_String_Length(MQTT_password);
		strcat(rf.m_tx_buf, rf.string_data_length);
		strcat(rf.m_tx_buf, ",");
		strcat(rf.m_tx_buf, MQTT_password);
		strcat(rf.m_tx_buf, ",");
		strcat(rf.m_tx_buf, MQTT_En_clean_session);
		strcat(rf.m_tx_buf, ",");
		strcat(rf.m_tx_buf, MQTT_En_keep_alive_interval);
		strcat(rf.m_tx_buf, ",");
		strcat(rf.m_tx_buf, MQTT_client_port);
		BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_MQTT_Init,TIME_OUT_MQTT_MS);

		/*MQTT_MQTT_Con */
		memset(rf.m_tx_buf, '\0', strlen(rf.m_tx_buf));	
		strcat(rf.m_tx_buf, rsi_cmd.MQTT);
		strcat(rf.m_tx_buf, rsi_data.MQTT_Con);
		BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_MQTT_Con,TIME_OUT_MQTT_MS);

		if(rf.rs_state==S_MQTT_Con)
		{
				return true;
		}
		else
		{
			 return false;

		}
}
/**
  *@brief  DisConnect to AWS MQTT broker
  *@param  None
	*@retval True if successful Connect
  *@author YZTEK Wilson
  *
  */
bool BSP_RF_RS9116_MQTT_DisConnect(void)
{

	memset(rf.m_tx_buf, '\0', strlen(rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.MQTT);
	strcat(rf.m_tx_buf, rsi_data.MQTT_disCon);
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_is_WIFI_Connected,TIME_OUT_MQTT_MS);
	if(rf.rs_state==S_is_WIFI_Connected)
	{
			return true;
	}
	else
	{
		 return false;

	}
}
/**
  *@brief  Publish to AWS MQTT broker
  *@param  None
	*@retval True if successful Connect
  *@author YZTEK Wilson
  *
  */
bool BSP_RF_RS9116_MQTT_Publish(char * Topic,char * data)
{

	memset(rf.m_tx_buf, '\0', strlen(rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.MQTT);
	strcat(rf.m_tx_buf, rsi_data.MQTT_Pub);
	BSP_RF_Get_String_Length(Topic);
	strcat(rf.m_tx_buf, rf.string_data_length);
	strcat(rf.m_tx_buf, ",");	
	strcat(rf.m_tx_buf, Topic);	
	strcat(rf.m_tx_buf, ",1,0,0,");	
	BSP_RF_Get_String_Length(data);
	strcat(rf.m_tx_buf, rf.string_data_length);
	strcat(rf.m_tx_buf, ",");	
	strcat(rf.m_tx_buf, data);
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_MQTT_Con,TIME_OUT_MQTT_MS);
	if(rf.rs_state==S_is_WIFI_Connected)
	{
			return true;
	}
	else
	{
		 return false;

	}
}
/**
  *@brief  Transform data  to JSON format
  *@param  JSON    :buffer storage  JSON format data
  *@param  Object  :name of Object1
	*@param  value 	 :value of data1
  *@param  Object  :name of Object2
	*@param  value 	 :value of data2
	*@retval True if successful Connect
  *@author YZTEK Wilson
  *
  */
void BSP_RF_RS9116_JSON_Encode(char * JSON ,char * Object1,char * value1,char * Object2,char * value2)
{
  
	memset(JSON, '\0', strlen(JSON));	
	strcat(JSON, "{\"");
	strcat(JSON, Object1);
	strcat(JSON, "\":\"");  
	strcat(JSON, value1);  
	strcat(JSON, "\",\"");  
	strcat(JSON, Object2);
	strcat(JSON, "\":\"");  
	strcat(JSON, value2);  
	strcat(JSON, "\"}");  
}

/**
  *@brief  BSP for Setting RF module Status
  *@param  RS9116_State_t
	*@retval none
  *@author YZTEK Wilson
  *
  */

void BSP_RF_set_status(RS9116_State_t stage)
{

	 rf.rs_state=stage;

}
/**
  *@brief  BSP for Getting RF module Status
  *@param  None
	*@retval RS9116_State_t
  *@author YZTEK Wilson
  *
  */
RS9116_State_t BSP_RF_get_module_status(void)
{

	return rf.rs_state;

}

#endif
/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
