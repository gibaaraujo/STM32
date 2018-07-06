/*
 * Kentec_SSD2119.c
 *
 *	SPI 4-Wire Interface for SSD2119 LCD Controller
 *
 *  Implements the device driver hardware interface
 *  for 320x240 TFT Display Board BOOSTXL-K350QVG-S1,
 *  based on STMicroeletronics STM32F429ZI Nucleo board
 *  and STemWin graphic library
 *
 *  Created on: 14 de out de 2017
 *  Author: Gilberto Araujo
 */
#include <Global.h>
#include "stm32f4xx_hal.h"
#include "Kentec_SSD2119.h"

/* Handle to SPI port in use */
extern SPI_HandleTypeDef hspi3;

/*
 * LCD Init
 */
void LCD_Init()
{
    uint32_t i;

	//
    // Turn off the backlight
    //
    LCD_OFF();

    //
    // Reset the LCD
    //
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(20);

    //
    // Enter sleep mode.
    //
    LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG);
    LCD_WriteData(0x0001);

    //
    // Set initial power parameters.
    //
    LCD_WriteReg(SSD2119_PWR_CTRL_5_REG);
    LCD_WriteData(0x00BA);
    LCD_WriteReg(SSD2119_VCOM_OTP_1_REG);
    LCD_WriteData(0x0006);

    //
    // Start the oscillator.
    //
    LCD_WriteReg(SSD2119_OSC_START_REG);
    LCD_WriteData(0x0001);

    //
    // Set pixel format and basic display orientation (scanning direction).
    //
    LCD_WriteReg(SSD2119_OUTPUT_CTRL_REG);
//    LCD_WriteData(0x72EF);
    LCD_WriteData(0x30EF);
    LCD_WriteReg(SSD2119_LCD_DRIVE_AC_CTRL_REG);
    LCD_WriteData(0x0600);

    //
    // Exit sleep mode.
    //
    LCD_WriteReg(SSD2119_SLEEP_MODE_1_REG);
    LCD_WriteData(0x0000);

    //
    // Delay 30mS
    //
    HAL_Delay(30);

    //
    // Configure pixel color format and MCU interface parameters.
    //
    LCD_WriteReg(SSD2119_ENTRY_MODE_REG);
    LCD_WriteData(ENTRY_MODE_DEFAULT);

    //
    // Enable the display.
    //
    LCD_WriteReg(SSD2119_DISPLAY_CTRL_REG);
    LCD_WriteData(0x0033);

    //
    // Set VCIX2 voltage to 6.1V.
    //
    LCD_WriteReg(SSD2119_PWR_CTRL_2_REG);
    LCD_WriteData(0x0005);

    //
    // Configure gamma correction.
    //
    LCD_WriteReg(SSD2119_GAMMA_CTRL_1_REG);
    LCD_WriteData(0x0000);
    LCD_WriteReg(SSD2119_GAMMA_CTRL_2_REG);
    LCD_WriteData(0x0400);
    LCD_WriteReg(SSD2119_GAMMA_CTRL_3_REG);
    LCD_WriteData(0x0106);
    LCD_WriteReg(SSD2119_GAMMA_CTRL_4_REG);
    LCD_WriteData(0x0700);
    LCD_WriteReg(SSD2119_GAMMA_CTRL_5_REG);
    LCD_WriteData(0x0002);
    LCD_WriteReg(SSD2119_GAMMA_CTRL_6_REG);
    LCD_WriteData(0x0702);
    LCD_WriteReg(SSD2119_GAMMA_CTRL_7_REG);
    LCD_WriteData(0x0707);
    LCD_WriteReg(SSD2119_GAMMA_CTRL_8_REG);
    LCD_WriteData(0x0203);
    LCD_WriteReg(SSD2119_GAMMA_CTRL_9_REG);
    LCD_WriteData(0x1400);
    LCD_WriteReg(SSD2119_GAMMA_CTRL_10_REG);
    LCD_WriteData(0x0F03);

    //
    // Configure Vlcd63 and VCOMl.
    //
    LCD_WriteReg(SSD2119_PWR_CTRL_3_REG);
    LCD_WriteData(0x0007);
    LCD_WriteReg(SSD2119_PWR_CTRL_4_REG);
    LCD_WriteData(0x3100);

    //
    // Set the display size and ensure that the GRAM window is set to allow
    // access to the full display buffer.
    //
    LCD_WriteReg(SSD2119_V_RAM_POS_REG);
    LCD_WriteData((LCD_V_MAX - 1) << 8);
    LCD_WriteReg(SSD2119_H_RAM_START_REG);
    LCD_WriteData(0x0000);
    LCD_WriteReg(SSD2119_H_RAM_END_REG);
    LCD_WriteData(LCD_H_MAX - 1);
    LCD_WriteReg(SSD2119_X_RAM_ADDR_REG);
    LCD_WriteData(0x0000);
    LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG);
    LCD_WriteData(0x0000);

    //
    // Clear the contents of the display buffer.
    //
    LCD_WriteReg(SSD2119_RAM_DATA_REG);
    for(i = 0; i < (LCD_H_MAX * LCD_V_MAX); i++)
    {
        LCD_WriteData(0x0000);
    }

    //
    // Switch on the LED backlight
    //
    LCD_ON();
}
/*
 * Accesses a register
 */
