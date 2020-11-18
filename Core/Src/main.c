/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "stdio.h"

#include <stdlib.h>
#include <string.h>

#include "bsp_uart.h"
#include "bsp_rf.h"

/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_GPIO_Init(void);

/* Private user code ---------------------------------------------------------*/


//extern UART_HandleTypeDef huart;
extern RF_Ctrl_t rf;

const char * RSI_AT_rest    = "reset";
/*Operation mode*/
const char * RSI_AT_OperMode="opermode=";
const char * RSI_AT_OperMode_parameter="0,20,2147550596,2147483648,3670090,0,3221356616,0,0,1";
/*Feat */
const char * RSI_AT_Feat="feat_frame=";
const char * RSI_AT_Feat_parameter="0,1,0,0,1,49";
/*Band */
const char * RSI_AT_Band="band=";
const char * RSI_AT_Band_parameter="0";
/*Init */
const char * RSI_AT_Init="init";

/*WIFI_Scan */
const char * RSI_AT_WIFI_Scan="scan=";
const char * RSI_AT_WIFI_Scan_parameter="0,myLoad";
/*WIFI_PSK */
const char * RSI_AT_WIFI_PSK="psk=";
const char * RSI_AT_WIFI_PSK_parameter="1,0912345678";
/*WIFI_Join */
const char * RSI_AT_WIFI_Join="join=";
const char * RSI_AT_WIFI_Join_parameter="myLoad,0,2,2,2,1000,0,0";
/*DCHP */
const char * RSI_AT_DCHP="ipconf=";
const char * RSI_AT_DCHP_parameter="1,0,0,0";
/*DNS_Get */
const char * RSI_AT_DNS_Get="dnsget=";
const char * RSI_AT_DNS_Get_parameter="av9pn5o54ct6w-ats.iot.ap-northeast-1.amazonaws.com,1";
/*MQTT */
const char * RSI_AT_MQTT="mqtt=";
/*MQTT_Init */
const char * RSI_AT_MQTT_Init_parameter_1="1,";
      char * RSI_AT_MQTT_Init_parameter_2;
const char * RSI_AT_MQTT_Init_parameter_3=",8883,12,NECCUIUaAZDC,80,8,username,8,password,1,1,5003";
/*MQTT_MQTT_Con */
const char * RSI_AT_MQTT_Con_parameter="2,1,1,0,0,0,0,0";
/*MQTT_MQTT_Sub */
const char * RSI_AT_MQTT_Sub_parameter="3,26,yztek/ty002/d/NECCUIUaAZDC,1";
/*MQTT_MQTT_Pub */
const char * RSI_AT_MQTT_Pub_parameter="4,13,yztek/ty002/s,1,0,0,31,{\"udid\":\"NECCUIUaAZDC\",\"s\":\"2\"}";
/*MQTT_MQTT_unSub */
const char * RSI_AT_MQTT_unSub_parameter="5,26,yztek/ty002/d/NECCUIUaAZDC";
/*MQTT_MQTT_DisCon */
const char * RSI_AT_MQTT_DisCon_parameter="8";
/*MQTT_MQTT_Delete */
const char * RSI_AT_MQTT_Delete_parameter="9";
int main(void)
{

  HAL_Init();  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  SystemClock_Config();  /* Configure the system clock */
  MX_GPIO_Init();  /* Initialize all configured peripherals */
	BSP_UART_Init();
	//uint8_t a[100];
//	HAL_UART_Receive_DMA(&huart, a, 100);
	//uint8_t count =0; 
	/* Infinite loop */
	
	//char AT_Send_buffer[100];

  while (1)

  { 
		if(BSP_RF_Get_DMA_RX_Status()==DONE)
		{
			BSP_RF_Start_dma_receive();
		}
//		/*ReSet Module*/	
//     BSP_RF_AT_Command_Send(RSI_AT_rest);
//	   BSP_RF_AT_Send_FS();
//     BSP_RF_AT_Send_Char('U');
//		 BSP_RF_AT_Send_Char('1');
//		/*Operation mode*/	   
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_OperMode);
//		strcat(AT_Send_buffer, RSI_AT_OperMode_parameter);

//		BSP_RF_Start_dma_receive();
//		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);


//    /*Feat */
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_Feat);
//		strcat(AT_Send_buffer, RSI_AT_Feat_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);


//    /*Band */
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_Band);
//		strcat(AT_Send_buffer, RSI_AT_Band_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);		
//    /*Init */
//		
//		BSP_RF_AT_Command_Send(RSI_AT_Init);		

//		/*WIFI_Scan */

//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_WIFI_Scan);
//		strcat(AT_Send_buffer, RSI_AT_WIFI_Scan_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);
//		/*WIFI_PSK */
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_WIFI_PSK);
//		strcat(AT_Send_buffer, RSI_AT_WIFI_PSK_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);

//		/*WIFI_Join */
//		
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_WIFI_Join);
//		strcat(AT_Send_buffer, RSI_AT_WIFI_Join_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);

//		/*DCHP */
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_DCHP);
//		strcat(AT_Send_buffer, RSI_AT_DCHP_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);

//		/*DNS_Get */
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_DNS_Get);
//		strcat(AT_Send_buffer, RSI_AT_DNS_Get_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);

//		/*MQTT_Init */
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_MQTT);
//		strcat(AT_Send_buffer, RSI_AT_MQTT_Init_parameter_1);		
//		strcat(AT_Send_buffer, RSI_AT_MQTT_Init_parameter_2);
//		strcat(AT_Send_buffer, RSI_AT_MQTT_Init_parameter_3);	
//		BSP_RF_AT_Command_Send(AT_Send_buffer);	


//		/*MQTT_MQTT_Con */
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_MQTT);
//		strcat(AT_Send_buffer, RSI_AT_MQTT_Con_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);	


//		/*MQTT_MQTT_Sub */

//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_MQTT);
//		strcat(AT_Send_buffer, RSI_AT_MQTT_Sub_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);		
//		/*MQTT_MQTT_Pub */

//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_MQTT);
//		strcat(AT_Send_buffer, RSI_AT_MQTT_Pub_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);	




//		/*MQTT_MQTT_unSub */

//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_MQTT);
//		strcat(AT_Send_buffer, RSI_AT_MQTT_unSub_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);	

//		/*MQTT_MQTT_DisCon */
//		
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_MQTT);
//		strcat(AT_Send_buffer, RSI_AT_MQTT_DisCon_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);	
//	

//		/*MQTT_MQTT_Delete */
//		
//		memset(AT_Send_buffer, '\0', strlen(AT_Send_buffer));	
//		strcat(AT_Send_buffer, RSI_AT_MQTT);
//		strcat(AT_Send_buffer, RSI_AT_MQTT_Delete_parameter);		
//		BSP_RF_AT_Command_Send(AT_Send_buffer);	

	

    HAL_Delay(5000);

  }
}
/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/

void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
