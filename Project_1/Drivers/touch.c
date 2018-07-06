/*
 * touch.c
 *
 *	Touch Screen Interface
 *
 *  Implements the touch screen hardware interface for
 *  Kentec 320x240 TFT Display Board BOOSTXL-K350QVG-S1,
 *  based on STMicroeletronics STM32F429ZI Nucleo board
 *  and STemWin graphic library
 *
 *  Created on: 28 de out de 2017
 *  Author: Gilberto Araujo
 */

#include "stm32f4xx_hal.h"
#include "LCD.h"
#include "gpio.h"
#include "adc.h"
#include "touch.h"

#define	N					5	/* Divisor by right shift */
#define	ADC_AVERAGE			32	/* Average of (2^N) ADC samples */
#define	ADC_TIMEOUT			1	/* ADC conversion timeout in milliseconds */
#define	ADC_CONV_DELAY		100	/* Additional delay between samples conversion */
/* Extended sampling time for better samples averaging */
#define	ADC_SAMPLETIME		ADC_SAMPLETIME_480CYCLES

/*
 * Private functions
 */
static void Discharge_X();
static void Discharge_Y();
static void Interchangeable_Activate_X();
static void Interchangeable_Activate_Y();

/*
 * Prepares measurement for X-axis.
 */
static
void Interchangeable_Activate_X()
{
	ADC_ChannelConfTypeDef sConfig;
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Discharges the axis */
	Discharge_X();

	/* Configures Y pins to outputs */
	GPIO_InitStruct.Pin = TOUCH_YP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_YP_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = TOUCH_YN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_YN_GPIO_Port, &GPIO_InitStruct);

	/* Configures X- pin to floating input */
	GPIO_InitStruct.Pin = TOUCH_XN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_XN_GPIO_Port, &GPIO_InitStruct);

	/* Configures X+ pin to ADC input channel */
	sConfig.Channel = ADC_CHANNEL_13;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);

	GPIO_InitStruct.Pin = TOUCH_XP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TOUCH_XP_GPIO_Port, &GPIO_InitStruct);

	/* Drives the Y- pin with GND */
	HAL_GPIO_WritePin(TOUCH_YN_GPIO_Port, TOUCH_YN_Pin, GPIO_PIN_RESET);

    /* Drives the Y+ pin with VDD */
	HAL_GPIO_WritePin(TOUCH_YP_GPIO_Port, TOUCH_YP_Pin, GPIO_PIN_SET);
}

/*
 * Prepares measurement for Y-axis.
 */
static
void Interchangeable_Activate_Y()
{
	ADC_ChannelConfTypeDef sConfig;
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Discharges the axis */
	Discharge_Y();

	/* Configures X pins to outputs */
	GPIO_InitStruct.Pin = TOUCH_XP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_XP_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = TOUCH_XN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_XN_GPIO_Port, &GPIO_InitStruct);

	/* Configures Y- pin to floating input */
	GPIO_InitStruct.Pin = TOUCH_YN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_YN_GPIO_Port, &GPIO_InitStruct);

	/* Configures Y+ pin to ADC input channel */
	sConfig.Channel = ADC_CHANNEL_10;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME;
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);

	GPIO_InitStruct.Pin = TOUCH_YP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(TOUCH_YP_GPIO_Port, &GPIO_InitStruct);

	/* Drives the X- pin with GND */
	HAL_GPIO_WritePin(TOUCH_XN_GPIO_Port, TOUCH_XN_Pin, GPIO_PIN_RESET);

    /* Drives the X+ pin with VDD */
	HAL_GPIO_WritePin(TOUCH_XP_GPIO_Port, TOUCH_XP_Pin, GPIO_PIN_SET);
}

/*
 * Returns the X-result of the ADC.
 */
int GUI_TOUCH_X_MeasureX()
{
	volatile uint32_t  i,j;
	uint32_t sum = 0;

	/* Performs averaging of (2^N) ADC samples */
	for (i=0; i < ADC_AVERAGE; i++)
	{
		HAL_ADC_Start(&hadc3);
		if (HAL_ADC_PollForConversion(&hadc3, ADC_TIMEOUT) == HAL_OK)
			sum += HAL_ADC_GetValue(&hadc3);
		else
			sum += 0;
		/* Short delay between samples */
		for(j=0; j<ADC_CONV_DELAY; j++) {}
	}
	return (sum >>= N);
}

/*
 * Returns the Y-result of the ADC.
 */
int GUI_TOUCH_X_MeasureY()
{
	volatile uint32_t  i,j;
	uint32_t sum = 0;

	/* Performs averaging of (2^N) ADC samples */
	for (i=0; i < ADC_AVERAGE; i++)
	{
		HAL_ADC_Start(&hadc3);
		if (HAL_ADC_PollForConversion(&hadc3, ADC_TIMEOUT) == HAL_OK)
			sum += HAL_ADC_GetValue(&hadc3);
		else
			sum += 0;
		/* Short delay between samples */
		for(j=0; j<ADC_CONV_DELAY; j++) {}
	}
	return (sum >>= N);
}
/*
 * Discharges the Y axis.
 */
static
void Discharge_Y()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*
	 * Configures Y pins as outputs and drives
	 * them to GND to discharge residual voltages.
	 */
	GPIO_InitStruct.Pin = TOUCH_YP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_YP_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = TOUCH_YN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_YN_GPIO_Port, &GPIO_InitStruct);

	/* Drives the Y+ and Y- pins with GND */
	HAL_GPIO_WritePin(TOUCH_YN_GPIO_Port, TOUCH_YN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TOUCH_YP_GPIO_Port, TOUCH_YP_Pin, GPIO_PIN_RESET);
}
/*
 * Discharges the X axis.
 */
static
void Discharge_X()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	/*
	 * Configures X pins as outputs and drives
	 * them to GND to discharge residual voltages.
	 */
	GPIO_InitStruct.Pin = TOUCH_XP_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_XP_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = TOUCH_XN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TOUCH_XN_GPIO_Port, &GPIO_InitStruct);

	/* Drives the X+ and X- pins with GND */
	HAL_GPIO_WritePin(TOUCH_XN_GPIO_Port, TOUCH_XN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TOUCH_XP_GPIO_Port, TOUCH_XP_Pin, GPIO_PIN_RESET);
}

/*
 * Prepares measurement for Y-axis.
 */
void GUI_TOUCH_X_ActivateX()
{
	if (LCD_GetSwapXY())
		Interchangeable_Activate_Y();
	else
		Interchangeable_Activate_X();
}
/*
 * Prepares measurement for X-axis.
 */
void GUI_TOUCH_X_ActivateY()
{
	if (LCD_GetSwapXY())
		Interchangeable_Activate_X();
	else
		Interchangeable_Activate_Y();
}

