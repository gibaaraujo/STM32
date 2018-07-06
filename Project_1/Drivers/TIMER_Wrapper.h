/*
 *  TIMER_Wrapper.h
 *
 *  Timer's driver wrapper for
 *  STM32F429ZI Nucleo board platform.
 *  Limited configuration capability.
 *
 *  Created on: 16 de jun de 2018
 *  Author: Gilberto Araujo
 */

#ifndef TIMER_WRAPPER_H_
#define TIMER_WRAPPER_H_

#include "stm32f4xx_hal.h"
#include "main.h"

/*
 * TIMER Public Definitions
 */

/* Timer identifiers */
typedef enum {
	TIMER_1,	/*!< Advanced 16-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIMER_2,    /*!< 32-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIMER_3,    /*!< 16-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIMER_4,    /*!< 16-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIMER_5,    /*!< 32-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIMER_6,    /*!< Basic 16-bit auto-reload upcounter, 16-bit prescaler */
	TIMER_7,    /*!< Basic 16-bit auto-reload upcounter, 16-bit prescaler */
	TIMER_8,    /*!< Advanced 16-bit up, down, up/down auto-reload, 16-bit  prescaler */
	TIMER_9,    /*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIMER_10,   /*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIMER_11,   /*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIMER_12,   /*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIMER_13,   /*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIMER_14    /*!< 16-bit auto-reload upcounter, 16-bit prescaler */
} timerId_t;

/* Config parameters */
typedef enum {
	TIMER_PERIODIC,
	TIMER_PERIODIC_UP,
	TIMER_ONESHOT,
	TIMER_ONESHOT_UP
} cfg_t;

/*
 * TIMER Public Functions
 */
extern void TIMER_Init(timerId_t timer);
extern void TIMER_Config(timerId_t timer, cfg_t param);
extern void TIMER_ConfigPrescaler(timerId_t timer, uint16_t freq);
extern void TIMER_Load(timerId_t timer, uint32_t us);
extern uint32_t TIMER_GetCount(timerId_t timer);
extern void TIMER_IntClear(timerId_t timer);
extern void TIMER_Enable(timerId_t timer);
extern void TIMER_Disable(timerId_t timer);


#endif /* TIMER_WRAPPER_H_ */
