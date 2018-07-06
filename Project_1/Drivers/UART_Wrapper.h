/*
 *  UART_Wrapper.h
 *
 *  Uart's driver wrapper for
 *  STM32F429ZI Nucleo board platform.
 *  Limited configuration capability.
 *
 *  Created on: 19 de jun de 2018
 *  Author: Gilberto Araujo
 */

#ifndef UART_WRAPPER_H_
#define UART_WRAPPER_H_

#include "stm32f4xx_hal.h"
#include "main.h"

/*
 * UART Public Definitions
 */
typedef enum {
	UART_1,		/*!<  */
	UART_2,		/*!<  */
	UART_3,		/*!<  */
	UART_4,		/*!<  */
	UART_5,		/*!<  */
	UART_6,		/*!<  */
	UART_7,		/*!<  */
	UART_8		/*!<  */
} uartId_t;

/* Config parameters */
typedef enum {
	/* Word Lenght parameters */
	UART_WLEN_8 = UART_WORDLENGTH_8B,   /* 8 bit data    */
	UART_WLEN_9 = UART_WORDLENGTH_9B,   /* 9 bit data    */
	/* Stop Bits parameters */
	UART_STOP_ONE = UART_STOPBITS_1,  	/* One stop bit  */
	UART_STOP_TWO = UART_STOPBITS_2,  	/* Two stop bits */
	/* Parity parameters */
	UART_PAR_NONE = UART_PARITY_NONE,   /* No parity     */
	UART_PAR_EVEN = UART_PARITY_EVEN,   /* Even parity   */
	UART_PAR_ODD = UART_PARITY_ODD,     /* Odd parity    */
} uartCfg_t;

/* FIFO Interrupt Trigger Level */
typedef enum {
	UART_FIFO_TX_2_8  =	DMA_FIFO_THRESHOLD_1QUARTERFULL,   	/* Transmit interrupt at 1/4 Full */
	UART_FIFO_TX_4_8  =	DMA_FIFO_THRESHOLD_HALFFULL,   		/* Transmit interrupt at 1/2 Full */
	UART_FIFO_TX_6_8  =	DMA_FIFO_THRESHOLD_3QUARTERSFULL,	/* Transmit interrupt at 3/4 Full */
	UART_FIFO_TX_FULL = DMA_FIFO_THRESHOLD_FULL,  			/* Transmit interrupt at Full     */
	UART_FIFO_RX_2_8  =	DMA_FIFO_THRESHOLD_1QUARTERFULL,   	/* Receive interrupt at 1/4 Full  */
	UART_FIFO_RX_4_8  =	DMA_FIFO_THRESHOLD_HALFFULL,   		/* Receive interrupt at 1/2 Full  */
	UART_FIFO_RX_6_8  =	DMA_FIFO_THRESHOLD_3QUARTERSFULL,	/* Receive interrupt at 3/4 Full  */
	UART_FIFO_RX_FULL = DMA_FIFO_THRESHOLD_FULL			   	/* Receive interrupt at Full      */
} uartFIFO_t;

/* Interrupt flags */
typedef enum {
	UART_FLAG_DMATX,    /* DMA TX interrupt               */
	UART_FLAG_DMARX,    /* DMA RX interrupt               */
	UART_FLAG_9BIT,     /* 9-bit address match interrupt  */
	UART_FLAG_OE,       /* Overrun Error Interrupt Mask   */
	UART_FLAG_BE,       /* Break Error Interrupt Mask     */
	UART__FLAG_PE,      /* Parity Error Interrupt Mask    */
	UART__FLAG_FE,      /* Framing Error Interrupt Mask   */
	UART_FLAG_RT,       /* Receive Timeout Interrupt Mask */
	UART_FLAG_TX,       /* Transmit Interrupt Mask        */
	UART_FLAG_RX,       /* Receive Interrupt Mask         */
	UART_FLAG_DSR,      /* DSR Modem Interrupt Mask       */
	UART_FLAG_DCD,      /* DCD Modem Interrupt Mask       */
	UART__FLAG_CTS,     /* CTS Modem Interrupt Mask       */
	UART_FLAG_RI        /* RI Modem Interrupt Mask        */
} uartFlag_t;

/*
 * UART Public Functions
 */
extern void UART_Init(uartId_t uart);
extern void UART_Config(uartId_t uart, uint32_t baudRate, uartCfg_t wlen,
						uartCfg_t sbits, uartCfg_t parity, uint32_t flag);
extern void UART_ConfigFIFO(uartId_t uart, uartFIFO_t txLevel,
							uartFIFO_t rxLevel, bool enable);
extern void UART_IntClear(uartId_t uart, uint32_t flag);
extern bool UART_WriteChar(uartId_t uart, uint8_t c);
extern uint32_t UART_WriteStr(uartId_t uart, uint8_t data[]);
extern uint8_t UART_ReadChar(uartId_t uart);
extern void UART_ReadCount(uartId_t uart, int32_t buf[], uint32_t count);
extern uint32_t UART_ReadAll(uartId_t uart, int32_t buf[]);


#endif /* UART_WRAPPER_H_ */
