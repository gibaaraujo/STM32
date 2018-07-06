/*
 *  UART_Wrapper.c
 *
 *  Uart's driver wrapper for
 *  STM32F429ZI Nucleo board platform.
 *  Limited configuration capability.
 *
 *  Created on: 19 de jun de 2018
 *  Author: Gilberto Araujo
 */

#include <stdbool.h>
#include <string.h>
#include "UART_Wrapper.h"

/*
 * Private Definitions
 */

#define FIFO_LEN		16
#define WRITE_TIMEOUT	1		/*!< Timeout duration in milliseconds */
#define READ_TIMEOUT	1		/*!< Timeout duration in milliseconds */

/* GPIO mapping element type */
typedef struct {
	uint8_t			gpio_AF;	/*!< GPIO Alternate Function */
	GPIO_TypeDef*	port_Tx;	/*!< GPIO Tx port */
	uint16_t		pin_Tx;		/*!< GPIO Tx pin */
	GPIO_TypeDef*	port_Rx;	/*!< GPIO Rx port */
	uint16_t		pin_Rx;		/*!< GPIO Rx pin */
} gpio_t;

/*
 * Private Data
 */

/** UART instance identifiers */
static
USART_TypeDef* id[] = {
	USART1,
	USART2,
	USART3,
	UART4,
	UART5,
	USART6,
	UART7,
	UART8
};

/** UART GPIO Port/Pin mapping */
static const
gpio_t gpio[] = {
/*      gpio_AF     , port_TX,  pin_TX  ,  port_RX,  pin_RX    */
	{GPIO_AF7_USART1, GPIOA,  GPIO_PIN_9,  GPIOA,  GPIO_PIN_10},
	{GPIO_AF7_USART2, GPIOA,  GPIO_PIN_2,  GPIOA,  GPIO_PIN_3},
	{GPIO_AF7_USART3, GPIOD,  GPIO_PIN_8,  GPIOD,  GPIO_PIN_9},
	{GPIO_AF8_UART4,  GPIOC,  GPIO_PIN_10, GPIOC,  GPIO_PIN_11},
	{GPIO_AF8_UART5,  GPIOC,  GPIO_PIN_12, GPIOD,  GPIO_PIN_2},
	{GPIO_AF8_USART6, GPIOC,  GPIO_PIN_6,  GPIOC,  GPIO_PIN_7},
	{GPIO_AF8_UART7,  GPIOE,  GPIO_PIN_8,  GPIOE,  GPIO_PIN_7},
	{GPIO_AF8_UART8,  GPIOE,  GPIO_PIN_1,  GPIOE,  GPIO_PIN_0}
};

/** UART internal handles */
static UART_HandleTypeDef huart[8];

/** DMA used for UART Rx transfers */
DMA_HandleTypeDef hRxDMA;

/** FIFO Buffers */
uint8_t txFIFO[FIFO_LEN];
uint8_t rxFIFO[FIFO_LEN];

/**
 *  UART initialization
 */
void UART_Init(uartId_t uart)
{
	/* Config UART instance */
	huart[uart].Instance = id[uart];
	huart[uart].pTxBuffPtr = txFIFO;
	huart[uart].pRxBuffPtr = rxFIFO;
	huart[uart].TxXferSize = FIFO_LEN;
	huart[uart].RxXferSize = FIFO_LEN;
	huart[uart].TxXferCount;
	huart[uart].RxXferCount;
}
/**
 *  UART Configuration
 */
