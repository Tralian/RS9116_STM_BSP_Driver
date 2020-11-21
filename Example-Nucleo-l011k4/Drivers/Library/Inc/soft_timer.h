/**
  ******************************************************************************
  * @file    soft_timer.h 
  * @author  YZTek Software TEAM
  * @brief   Header for soft_timer module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2020 YZTek</center></h2>
  *
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SOFT_TIMER_H
#define __SOFT_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

/* Exported constants --------------------------------------------------------*/
#define IFINITY_COUNT						UINT16_MAX		// 49.7 days means infinite period count, not stop.

/* Exported types ------------------------------------------------------------*/
//typedef void (*start_task_t)(void);
//typedef void (*periodic_task_t)(void);
//typedef void (*end_task_t)(void);
//typedef void (*cmr_task_t)(void);

typedef struct
{
	uint32_t	delay;				/* Initial delay offset ticks before periodic tmr, down-counting */
	uint32_t	period;				/* Period in ticks, down-counting */
	uint16_t	req_cnt;			/* Requested counts for periodic task */	
}ST_Init_t;


typedef struct __ST_t
{
	ST_Init_t	init;
	bool	enabled;			/* true if soft tmr is ticking */
	bool	started;			/* true if initial delay is past */
	uint32_t	tick;			/* current down-counting ticks */
	uint16_t	cnt;			/* remaining periodic tasks counts to be performed */
	void (* per_task)(struct __ST_t *tmr);
	void (* start_task)(struct __ST_t *tmr);
	void (* end_task)(struct __ST_t *tmr);
}ST_t;

typedef void (*tmr_task_t)(ST_t *tmr);

typedef struct	__OC_t
{	
	ST_t tmr;							/* inheritance  from soft timer type */
	uint32_t pulse;				/* value to be compared */
	void (* cmr_task)(struct __OC_t *oc_tmr);
}OC_t;

typedef void (*cmr_task_t)(OC_t *oc_tmr);

/* Exported functions ------------------------------------------------------- */

/**@brief   soft timer exported functions */
void ST_ctor( ST_t * const me, uint32_t delay, uint32_t period, uint16_t req_cnt, 
				tmr_task_t start_task, tmr_task_t per_task, tmr_task_t end_task);

uint32_t ST_getTick(ST_t * const me);
void ST_setTick(ST_t * const me, uint32_t tick);
void ST_tickHandler(ST_t * const me);
void ST_startTick(ST_t * const me);
void ST_stopTick(ST_t *const me);

/**@brief 	Output compare exported functions */
void OC_ctor(OC_t * const me, uint32_t period, uint32_t pulse, uint16_t req_cnt,
						tmr_task_t per_task, tmr_task_t end_task, cmr_task_t cmr_task);
void OC_tickHandler(OC_t * const me);
void OC_setPulse(OC_t * const me, uint32_t pulse);
void OC_startTick(OC_t * const me);
void OC_stopTick(OC_t * const me);



#endif /* __SOFT_TIMER_H */
/************************ (C) COPYRIGHT YZTek *****END OF FILE****/
