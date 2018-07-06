/*
 * Kentec_SSD2119.h
 *
 * 	SPI 4-Wire Interface for SSD2119 LCD Controller
 *
 *  Implements the device driver hardware interface
 *  for 320x240 TFT Display Board BOOSTXL-K350QVG-S1,
 *  based on STMicroeletronics STM32F429ZI Nucleo board
 *  and STemWin graphic library
 *
 *  Created on: 14 de out de 2017
 *  Author: Gilberto Araujo
 */

#ifndef KENTEC_SSD2119_H_
#define KENTEC_SSD2119_H_

/*
 * SSD2119 registers
 */
#define SSD2119_DEVICE_CODE_READ_REG  0x00
#define SSD2119_OSC_START_REG         0x00
#define SSD2119_OUTPUT_CTRL_REG       0x01
#define SSD2119_LCD_DRIVE_AC_CTRL_REG 0x02
#define SSD2119_PWR_CTRL_1_REG        0x03
#define SSD2119_DISPLAY_CTRL_REG      0x07
#define SSD2119_FRAME_CYCLE_CTRL_REG  0x0b
#define SSD2119_PWR_CTRL_2_REG        0x0c
#define SSD2119_PWR_CTRL_3_REG        0x0d
#define SSD2119_PWR_CTRL_4_REG        0x0e
#define SSD2119_GATE_SCAN_START_REG   0x0f
#define SSD2119_SLEEP_MODE_1_REG      0x10
#define SSD2119_ENTRY_MODE_REG        0x11
#define SSD2119_SLEEP_MODE_2_REG      0x12
#define SSD2119_GEN_IF_CTRL_REG       0x15
#define SSD2119_PWR_CTRL_5_REG        0x1e
#define SSD2119_RAM_DATA_REG          0x22
#define SSD2119_FRAME_FREQ_REG        0x25
#define SSD2119_ANALOG_SET_REG        0x26
#define SSD2119_VCOM_OTP_1_REG        0x28
#define SSD2119_VCOM_OTP_2_REG        0x29
#define SSD2119_GAMMA_CTRL_1_REG      0x30
#define SSD2119_GAMMA_CTRL_2_REG      0x31
#define SSD2119_GAMMA_CTRL_3_REG      0x32
#define SSD2119_GAMMA_CTRL_4_REG      0x33
#define SSD2119_GAMMA_CTRL_5_REG      0x34
#define SSD2119_GAMMA_CTRL_6_REG      0x35
#define SSD2119_GAMMA_CTRL_7_REG      0x36
#define SSD2119_GAMMA_CTRL_8_REG      0x37
#define SSD2119_GAMMA_CTRL_9_REG      0x3a
#define SSD2119_GAMMA_CTRL_10_REG     0x3b
#define SSD2119_V_RAM_POS_REG         0x44
#define SSD2119_H_RAM_START_REG       0x45
#define SSD2119_H_RAM_END_REG         0x46
#define SSD2119_X_RAM_ADDR_REG        0x4e
#define SSD2119_Y_RAM_ADDR_REG        0x4f

#define ENTRY_MODE_DEFAULT      	0x6830
#define MAKE_ENTRY_MODE(x)      	((ENTRY_MODE_DEFAULT & 0xff00) | (x))

/*
 * Panel dimensions
 */
#define LCD_V_MAX					240
#define LCD_H_MAX					320

/*
 * Prototypes
 */
extern void LCD_Init();
extern void LCD_WriteReg(U16 reg);
extern void LCD_WriteData(U16 data);
extern void LCD_WriteDataMultiple(U16 *data, int numItems);
extern void LCD_ReadDataMultiple(U16 *data, int numItems);
extern void LCD_ON();
extern void LCD_OFF();

#endif /* KENTEC_SSD2119_H_ */
