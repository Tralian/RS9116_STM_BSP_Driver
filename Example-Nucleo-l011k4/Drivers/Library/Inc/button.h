/**
  ******************************************************************************
  * @file    button.h 
  * @author  YZTek Software TEAM
  * @brief   Header for button module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020  YZTek</center></h2>
  *
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTON_H
#define __BUTTON_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>

/* Exported macro ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/**@brief Button operation mode enumeration type */
typedef enum
{
	BTN_MODE_SIMPLE = 0x00,			/* Support single normal press type and release typre */
	BTN_MODE_LONG_PRESS,					/* Support single normal press type, long press type and release type*/
	BTN_MODE_CONTINUOUS_PRESS,	/* Support continuous normal press type and release type*/
}BTN_Mode_t;

/**@brief Button press active state enumeration type */
typedef enum
{
  BTN_RESET = 0U,							
  BTN_SET
}BTN_PinState_t;

/**@brief Button parameters structure */
typedef struct
{
	uint16_t 				debounce_time;				/* in msec */
	uint16_t 				normal_press_time;			/* in msec */
	uint16_t				conti_press_time;
	uint16_t 				long_press_time;			/* in msec */
	BTN_Mode_t 			btn_mode;					/* btn operation mode */
	BTN_PinState_t 	active_state;
}BTN_Param_t;

/**@brief Button press enumeration type */
typedef enum
{
	BTN_PRESS_NORMAL = 0x00,		/* Simple normal press */
	BTN_PRESS_CONTI,
	BTN_PRESS_LONG,							/* Long press type supported in extended button mode */
	BTN_PRESS_RELEASED,					/* Release type */
}BTN_Press_t;

/**@brief Button state enumeration type */
typedef enum
{
	BTN_STATE_IDLE = 0x00,			/* button not enabled */
	BTN_STATE_INITIAL,
	BTN_STATE_DEBOUNCE,					
	BTN_STATE_PRESSED,
	BTN_STATE_WAITRELEASE,
	BTN_STATE_CONTI_PRESS,			/* only in BTN_MODE_CONTINUOUS_PRESS */
	BTN_STATE_WAIT_LONGPRESS		/* only in BTN_MODE_LONG_PRESS */
}BTN_State_t;

/**@brief Button press handler typedef */
typedef void (*btn_handler_t)(BTN_Press_t press_type, void *reserve);
typedef BTN_PinState_t (*btn_read_pin_t)(void);

/**@brief Button structure type */
typedef struct
{
	BTN_State_t			state;	
	uint32_t 				last_time;
	BTN_Param_t 		params;
	btn_handler_t 	handler;
	btn_read_pin_t 	read;
	void 						*reserve;
}BTN_t;

/* Exported functions ------------------------------------------------------- */

void BTN_ctor(BTN_t *const me, BTN_Param_t params, btn_handler_t handler, btn_read_pin_t read, void *reserve);
void BTN_config(BTN_t *const me, BTN_Param_t params, btn_handler_t handler);
void BTN_start(BTN_t *const me);
void BTN_stop(BTN_t *const me);
void BTN_update(BTN_t *const me);
BTN_Param_t BTN_getParam(const BTN_t *const me);

/**@brief Exported function to be implement, only weakly defined here */
uint32_t BTN_now(void);

#endif /* __BUTTON_H */
/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
