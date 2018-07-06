/**
 *  @file SYS.h
 *
 *  System Control driver wrapper for
 *  STM32F429ZI Nucleo board platform.
 *  Limited configuration capability.
 *
 *  Created on: 12 de nov de 2017
 *  Author: Gilberto Araujo
 */

#ifndef SYS_H_
#define SYS_H_

/**
 * SYS Public Definitions
 */

/**
 * System Clock definitions
 * Note: Some configurations with internal clock source (HSI),
 * 		 do not supports all peripheral clocks (e.g. 48MHz USB clocking).
 * 		 Clock source definitions are as follows:
 * 		 (INT) from High Speed Internal (HSI) 16 MHz Oscillator.
 * 		 (EXT) from High Speed External (HSE)  8 MHz Clock.
 * 		 (XTAL) from External Crystal/Ceramic Resonator - 8 MHz.
 */
typedef enum {
	SYS_CLOCK_16_MHZ_INT,
	SYS_CLOCK_16_MHZ_EXT,
	SYS_CLOCK_16_MHZ_XTAL,
	SYS_CLOCK_72_MHZ_INT,
	SYS_CLOCK_72_MHZ_EXT,
	SYS_CLOCK_72_MHZ_XTAL,
	SYS_CLOCK_120_MHZ_INT,
	SYS_CLOCK_120_MHZ_EXT,
	SYS_CLOCK_120_MHZ_XTAL,
	SYS_CLOCK_168_MHZ_INT,
	SYS_CLOCK_168_MHZ_EXT,
	SYS_CLOCK_168_MHZ_XTAL,
	SYS_CLOCK_180_MHZ_INT,
	SYS_CLOCK_180_MHZ_EXT,
	SYS_CLOCK_180_MHZ_XTAL
} sysClk_t;

/**
 * System power mode definitions
 */
typedef enum {
	SYS_PWR_MODE_SLEEP,
	SYS_PWR_MODE_DEEPSLEEP
}sysMode_t;

/*
 * SYS Public Functions
 */
extern void SYS_Init();
extern void SYS_ClockSet(sysClk_t sysClock);
extern uint32_t SYS_ClockGet();
extern void SYS_PowerModeSet(sysMode_t mode);

#endif /* SYS_H_ */