void UART_Config(uartId_t uart, uint32_t baudRate, uartCfg_t wlen,
				 uartCfg_t sbits, uartCfg_t parity, uint32_t flag)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	HAL_StatusTypeDef status;

	/* Config UART parameters */
	huart[uart].Init.BaudRate = baudRate;
	huart[uart].Init.WordLength = wlen;
	huart[uart].Init.StopBits = sbits;
	huart[uart].Init.Parity = parity;
	huart[uart].Init.Mode = UART_MODE_TX_RX;
	huart[uart].Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart[uart].Init.OverSampling = UART_OVERSAMPLING_16;
	status = HAL_UART_Init(&huart[uart]);

    /*
     * UART MSP config
     */

    /* Config UART Tx pin */
    GPIO_InitStruct.Pin = gpio[uart].pin_Tx;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = gpio[uart].gpio_AF;
    HAL_GPIO_Init(gpio[uart].port_Tx, &GPIO_InitStruct);

    /* Config UART Rx pin
     * NOTE: Separate configuration was needed because some Tx and Rx pins
     *       are on different GPIO ports (e.g. UART5: Tx->PC12, Rx->PD2) */
    if (gpio[uart].port_Tx == gpio[uart].port_Rx)
    {
    	GPIO_InitStruct.Pin |= gpio[uart].pin_Rx;
    }
    else
    {
    	GPIO_InitStruct.Pin = gpio[uart].pin_Rx;
    }

    HAL_GPIO_Init(gpio[uart].port_Tx, &GPIO_InitStruct);

	/* Enables clock to UART module and GPIO port */
	switch (uart)
	{
		case UART_1:
			__HAL_RCC_USART1_CLK_ENABLE();
			__HAL_RCC_GPIOA_CLK_ENABLE();
			break;
		case UART_2:
			__HAL_RCC_USART2_CLK_ENABLE();
			__HAL_RCC_GPIOA_CLK_ENABLE();
			break;
		case UART_3:
			__HAL_RCC_USART3_CLK_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();
			break;
		case UART_4:
			__HAL_RCC_UART4_CLK_ENABLE();
			__HAL_RCC_GPIOC_CLK_ENABLE();
			break;
		case UART_5:
			__HAL_RCC_UART5_CLK_ENABLE();
			__HAL_RCC_GPIOC_CLK_ENABLE();
			__HAL_RCC_GPIOD_CLK_ENABLE();
			break;
		case UART_6:
			__HAL_RCC_USART6_CLK_ENABLE();
			__HAL_RCC_GPIOC_CLK_ENABLE();
			break;
		case UART_7:
			__HAL_RCC_UART7_CLK_ENABLE();
			__HAL_RCC_GPIOE_CLK_ENABLE();
			break;
		case UART_8:
			__HAL_RCC_UART8_CLK_ENABLE();
			__HAL_RCC_GPIOE_CLK_ENABLE();
			break;
		default:
			status = HAL_ERROR;
			break;
	}

	/* Check for config error */
	if (status != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

    /* Enables Receive Data register not empty
       and the Idle line detected interrupts */
    __HAL_UART_ENABLE_IT(&huart[uart], UART_IT_RXNE | UART_IT_IDLE);

    /* UART global interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
}
/**
 *  UART FIFO Configuration
 *  Config interrupt trigger levels and enable or disable FIFOs.
 *  Each Rx and Tx FIFO is 16 words deep.
 */
void UART_ConfigFIFO(uartId_t uart, uartFIFO_t txLevel, uartFIFO_t rxLevel, bool enable)
{
    /* UART DMA Rx Init */
    hRxDMA.Instance = DMA1_Stream1;
    hRxDMA.Init.Channel = DMA_CHANNEL_4;
    hRxDMA.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hRxDMA.Init.PeriphInc = DMA_PINC_DISABLE;
    hRxDMA.Init.MemInc = DMA_MINC_ENABLE;
    hRxDMA.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hRxDMA.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hRxDMA.Init.Mode = DMA_NORMAL;
    hRxDMA.Init.Priority = DMA_PRIORITY_LOW;
    hRxDMA.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hRxDMA.Init.FIFOThreshold = rxLevel;
    hRxDMA.Init.MemBurst = DMA_MBURST_SINGLE;
    hRxDMA.Init.PeriphBurst = DMA_PBURST_SINGLE;
    if (HAL_DMA_Init(&hRxDMA) != HAL_OK)
    {
      _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(&huart[uart], hdmarx, hRxDMA);

    /* DMA clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
}
/**
 *  UART Interrupt Clear
 *  To be called on top of 'uart' ISR
 */
void UART_IntClear(uartId_t uart, uint32_t flag)
{
	/* Clear the uart interrupt */
	__HAL_UART_CLEAR_FLAG(&huart[uart], flag);
}
/**
 *  UART Write Character Function
 *  Writes byte on 'uart'
 *  Returns 'true' if the character was successfully placed in the
 *  transmit FIFO or 'false' if there was no space available.
 */
bool UART_WriteChar(uartId_t uart, uint8_t c)
{
	bool writeStatus;

	if (HAL_UART_Transmit(&huart[uart], &c, 1, WRITE_TIMEOUT))
		writeStatus = true;
	else
		writeStatus = false;

	return writeStatus;
}
/**
 *  UART Write String Function
 *  Writes data string on 'uart'.
 *  Returns the number of bytes written.
 */
uint32_t UART_WriteStr(uartId_t uart, uint8_t data[])
{
	uint32_t i, size;

	size = strlen((char*)data);
	for (i=0 ; i<size ; i++)
	{
		HAL_UART_Transmit(&huart[uart], data, 1, WRITE_TIMEOUT);
	}
	return i;
}
/**
 *  UART Read Character Function
 *  Returns the byte read.
 */
uint8_t UART_ReadChar(uartId_t uart)
{
	uint8_t data = 0;

	/* Receives a char data in non blocking mode */
	HAL_UART_Receive_IT(&huart[uart], &data, 1);

	return data;
}
/**
 *  UART Read Function
 *  Read 'count' words from 'uart' to 'buf'.
 *  Each word contains a data byte (bits 7-0) plus four
 *  error flags (bits 11-8): OverRun, Break, Parity and Framing.
 */
void UART_ReadCount(uartId_t uart, int32_t buf[], uint32_t count)
{
	uint32_t i;

	for (i=0; i<count; i++)
	{
		/* Receives an amount of data in non blocking mode */
		HAL_UART_Receive_IT(&huart[uart], (uint8_t*)&buf[i], 1);
	}
}
/**
 *  UART Read Function
 *  Read all words available from 'uart' to 'buf'.
 *  Each word contains a data byte (bits 7-0) plus four
 *  error flags (bits 11-8): OverRun, Break, Parity and Framing.
 *  Returns the number of words read.
 */
uint32_t UART_ReadAll(uartId_t uart, int32_t buf[])
{
	uint32_t i = 0;

	/* Checks for Receive Data register not empty */
	while (__HAL_UART_GET_FLAG(&huart[uart], UART_IT_RXNE))
	{
		/* Receives an amount of data in non blocking mode */
		HAL_UART_Receive_IT(&huart[uart], (uint8_t*)&buf[i], 1);
		i++;
	}
	return i;
}