void LCD_WriteReg(U16 reg)
{
	uint8_t LowByte, HighByte;

	LowByte = reg;
	HighByte = (reg >> 8);

	/* Activates register access (SDC = 0) */
	HAL_GPIO_WritePin(LCD_SDC_GPIO_Port, LCD_SDC_Pin, GPIO_PIN_RESET);
	/* Activates chip select (SCS = 0) */
	HAL_GPIO_WritePin(LCD_SCS_GPIO_Port, LCD_SCS_Pin, GPIO_PIN_RESET);
	/* Transmit data */
	HAL_SPI_Transmit(&hspi3, &HighByte, 1, 1000);
	HAL_SPI_Transmit(&hspi3, &LowByte, 1, 1000);
	/* Deactivates chip select (SCS = 1) */
	HAL_GPIO_WritePin(LCD_SCS_GPIO_Port, LCD_SCS_Pin, GPIO_PIN_SET);
}
/*
 * Writes data
 */
void LCD_WriteData(U16 data)
{
	uint8_t LowByte, HighByte;

	LowByte = data;
	HighByte = (data >> 8);

	/* Activates data access (SDC = 1) */
	HAL_GPIO_WritePin(LCD_SDC_GPIO_Port, LCD_SDC_Pin, GPIO_PIN_SET);
	/* Activates chip select (SCS = 0) */
	HAL_GPIO_WritePin(LCD_SCS_GPIO_Port, LCD_SCS_Pin, GPIO_PIN_RESET);
	/* Transmit data */
	HAL_SPI_Transmit(&hspi3, &HighByte, 1, 1000);
	HAL_SPI_Transmit(&hspi3, &LowByte, 1, 1000);
	/* Deactivates chip select (SCS = 1) */
	HAL_GPIO_WritePin(LCD_SCS_GPIO_Port, LCD_SCS_Pin, GPIO_PIN_SET);
}
/*
 * Writes multiple data
 */
void LCD_WriteDataMultiple(U16 *data, int numItems)
{
	uint8_t LowByte, HighByte;

	/* Activates data access (SDC = 1) */
	HAL_GPIO_WritePin(LCD_SDC_GPIO_Port, LCD_SDC_Pin, GPIO_PIN_SET);
	/* Activates chip select (SCS = 0) */
	HAL_GPIO_WritePin(LCD_SCS_GPIO_Port, LCD_SCS_Pin, GPIO_PIN_RESET);
	/* Transmit multiple data */
	while (numItems--)
	{
		LowByte = *data;
		HighByte = (*data) >> 8;
		HAL_SPI_Transmit(&hspi3, &HighByte, 1, 1000);
		HAL_SPI_Transmit(&hspi3, &LowByte, 1, 1000);
		data++;
	}
	/* Deactivates chip select (SCS = 1) */
	HAL_GPIO_WritePin(LCD_SCS_GPIO_Port, LCD_SCS_Pin, GPIO_PIN_SET);
}
/*
 * Reads multiple data (unavailable in this display)
 */
void LCD_ReadDataMultiple(U16 *data, int numItems)
{
	/* Not implemented */
}
/*
 * Turns on the backlight
 */
void LCD_ON()
{
	HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_SET);
}
/*
 * Turns off the backlight
 */
void LCD_OFF()
{
	HAL_GPIO_WritePin(LCD_LED_GPIO_Port, LCD_LED_Pin, GPIO_PIN_RESET);
}
