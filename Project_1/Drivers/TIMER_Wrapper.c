/*
 *  TIMER_Wrapper.c
 *
 *  Timer's driver wrapper for
 *  STM32F429ZI Nucleo board platform.
 *  Limited configuration capability.
 *
 *  Created on: 16 de jun de 2018
 *  Author: Gilberto Araujo
 */

#include "TIMER_Wrapper.h"

/** TIMER instance identifiers */
static
TIM_TypeDef * id[] = {
	TIM1,	/*!< Advanced 16-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIM2,	/*!< 32-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIM3,	/*!< 16-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIM4,	/*!< 16-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIM5,	/*!< 32-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIM6,	/*!< Basic 16-bit auto-reload upcounter, 16-bit prescaler */
	TIM7,	/*!< Basic 16-bit auto-reload upcounter, 16-bit prescaler */
	TIM8,	/*!< Advanced 16-bit up, down, up/down auto-reload, 16-bit prescaler */
	TIM9,	/*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIM10,	/*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIM11,	/*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIM12,	/*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIM13,	/*!< 16-bit auto-reload upcounter, 16-bit prescaler */
	TIM14	/*!< 16-bit auto-reload upcounter, 16-bit prescaler */
};

/*
 * Private Data
 */

/** TIMER internal handles */
static TIM_HandleTypeDef htim[14];

/**
 *  Pointer to the function that returns the timer clock source.
 *  NOTE: The timers connected to APB2 are clocked from TIMxCLK up to 180 MHz,
 *  	  while the timers connected to APB1 are clocked from TIMxCLK either
 *  	  up to 90 MHz or 180 MHz depending on TIMPRE bit configuration in the
 *  	  RCC_DCKCFGR register (STM32F429xx Datasheet, page 19).
 */
static uint32_t (*getTimerClock)(void) = NULL;

/**
 *	Timer Initialization
 */
void TIMER_Init(timerId_t timer)
{
	/* Config timer instance */
	htim[timer].Instance = id[timer];

	/* Finds the timer clock source */
	if ((timer == TIMER_1) || (timer == TIMER_8) || (timer == TIMER_9) ||
		(timer == TIMER_10) || (timer == TIMER_11))
	{
		/* Timers uses the APB2 bus clock source (up to 180 MHz) */
		getTimerClock = HAL_RCC_GetPCLK2Freq;
	}
	else
	{
		/* Timers uses the APB1 bus clock source (up to 90 MHz) */
		getTimerClock = HAL_RCC_GetPCLK1Freq;
	}
}
/**
 *  Timer config
 *  NOTE:  Some timer instances do not allow all of the config options
 *  	   available in \see cfg_t type parameters. Please check the
 *  	   \see timerId_t type for more information.
 */
void TIMER_Config(timerId_t timer, cfg_t param)
{
	HAL_StatusTypeDef status;

	/* Stops the timer */
	HAL_TIM_Base_Stop(&htim[timer]);

	/* Config timer */
	switch (param)
	{
		case TIMER_PERIODIC:
			htim[timer].Init.CounterMode = TIM_COUNTERMODE_DOWN;
			status = HAL_TIM_Base_Init(&htim[timer]);
			break;
		case TIMER_PERIODIC_UP:
			htim[timer].Init.CounterMode = TIM_COUNTERMODE_UP;
			status = HAL_TIM_Base_Init(&htim[timer]);
			break;
		case TIMER_ONESHOT:
			htim[timer].Init.CounterMode = TIM_COUNTERMODE_DOWN;
			status = HAL_TIM_OnePulse_Init(&htim[timer], TIM_OPMODE_REPETITIVE);
			break;
		case TIMER_ONESHOT_UP:
			htim[timer].Init.CounterMode = TIM_COUNTERMODE_UP;
			status = HAL_TIM_OnePulse_Init(&htim[timer], TIM_OPMODE_REPETITIVE);
			break;
		default:
			status = HAL_ERROR;
			break;
	}

	/* Enables clock to timer */
	switch (timer)
	{
		case TIMER_1:	__HAL_RCC_TIM1_CLK_ENABLE();
			break;
		case TIMER_2:	__HAL_RCC_TIM2_CLK_ENABLE();
			break;
		case TIMER_3:	__HAL_RCC_TIM3_CLK_ENABLE();
			break;
		case TIMER_4:	__HAL_RCC_TIM4_CLK_ENABLE();
			break;
		case TIMER_5:	__HAL_RCC_TIM5_CLK_ENABLE();
			break;
		case TIMER_6:	__HAL_RCC_TIM6_CLK_ENABLE();
			break;
		case TIMER_7:	__HAL_RCC_TIM7_CLK_ENABLE();
			break;
		case TIMER_8:	__HAL_RCC_TIM8_CLK_ENABLE();
			break;
		case TIMER_9:	__HAL_RCC_TIM9_CLK_ENABLE();
			break;
		case TIMER_10:	__HAL_RCC_TIM10_CLK_ENABLE();
			break;
		case TIMER_11:	__HAL_RCC_TIM11_CLK_ENABLE();
			break;
		case TIMER_12:	__HAL_RCC_TIM12_CLK_ENABLE();
			break;
		case TIMER_13:	__HAL_RCC_TIM13_CLK_ENABLE();
			break;
		case TIMER_14:	__HAL_RCC_TIM14_CLK_ENABLE();
			break;
		default:		status = HAL_ERROR;
			break;
	}

	/* Check for config error */
	if (status != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
}
/**
 *  Timer prescaler config.
 *  Sets a new frequency (in Hz) to increment the timer by
 *  dividing that of the internal clock source (APB1 or APB2).
 */
void TIMER_ConfigPrescaler(timerId_t timer, uint16_t freq)
{
	/* Stops the timer */
	HAL_TIM_Base_Stop(&htim[timer]);

	/* Config prescaler value */
	htim[timer].Init.Prescaler = (*getTimerClock)() / freq;
}
/**
 *  Timer load
 *  Reload value in microseconds.
 */
void TIMER_Load(timerId_t timer, uint32_t us)
{
	/* Config timer period */
	htim[timer].Init.Period = (*getTimerClock)() / 1000000 * us;
}
/**
 *  Returns timer count
 */
uint32_t TIMER_GetCount(timerId_t timer)
{
	/* Returns the counter register */
	return htim[timer].Instance->CNT;
}
/**
 *  Timer Interrupt Clear
 *  To be called on top of Timer ISR.
 */
void TIMER_IntClear(timerId_t timer)
{

}
/**
 *  Timer start
 */
void TIMER_Enable(timerId_t timer)
{
	/* Starts the timer */
    HAL_TIM_Base_Start(&htim[timer]);
}
/**
 *  Timer stop
 */
void TIMER_Disable(timerId_t timer)
{
	/* Stops the timer */
	HAL_TIM_Base_Stop(&htim[timer]);
}
