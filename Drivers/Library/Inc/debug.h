/**
  ******************************************************************************
  * @file    debug.h 
  * @author  YZTek Software TEAM
  * @brief   Header for debug.h module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020  YZTek</center></h2>
  *
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEBUG_H
#define __DEBUG_H
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
#ifdef USE_DEBUG_PRINT
	int	DBG_Printf(const char* format, ...);
  #define DBG_PRINT(format, ...) 					DBG_Printf(format, ##__VA_ARGS__)
#else
  #define DBG_PRINT(format ...) 					((void)0U)
#endif

#endif /* __DEBUG_H */

/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
