/**
  ******************************************************************************
  * @file    soft_timer.c 
  * @author  YZTek Software TEAM
  * @brief   soft_timer module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020 YZTek</center></h2>
  *
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "soft_timer.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
	void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif /* USE_FULL_ASSERT */
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions implementation ------------------------------------------*/
/* Exported functions implementation ---------------------------------------- */

/**@brief  Soft timer exported function implementations */
void ST_ctor( ST_t * const me, uint32_t delay, uint32_t period, uint16_t req_cnt, 
				tmr_task_t start_task, tmr_task_t per_task, tmr_task_t	end_task)
{
	/* Reset timer flags*/
	me->enabled = false;
	me->started = false;
	
	/* Update Init params */
	me->init.delay = delay;
	me->init.period = period;
	me->init.req_cnt = req_cnt;	
	
	/* Initialize tick and cnt */	
	me->tick = (delay == 0) ? period : delay ;
	me->cnt = req_cnt;
	
	/* register callback functions*/
	me->start_task = start_task;
	me->per_task = per_task;
	me->end_task = end_task;	
}

void ST_tickHandler(ST_t * const me)
{
	/* Handle only if it's enabled */
	if(me->enabled)
	{	
		/* Execute delay offset down-counting */
		if(!me->started)
		{
			me->tick--;
			if(me->tick == 0)
			{				
				/* Reload tick to period*/
				me->tick = me->init.period;				
				/* Perform intial start task */
				if(me->start_task != NULL)
				{
					me->start_task(me);
				}
				me->started = true;
			}
		}
		/* Start periodic counter down-counting */
		else if(me->started && (me->tick > 0))
		{
			/* tick_count decrement */
			me->tick--;
			
			/* One period past */
			if(me->tick == 0)
			{
				/* Reload tick_count*/
				me->tick = me->init.period;		
				
				/* Perform registed periodic task */
				if(me->per_task != NULL)
				{
					me->per_task(me);
				}
				/* update_cnt decrement */
				if((me->cnt > 0) && (me->cnt != IFINITY_COUNT))
				{
					me->cnt --;
					if(me->cnt == 0 )						
					{
						/* Perform registed end task*/
						if(me->end_task != NULL)
						{
							me->end_task(me);
						}
						/* Disable periodic handle*/
						me->enabled = false;			
						me->started = false;
						
						/* Reload tick and cnt in case of future use*/
						me->tick = (me->init.delay == 0) ? (me->init.period) : (me->init.delay);
						me->cnt = me->init.req_cnt;
					}				
				}
			}
		}
	}
}

void ST_startTick(ST_t * const me)
{
	me->enabled = true;
	/**@ToDo: reload params? */
	me->tick = (me->init.delay == 0) ? (me->init.period) : (me->init.delay) ;
	me->cnt = me->init.req_cnt;
	
	/* Perform start task immediately if it's not NULL and delay = 0*/
	if(me->init.delay == 0 && (me->start_task != NULL))
	{
		me->start_task(me);
		me->started = true;
	}
}

void ST_stopTick(ST_t *const me)
{
	if(me->end_task != NULL)
	{
		me->end_task(me);
	}
	me->enabled = false;
	/**@ToDo: reload params? */
}

uint32_t ST_getTick(ST_t * const me)
{
	return me->tick;
}
void ST_setTick(ST_t * const me, uint32_t tick)
{
	assert_param( (me->tick) <= (me->init.delay));
	me->tick = tick;	
}

/**@brief  Ouput compare timer exported function implementations */

void OC_ctor(OC_t * const me, uint32_t period, uint32_t pulse, uint16_t req_cnt,
						tmr_task_t per_task, tmr_task_t end_task, cmr_task_t cmr_task)
{
	/* Check compare value is less than period value */
	assert_param(pulse < period);	
	
	/* base object soft timer ctor, set tick to 1, hence immediately triiger per_task*/	
	ST_ctor((ST_t *)me, 0, period, req_cnt, NULL, per_task, end_task);
	ST_setTick((ST_t *)me, 1);
	/* Modify counter to match req_cnt */
	if( me->tmr.init.req_cnt != IFINITY_COUNT)
	{
		me->tmr.init.req_cnt++;
		me->tmr.cnt ++;
	}
	
	/* Initial output compare params*/
	me->pulse = pulse;	
	me->cmr_task = cmr_task;		
}

void OC_tickHandler(OC_t * const me)
{
	if(me->tmr.enabled)
	{
		/* execure base oject soft timer tick handler*/
		ST_tickHandler((ST_t *)me);
		if((me->tmr.tick == me->pulse) && (me->cmr_task != NULL))
		{
			me->cmr_task(me);
		}
	}
}

void OC_startTick(OC_t * const me)
{
	/**@ToDo: reload params? */
	ST_setTick((ST_t *)me, 1);
	me->tmr.cnt = me->tmr.init.req_cnt;
	
	me->tmr.enabled = true;
	me->tmr.started = true;
}

void OC_stopTick(OC_t * const me)
{
	ST_stopTick((ST_t *)me);	
}

void OC_setPulse(OC_t * const me, uint32_t pulse)
{
	assert_param(pulse < (me->tmr.init.period));
	me->pulse = pulse;
}









/************************ (C) COPYRIGHT YZTek *****END OF FILE****/
