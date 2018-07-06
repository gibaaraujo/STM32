/*
 * touch.h
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

#ifndef TOUCH_H_
#define TOUCH_H_

/*
 * Function prototypes
 */
extern void GUI_TOUCH_X_ActivateX();
extern void GUI_TOUCH_X_ActivateY();
extern int GUI_TOUCH_X_MeasureX();
extern int GUI_TOUCH_X_MeasureY();

#endif /* TOUCH_H_ */
