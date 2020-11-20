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

/* Private define ------------------------------------------------------------*/
#define ATtimeout_ms	  5000
#define WIFI_CONNECT_timeout_ms	  20000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/**@For User Config ***********************************************************/
const char * SSID="myLoad";
const char * SSID_password="0912345678";
const char * AWS_endpoint="av9pn5o54ct6w-ats.iot.ap-northeast-1.amazonaws.com";


const char * MQTT_Server_Port="8883";
const char * Device_ID="NECCUIUaAZDC";

const char * MQTT_username="username";
const char * MQTT_password="password";
const char * MQTT_client_port="5503";

const char * MQTT_Sub_Topic="yztek/ty002/d/NECCUIUaAZDC";
const char * MQTT_Pub_Topic="yztek/ty002/s";
/*******************************************************************************/

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
	.S_IPconfig="ipconf=",
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
	.S_IPconfig="1,0,0,0",
	.DnsGet=",1",
	.S_MQTT_Con="1",
	.MQTT_Sub="2",
	.MQTT_Pub="3",
	.MQTT_unSub="8",
	.S_MQTT_delete="9",
};
#ifdef RF_MODULE



static RF_Ctrl_t rf;
/*For RS9116 AT Command*/
/************************ Private functions Prototype************************/

DMA_Status_t BSP_RF_Get_DMA_RX_Status(void);

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
	rf.AT_respond=None;
	if(memcmp((void *)(rf.m_rx_buf),(void *)rsi_cmd.OK,2)==0)
	{
		rf.AT_respond=OK;
		memset(rf.m_rx_buf, '\0', strlen(rf.m_rx_buf));	//clearerr rx buffer

	}
	else if(memcmp((void *)(rf.m_rx_buf),(void *)rsi_cmd.Error,5)==0)
	{
				rf.AT_respond=Error;
				rf.error_code=AT_Respond_Error;

	}
	if(rf.AT_respond==OK)
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
							BSP_RF_set_status(S_IPconfig);					
					break;	
				case S_IPconfig:
							BSP_RF_set_status(S_DNS_get);
					break;
				case S_DNS_get:
							BSP_RF_set_status(S_MQTT_Init);
					break;
				case S_MQTT_Init:
							BSP_RF_set_status(S_MQTT_Con);
					break;	
				case S_MQTT_Con:
							//BSP_RF_set_status(S_MQTT_disCon);
					break;
				case S_MQTT_disCon:
						//BSP_RF_set_status(S_MQTT_delete);
					break;
				case S_MQTT_delete:
							//BSP_RF_set_status(S_LowerPowerMode);
					break;			
				case S_LowerPowerMode:
							//BSP_RF_set_status(S_Set_opermode);
					break;		
			}
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
		/**@TBD Can do something here*/
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
	HAL_Delay(1000);
	BSP_RF_set_status(S_Booting);
	/*Operation mode*/	   
	memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.OperMode);
	strcat(rf.m_tx_buf, rsi_data.OperMode);
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Set_opermode,ATtimeout_ms);
  /*Feat */
	memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.Frame);
	strcat(rf.m_tx_buf, rsi_data.OperMode);		
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Set_feat,ATtimeout_ms);
  /*Band */
	memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.Band);
	strcat(rf.m_tx_buf, rsi_data.Band);		
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Set_band,ATtimeout_ms);
  /*Init */
	memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
	strcat(rf.m_tx_buf, rsi_cmd.Init);
	BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Init,ATtimeout_ms);
	bool flag;
	if(rf.rs_state==S_Init)
	{
			flag=true;
	}
	else
	{
			flag=false;

	}
	return flag;
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

    /*WIFI_Scan */
		while(BSP_RF_get_module_status()==S_Init)
		{
			memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
			strcat(rf.m_tx_buf, rsi_cmd.AT_RSI_);
			strcat(rf.m_tx_buf, rsi_cmd.WiFI_Scan);
			strcat(rf.m_tx_buf, rsi_data.WiFI_Scan);	
			strcat(rf.m_tx_buf, SSID);		
			strcat(rf.m_tx_buf, rsi_cmd.Newline);		
			
			BSP_UART_TransmitBlocking((uint8_t *)rf.m_tx_buf,strlen(rf.m_tx_buf),100);
			BSP_RF_Start_dma_receive();
			uint32_t last_time=HAL_GetTick();
				while(1)
				{
					}
	
	
			//BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Scan_WIFI,WIFI_CONNECT_timeout_ms);
		}
		/*WIFI_PSK */
	  memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
		strcat(rf.m_tx_buf, rsi_cmd.PSK);
		strcat(rf.m_tx_buf, rsi_data.PSK);
		strcat(rf.m_tx_buf, SSID_password);	
	  BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Set_psk,WIFI_CONNECT_timeout_ms);
		/*WIFI_Join */
	  memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
		strcat(rf.m_tx_buf, SSID_password);	
		strcat(rf.m_tx_buf, rsi_cmd.WIFI_Join);
		strcat(rf.m_tx_buf, rsi_data.WIFI_Join);		
	  BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_Join_WIFI,WIFI_CONNECT_timeout_ms);
		/*DCHP */
		while(BSP_RF_get_module_status()==S_Join_WIFI)
		{
			memset(rf.m_tx_buf, '\0', strlen((char *)rf.m_tx_buf));	
			strcat(rf.m_tx_buf, rsi_cmd.S_IPconfig);
			strcat(rf.m_tx_buf, rsi_data.S_IPconfig);		
			BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_IPconfig,WIFI_CONNECT_timeout_ms);
			
		}
		bool flag;
		if(rf.rs_state==S_IPconfig)
		{
				flag=true;
		}
		else
		{
				flag=false;

		}
		return flag;
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


		/*DNS_Get */
		memset(rf.m_tx_buf, '\0', strlen(rf.m_tx_buf));	
		strcat(rf.m_tx_buf, rsi_cmd.DnsGet);
		strcat(rf.m_tx_buf, AWS_endpoint);		
		strcat(rf.m_tx_buf, rsi_data.DnsGet);	
		while(1)
		{
			BSP_RF_AT_Command_Communication(rf.m_tx_buf,S_MQTT_Con,WIFI_CONNECT_timeout_ms);
			if(rf.AT_respond==OK)
			{
         break;			
			}
		}
//		/*MQTT_Init */
//		memset(rf.m_tx_buf, '\0', strlen(rf.m_tx_buf));	
//		strcat(AT_Send_buffer, rsi_cmd.DnsGet);
//		strcat(AT_Send_buffer, AWS_endpoint);		
//		strcat(AT_Send_buffer, rsi_data.DnsGet);
//		BSP_RF_AT_Command_Communication(rf.m_tx_buf);	


//		/*MQTT_MQTT_Con */
//		memset(rf.m_tx_buf, '\0', strlen(rf.m_tx_buf));	
//		strcat(rf.m_tx_buf, RSI_AT_MQTT);
//		strcat(rf.m_tx_buf, RSI_AT_MQTT_Con_parameter);		
//		BSP_RF_AT_Command_Communication(rf.m_tx_buf);	
		bool flag;
		if(rf.rs_state==S_MQTT_Con)
		{
				flag=true;
		}
		else
		{
				flag=false;

		}
		return flag;
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
