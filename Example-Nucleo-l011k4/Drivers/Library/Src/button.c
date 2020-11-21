/**
  ******************************************************************************
  * @file    button.c 
  * @author  YZTek Software TEAM
  * @brief   button module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020  YZTek</center></h2>
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "button.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported function implementation ------------------------------------------*/

/**
  * @brief  BTN object constructor
  * @param  me, @ref BTN_t pointer
  * @param	params, button params
  * @param	handler, button handler
	* @param	read, read pin liner function
  */
void BTN_ctor(BTN_t *const me, BTN_Param_t params, btn_handler_t handler, btn_read_pin_t read, void *reserve)
{
	/* Copy input parameters and handlers*/
	me->params = params;
	me->handler = handler;
	me->read = read;
	
	/* Set firstly in IDLE state */
	me->state = BTN_STATE_IDLE;	
	
	/* reserve object */
	me->reserve = reserve;
}

/**
  * @brief  BTN object start update, set state out of idle
  * @param  me, @ref BTN_t pointer
  */
void BTN_start(BTN_t *const me)
{
	me->state = BTN_STATE_INITIAL;
}

/**
  * @brief  BTN object set to idle
  * @param  me, @ref BTN_t pointer
  */
void BTN_stop(BTN_t *const me)
{
	me->state = BTN_STATE_IDLE;
}

/**
  * @brief  BTN object update function.
  * @param  me, @ref BTN_t pointer
	*	@note		This routine should be called reqularly to update btn status and 
	*					execute btn handler automatically
  */
void BTN_update(BTN_t *const me)
{
	uint32_t now;
	BTN_PinState_t pin_state;

	/* return if idle state */
	if(me->state == BTN_STATE_IDLE)
	{
		return;
	}
	
	pin_state = me->read();											/* read current pin status */
	now =  BTN_now();														/* get current time */
		
	if(me->state == BTN_STATE_INITIAL)					/* Initial state */
	{			
		if(pin_state == me->params.active_state)	/* if btn is pressed */
		{			
			me->last_time = now;										/* update last time */
			me->state = BTN_STATE_DEBOUNCE;					/* btn pressed, go to debouce state*/
		}
	}
	else if(me->state == BTN_STATE_DEBOUNCE)		/* Debounce state */
	{	
		if(pin_state == me->params.active_state)	/* if btn is pressed */
		{	
			/* if debounce time is past */
			if((now - me->last_time) > me->params.debounce_time)
			{				
				me->last_time = now;									/* update last_time */
				me->state = BTN_STATE_PRESSED;				/* move to pressed state */
			}
		}
		else 
		{
			me->state = BTN_STATE_INITIAL;					/* Btn not pressed, start over */
		}
	}
	/* btn pressed state or continuous pressed state */
	else if(me->state == BTN_STATE_PRESSED)			
	{
		if(pin_state == me->params.active_state)	/* btn still pressed*/
		{
			/* btn pressed ok, call handler*/
			if((now - me->last_time) > (me->params.normal_press_time))
			{
				if(me->handler != NULL)
				{
					me->handler(BTN_PRESS_NORMAL, me->reserve);
				}
				me->last_time = now;
				
				if(me->params.btn_mode == BTN_MODE_SIMPLE)
				{
					me->state = BTN_STATE_WAITRELEASE;		/* move to wait release state */
				}
				else if(me->params.btn_mode == BTN_MODE_CONTINUOUS_PRESS)
				{
					me->state = BTN_STATE_CONTI_PRESS;
				}
				else if(me->params.btn_mode == BTN_MODE_LONG_PRESS)
				{
					me->state = BTN_STATE_WAIT_LONGPRESS;
				}
			}
		}
		else																			/* btn not pressed */
		{
			me->state = BTN_STATE_INITIAL;					/* start over */
		}
	}
	else if(me->state == BTN_STATE_CONTI_PRESS)	/* Contiously press state, only in BTN_MODE_CONTINUOUS_PRESS */
	{
		if(pin_state == me->params.active_state)
		{
			/* btn pressed continuously, call handler*/
			if((now - me->last_time) > (me->params.conti_press_time))
			{
				if(me->handler != NULL)
				{
					me->handler(BTN_PRESS_CONTI, me->reserve);
				}
				me->last_time = now;								
			}
		}
		/* btn is always not pressed, move to wait release state*/
		else
		{
			me->state = BTN_STATE_WAITRELEASE;
		}
	}
	else if(me->state == BTN_STATE_WAIT_LONGPRESS)	/* long press state, only in BTN_MODE_CONTINUOUS_PRESS */
	{
		if(pin_state == me->params.active_state)
		{
			if((now - me->last_time) > me->params.long_press_time)
			{
				if(me->handler != NULL)
				{
					me->handler(BTN_PRESS_LONG, me->reserve);
				}
				me->last_time = now;
				me->state = BTN_STATE_WAITRELEASE;			
			}
		}
		else		/* btn is not pressed, move to waitreleased */
		{
			me->state = BTN_STATE_WAITRELEASE;
		}
	}
	else if(me->state == BTN_STATE_WAITRELEASE)			/* finally in wait release state */
	{
		if(pin_state != me->params.active_state)
		{
			/* released debounce time is past */
			if((now - me->last_time) > me->params.debounce_time)
			{
				if(me->handler != NULL)
				{
					me->handler(BTN_PRESS_RELEASED, me->reserve);
				}
				me->state = BTN_STATE_INITIAL;
			}
		}
	}
}

/**
  * @brief  BTN object re-configuration 
  * @param  me, @ref BTN_t pointer
  * @param	params, button params
  * @param	handler, button handler
	*	@note		This function offers the possiblitiy to RE-CONFIGURE btn mode and btn handler
	*					when application context is changed.
  */
void BTN_config(BTN_t *const me, BTN_Param_t params, btn_handler_t handler)
{
	/* stop btn first */
	me->state = BTN_STATE_IDLE;		
	
	/* Copy input parameters and handlers*/
	me->params = params;
	me->handler = handler;
}



/**
  * @brief  Btn param getter
  * @param  me, @ref BTN_t pointer
  */
BTN_Param_t BTN_getParam(const BTN_t *const me)
{
	return (me->params);
}

/**
  * @brief  Get now time, to be implemented externally,
	* @retval uint32_t now_time in msec
	* @note		Implement this function externally, for example @ref HAL_GetTick()
	*/
__weak uint32_t BTN_now(void)
{
	return 0;
}
	

/************************ (C) COPYRIGHT  YZTek *****END OF FILE****/
