/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include <Global.h>
#include <time.h>
#include "stm32f4xx_nucleo_144.h"
#include "Kentec_SSD2119.h"
#include "GUI.h"
#include "GRAPH.h"
#include "PROGBAR.h"
#include "SLIDER.h"
#include "DIALOG.h"
#include "TIMER_Wrapper.h"
#include "UART_Wrapper.h"
#include "SYS.h"

#ifdef DEBUG_TOUCH
/* Recommended memory to run the sample with adequate performance */
#define RECOMMENDED_MEMORY (1024L * 10)
GUI_PID_STATE TouchState;
int	xPhys, xSize;
int	yPhys, ySize;
#define NUM_CALIB_POINTS  5  // Number of points for reference
#endif

#ifdef DEBUG_CAL
#define NUM_CALIB_POINTS  5  // Number of points for calibration
#endif

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern osSemaphoreId myBinarySem01Handle;
extern osThreadId defaultTaskHandle;
extern osThreadId myTask02Handle;
extern osThreadId myTask03Handle;

#ifdef DEBUG_WIN
extern WM_HWIN CreateProjeto_1();
extern WM_HWIN hTimeStr;
extern WM_HWIN hAdcStr;
extern WM_HWIN hTempStr;
extern WM_HWIN hGraph;
GRAPH_DATA_Handle hData;
#endif

/* Private variables ---------------------------------------------------------*/
static volatile unsigned int _Continue;

#ifdef DEBUG_TOUCH
static int _aRefX[NUM_CALIB_POINTS];
static int _aRefY[NUM_CALIB_POINTS];
static int _aSamX[NUM_CALIB_POINTS];
static int _aSamY[NUM_CALIB_POINTS];
#endif

#ifdef DEBUG_CAL
static int _aRefX[NUM_CALIB_POINTS];
static int _aRefY[NUM_CALIB_POINTS];
static int _aSamX[NUM_CALIB_POINTS];
static int _aSamY[NUM_CALIB_POINTS];
#endif

PROGBAR_Handle hProgBar;
SLIDER_Handle hSlider;
uint32_t value = 0;
uint32_t beacon = 0;

/* Definition of Run Time Statistics (RTS) timer */
TIM_HandleTypeDef timerRTS;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
#ifdef DEBUG_CAL
static void _Calibrate(void);
#endif
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
  uint32_t clock = 0;
//  RCC_OscInitTypeDef Osc;
//  RCC_ClkInitTypeDef Clk;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* USER CODE BEGIN Init */
  SYS_Init();
  SYS_ClockSet(SYS_CLOCK_168_MHZ_EXT);
  /* USER CODE END Init */

  /* USER CODE BEGIN SysInit */
  clock = HAL_RCC_GetSysClockFreq();
  clock = HAL_RCC_GetHCLKFreq();
  clock = HAL_RCC_GetPCLK1Freq();
  clock = HAL_RCC_GetPCLK2Freq();
  clock = clock;
//  HAL_RCC_GetOscConfig(&Osc);
//  HAL_RCC_GetClockConfig(&Clk, FLASH_LATENCY_5);

  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM7_Init();
  MX_CRC_Init();
  MX_SPI3_Init();
  MX_ADC3_Init();
  MX_TIM6_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();

  /* USER CODE BEGIN 2 */
#ifdef DEBUG_LCD
  uint32_t i;

  /* LCD Test */
  LCD_Init();

  LCD_WriteReg(SSD2119_X_RAM_ADDR_REG);
  LCD_WriteData(0);
  LCD_WriteReg(SSD2119_Y_RAM_ADDR_REG);
  LCD_WriteData(0);

  uint16_t pixels[] = {0x00F8, 0x00F8, 0x00F8,
		               0x00F8, 0x00F8, 0x00F8,
					   0x00F8, 0x00F8, 0x00F8,
		  	  	  	   0xF800, 0xF800, 0xF800,
					   0xF800, 0xF800, 0xF800,
					   0xF800, 0xF800, 0xF800,
					   0x0FE0, 0x0FE0, 0x0FE0,
  	  	  	  	  	   0x0FE0, 0x0FE0, 0x0FE0,
  	  	  	  	  	   0x0FE0, 0x0FE0, 0x0FE0};

  LCD_WriteReg(SSD2119_RAM_DATA_REG);
  for (i = 0; i < (76800/27) ; i++)
  {
	  LCD_WriteDataMultiple(pixels, 27);
  }

//  LCD_WriteReg(SSD2119_RAM_DATA_REG);
//  for(i = 0; i < 320*240 ; i++)
//  {
//      LCD_WriteData(0x00F8);
//  }
#endif

//#ifdef DEBUG_WIN
//  GUI_Init();
//  GUI_SetBkColor(GUI_WHITE);
//  GUI_SetColor(GUI_RED);
//  CreateProjeto_1();
////  CreateTest1();
////  CreateBigDialog();
////  CreateTest();
////  hAdjust = CreateAdjust();
////  hClock = CreateClock();
////  hAnalog = CreateAnalog();
//#endif

#ifdef DEBUG_GUI
  /* Init STemWin Graphic Library */
  GUI_Init();

//  GUI_SetTextMode(GUI_TEXTMODE_TRANS);
//  GUI_SetBkColor(GUI_BLACK);
//  GUI_SetColor(GUI_YELLOW);

//  GUI_SetFont(GUI_FONT_6X8);
//  GUI_DispStringAt("FONT_6X8..ABC0123", 10, 10);
//  GUI_SetFont(GUI_FONT_10_ASCII);
//  GUI_DispStringAt("FONT_10_ASCII..ABC0123", 10, 30);
//  GUI_SetFont(GUI_FONT_10S_ASCII);
//  GUI_DispStringAt("FONT_10S_ASCII..ABC0123", 10, 50);
//  GUI_SetFont(GUI_FONT_10_1);
//  GUI_DispStringAt("FONT_10_1..ABC0123", 10, 65);
//  GUI_SetFont(GUI_FONT_20F_ASCII);
//  GUI_DispStringAt("FONT_20F_ASCII..ABC0123", 10, 85);
//  GUI_SetFont(GUI_FONT_D32);
//  GUI_DispStringAt("32..+-", 10, 110);
//  GUI_SetFont(GUI_FONT_24_ASCII);
//  GUI_DispStringAt("GUI_FONT_24_ASCII", 10, 140);
//  GUI_SetFont(GUI_FONT_COMIC18B_ASCII);
//  GUI_DispStringAt("FONT_COMIC18B_ASCII..<>{}!@#$%&*", 10, 160);
//  GUI_SetFont(GUI_FONT_COMIC24B_ASCII);
//  GUI_DispStringAt("FONT_COMIC24B_ASCIIB..<>{}!@#$%&*", 10, 180);
//  GUI_SetFont(GUI_FONT_8X16X3X3);
//  GUI_DispStringAt("FONT_8X16X3X3..<>{}!@#$%&*", 10, 200);

//  GUI_FillRect(160, 10, 310, 60);
//  GUI_DrawLine(160, 68, 310, 78);

//  GUI_AA_SetDrawMode(GUI_AA_TRANS);
//  GUI_AA_EnableHiRes();
//  GUI_AA_SetFactor(5);
//  GUI_AA_DrawLine(160, 65, 310, 68);

  hProgBar = PROGBAR_Create(50, 190, 200, 25, WM_CF_SHOW);
  PROGBAR_SetBarColor(hProgBar, 0, GUI_GREEN);
  PROGBAR_SetBarColor(hProgBar, 1, GUI_RED);
//  PROGBAR_SetSkinClassic(hProgBar);

  hSlider = SLIDER_CreateEx(50, 20, 200, 25, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_SLIDER0);
  SLIDER_SetBkColor(hSlider, GUI_RED);
//  SLIDER_SetSkinClassic(hSlider);

  GUI_SetBkColor(GUI_BLUE);
  GUI_SetColor(GUI_YELLOW);
  GUI_Clear();
  GUI_SetFont(GUI_FONT_8X16X2X2);

//  GUI_SetFont(GUI_FONT_32B_ASCII);
//  GUI_SetBkColor(GUI_BLUE);
//  GUI_Clear();
//  GUI_DrawLine(0, 0, 319, 239);

//  GUI_SetFont(&GUI_Font8x16);
//  GUI_SetBkColor(GUI_BLUE);
//  GUI_Clear();
//  GUI_SetPenSize(10);
//  GUI_SetColor(GUI_RED);
//  GUI_DrawLine(80, 10, 240, 90);
//  GUI_DrawLine(80, 90, 240, 10);
//  GUI_SetBkColor(GUI_BLACK);
//  GUI_SetColor(GUI_WHITE);
//  GUI_SetTextMode(GUI_TM_NORMAL);
//  GUI_DispStringHCenterAt("GUI_TM_NORMAL" , 160, 10);
//  GUI_SetTextMode(GUI_TM_REV);
//  GUI_DispStringHCenterAt("GUI_TM_REV" , 160, 26);
//  GUI_SetTextMode(GUI_TM_TRANS);
//  GUI_DispStringHCenterAt("GUI_TM_TRANS" , 160, 42);
//  GUI_SetTextMode(GUI_TM_XOR);
//  GUI_DispStringHCenterAt("GUI_TM_XOR" , 160, 58);
//  GUI_SetTextMode(GUI_TM_TRANS | GUI_TM_REV);
//  GUI_DispStringHCenterAt("GUI_TM_TRANS | GUI_TM_REV", 160, 74);
#endif

#ifdef DEBUG_ALIAS
  GUI_Init();

  GUI_SetFont(GUI_FONT_COMIC18B_ASCII);
  GUI_SetBkColor(GUI_BLACK);
  GUI_SetColor(GUI_WHITE);
  GUI_Clear();
  GUI_AA_SetFactor(4);
  GUI_DispStringHCenterAt("Anti-Aliasing Text", 160, 20);
  GUI_AA_DrawLine(160, 65, 310, 68);
  GUI_DrawArc(160, 200, 70, 0, 0, 180);
  GUI_AA_DrawArc(160, 200, 80, 0, 0, 180);
  GUI_AA_DrawArc(160, 200, 90, 0, 30, 90);
  GUI_AA_SetFactor(2);
  GUI_AA_DrawLine(165, 70, 315, 73);
  GUI_AA_DrawArc(160, 200, 100, 0, 30, 90);
  GUI_AA_DrawArc(160, 200, 110, 0, 30, 90);
#endif

#ifdef DEBUG_GRAPH
  PROGBAR_SKINFLEX_PROPS props;
  GUI_Init();
  GUI_DispStringAt("Progress bar", 100, 20+150);
  hProgBar = PROGBAR_Create(100, 40+150, 150, 30, WM_CF_SHOW);
  PROGBAR_SetBarColor(hProgBar, 0, GUI_GREEN);
  PROGBAR_SetBarColor(hProgBar, 1, GUI_RED);
  PROGBAR_SetValue(hProgBar, 75);
  PROGBAR_GetSkinFlexProps(&props, 0);
  props.ColorText = GUI_WHITE;
  props.ColorFrame = GUI_BLACK;
  props.aColorUpperL[0] = GUI_WHITE;
  props.aColorUpperL[1] = GUI_RED;
  props.aColorLowerL[0] = GUI_RED;
  props.aColorLowerL[1] = GUI_BLACK;
  PROGBAR_SetSkinFlexProps(&props, 0);

  GRAPH_DATA_Handle hData;
  GRAPH_SCALE_Handle hScale;
  WM_HWIN hGraph;
  I16 aData0[] = {2,3,4,5,3,4,5,2,3,4,3,4,14,12,9,8,6,2,3,4,2,1};

  hGraph = GRAPH_CreateEx(10, 10, 216, 106, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_GRAPH0);
  hData = GRAPH_DATA_YT_Create(GUI_DARKGREEN, 16, aData0, 16);
  GRAPH_AttachData(hGraph, hData);
  hScale = GRAPH_SCALE_Create(28, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 20);
  GRAPH_AttachScale(hGraph, hScale);

  GUI_Exec();

  /*
  Do something with the widget...
  */
//  WM_DeleteWindow(hGraph);
#endif

#ifdef DEBUG_TOUCH
//  GUI_PID_STATE TouchState;
//  int	xPhys, xSize;
//  int	yPhys, ySize;
//  int	i;
//
//  GUI_Init();
//  //
//  // Check if recommended memory for the sample is available
//  //
//  if (GUI_ALLOC_GetNumFreeBytes() < RECOMMENDED_MEMORY)
//  {
//    GUI_ErrorOut("Not enough memory available.");
//    return -1;
//  }
//  GUI_CURSOR_Show();
//  GUI_CURSOR_Select(&GUI_CursorCrossL);
//  GUI_SetBkColor(GUI_WHITE);
//  GUI_SetColor(GUI_BLUE);
//  GUI_SetPenSize(1);
//  GUI_Clear();
//
//  xSize = LCD_GetXSize();
//  ySize = LCD_GetYSize();
//
//  //
//  // Calculate reference points depending on LCD size
//  //
//  _aRefX[0] = (xSize * 5) / 100;
//  _aRefY[0] = (ySize * 5) / 100;
//  _aRefX[1] = xSize - (xSize * 5) / 100;
//  _aRefY[1] = _aRefY[0];
//  _aRefX[2] = _aRefX[1];
//  _aRefY[2] = ySize - (ySize * 5) / 100;
//  _aRefX[3] = _aRefX[0];
//  _aRefY[3] = _aRefY[2];
//  _aRefX[4] = xSize / 2;
//  _aRefY[4] = ySize / 2;
//  //
//  // Draw reference points on LCD
//  //
//  for (i = 0; i < NUM_CALIB_POINTS; i++)
//  {
//    GUI_DrawCircle(_aRefX[i], _aRefY[i], 5);
//  }
#endif

#ifdef DEBUG_CAL
  GUI_Init();
  //
  // Calibrate touch ...
  //
  _Calibrate();
  //
  // ... and play with it
  //
  GUI_CURSOR_Show();
  BUTTON_Create(100, 100, 200, 50, 1234, WM_CF_SHOW);  // A button to be touched
#endif

  GUI_Init();

  /*
   * IMPORTANTE!
   * ----------
   * Dispare os Timers que geram interrupções somente após a inicialização do RTOS.
   * Como isso implica em escrever fora da área de código do usuário, lembre-se de
   * reescrever este código após cada atualização do projeto via CubeMX:
   *
   *  HAL_TIM_Base_Start_IT(&htim6);
   *  HAL_TIM_Base_Start_IT(&htim7);
   */

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();

  /* Runs Timer 6 to trigger interrupts */
//  HAL_TIM_Base_Start_IT(&htim6);

  /* Runs Timer 7 to trigger interrupts */
  HAL_TIM_Base_Start_IT(&htim7);

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
#ifdef DEBUG_TOUCH
	 GUI_TOUCH_GetState(&TouchState);  // Get the touch position in pixel
	 xPhys = GUI_TOUCH_GetxPhys();     // Get the A/D mesurement result in x
	 yPhys = GUI_TOUCH_GetyPhys();     // Get the A/D mesurement result in y
	 //
	 // Display the measurement result
	 //
	 GUI_SetColor(GUI_BLUE);
	 GUI_DispStringAt("Analog input:\n", 0, 40);
	 GUI_GotoY(GUI_GetDispPosY() + 2);
	 GUI_DispString("x:");
	 GUI_DispDec(xPhys, 4);
	 GUI_DispString(", y:");
	 GUI_DispDec(yPhys, 4);
	 //
	 // Display the according position
	 //
	 GUI_SetColor(GUI_RED);
	 GUI_GotoY(GUI_GetDispPosY() + 4);
	 GUI_DispString("\nPosition:\n");
	 GUI_GotoY(GUI_GetDispPosY() + 2);
	 GUI_DispString("x:");
	 GUI_DispDec(TouchState.x,4);
	 GUI_DispString(", y:");
	 GUI_DispDec(TouchState.y,4);
	 /*
	  * Draw pixel path
	  */
	 GUI_SetColor(GUI_BLACK);
//	 GUI_DrawPixel(TouchState.x, TouchState.y);
//	 GUI_DrawPoint(TouchState.x, TouchState.y);
	 GUI_DrawLineTo(TouchState.x, TouchState.y);
//	 GUI_DrawLineRel(TouchState.x, TouchState.y);
	 //
	 // Wait a while
	 //
	 GUI_Delay(1);
//	 HAL_Delay(100);
//	 GUI_Exec();
#endif

#ifdef DEBUG_CAL
	 GUI_Delay(100);
//	 HAL_Delay(100);
	 GUI_Exec();
#endif

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/** NVIC Configuration
*/
static void MX_NVIC_Init(void)
{
  /* TIM6_DAC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
  /* TIM7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM7_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(TIM7_IRQn);
}

/* USER CODE BEGIN 4 */

#ifdef DEBUG_CAL
/*********************************************************************
*
*       _Calibrate
*
* Purpose:
*   Calibrates the touch screen. Please note that this sample assumes
*   that the 'Driver API for analog touch screens' is used.
*/
static void _Calibrate(void) {
  GUI_PID_STATE State;
  int i;
  int xSize, ySize;

  xSize = LCD_GetXSize();
  ySize = LCD_GetYSize();
  //
  // Calculate reference points depending on LCD size
  //
  _aRefX[0] = (xSize * 5) / 100;
  _aRefY[0] = (ySize * 5) / 100;
  _aRefX[1] = xSize - (xSize * 5) / 100;
  _aRefY[1] = _aRefY[0];
  _aRefX[2] = _aRefX[1];
  _aRefY[2] = ySize - (ySize * 5) / 100;
  _aRefX[3] = _aRefX[0];
  _aRefY[3] = _aRefY[2];
  _aRefX[4] = xSize / 2;
  _aRefY[4] = ySize / 2;
  //
  // Draw reference points on LCD
  //
  GUI_TOUCH_GetState(&State);
  State.Pressed = 0;
  GUI_SetPenSize(3);
  for (i = 0; i < NUM_CALIB_POINTS; i++)
  {
    GUI_Clear();
    GUI_DispStringHCenterAt("Please touch the point", LCD_GetXSize() / 2, LCD_GetYSize() / 2 - 60);
    GUI_DrawCircle(_aRefX[i], _aRefY[i], 5);
    while (State.Pressed != 1)
    {
      GUI_Delay(250);
      GUI_TOUCH_GetState(&State);
    }
    if (State.Pressed == 1) {
      //
      // Store sample points
      //
      _aSamX[i] = GUI_TOUCH_GetxPhys();
      _aSamY[i] = GUI_TOUCH_GetyPhys();
    }
    State.Pressed = 0;
    GUI_Delay(250);
  }
  //
  // Pass measured points to emWin
  //
  GUI_TOUCH_CalcCoefficients(NUM_CALIB_POINTS, _aRefX, _aRefY, _aSamX, _aSamY, xSize, ySize);
}
#endif
/*
 * Teste de interrupcao
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	osStatus status;
	int32_t signal;

	/*
	 * This function is called  when TIM14 interrupt took place, inside
	 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
	 * a global variable "uwTick" used as application time base.
	 */
	if (htim->Instance == TIM14)
	{
		HAL_IncTick();
	}
	  /*
	 * Touch Screen ISR code (executes at 100Hz)
	 */
	if (htim == &htim6)
	{
//#ifdef DEBUG_WIN
//		GUI_TOUCH_Exec();
//#endif
//		signal = osSignalSet(myTask03Handle, 5);
	}
	/*
	 * Application ISR code (executes at 10Hz)
	 */
	if (htim == &htim7)
	{
		/*
		 * Libera semáforo em DefaultTask.
		 */
//		status = osSemaphoreRelease(myBinarySem01Handle);
		signal = osSignalSet(defaultTaskHandle, 5);
//		BSP_LED_Toggle(LED1);

//		beacon ^= 1;
//
//		BSP_LED_Toggle(LED2);
//		/* ADC conversion started with DMA transfer */
//		HAL_ADC_Start_DMA(&hadc1, &value, 1);

//		HAL_ADC_Start(&hadc1);
//		/* Waits for ADC conversion or 50 ms timeout */
//		if (HAL_ADC_PollForConversion(&hadc1, 50) == HAL_OK)
//		{
//			value = HAL_ADC_GetValue(&hadc1);
//		}

//#ifdef DEBUG_WIN
//		char str[15];
//
////		GUI_DispDecAt(value, 15, 10, 4);
////		GUI_DispStringAt("0000", 60, 10);
//
//		sprintf(str, "%04d", (int)value);
//		TEXT_SetText(hAdcStr, str);
//		TEXT_SetText(hTempStr, "25ºC");
//
////		struct tm* st = localtime((time_t*)&value);
////		sprintf(str, "%02d:%02d:%02d.%1d ", st->tm_hour,
////										    st->tm_min,
////										    st->tm_sec,
////										    (int)value);
//		TEXT_SetText(hTimeStr, "00:00:00.0");
//
////	    TEXT_SetText(hTimeStr, str);
////		PROGBAR_SetValue(hProgbar, value);
////	    SLIDER_SetValue(hSlider, value);
//
//		GUI_Exec();
//#endif

#ifdef DEBUG_LCD
		BSP_LED_Toggle(LED1);
#endif

#ifdef DEBUG_GUI
		GUI_DispDecAt(value, 100, 100, 4);
		GUI_DispString("°C");
		int bar = (int32_t)((value*10)/409);
		PROGBAR_SetValue(hProgBar, bar);
		SLIDER_SetValue(hSlider, bar);
//		WM_Exec();
		GUI_Exec();
		BSP_LED_Toggle(LED3);
#endif

#ifdef DEBUG_GRAPH
		int bar = (int32_t)((value*10)/409);
		PROGBAR_SetValue(hProgBar, bar);
		GUI_Exec();
		BSP_LED_Toggle(LED3);
#endif
	//  beacon ^= 1;
	//  BSP_LED_Toggle(LED1);
	}
}
/**
* @brief This function handles Hard fault interrupt.
*/
//void HardFault_Handler(void) {
//_Continue = 0u;
////
//// When stuck here, change the variable value to != 0 in order to step out
////
//while (_Continue == 0u);
//}

/**
 *  This function configures a timer that is used as the time base when
 *  collecting run time statistical information - basically the percentage
 *  of CPU time that each task is utilizing.  It is called automatically when
 *  the scheduler is started (assuming configGENERATE_RUN_TIME_STATS is set
 *  to 1).
 */
void configureTimerForRunTimeStats()
{
//	/* Configures 32-bit Timer 5 as a 20kHz time base */
//	timerRTS.Instance = TIM5;
//	timerRTS.Init.Prescaler = HAL_RCC_GetPCLK1Freq() / 20000 + 1;
//	timerRTS.Init.CounterMode = TIM_COUNTERMODE_UP;
//	timerRTS.Init.Period = 0;
//	if (HAL_TIM_Base_Init(&timerRTS) != HAL_OK)
//	{
//		_Error_Handler(__FILE__, __LINE__);
//	}
//
//	/* Enables clock to timer */
//	__HAL_RCC_TIM5_CLK_ENABLE();
//
//    /* Run timer */
//    HAL_TIM_Base_Start(&timerRTS);

	/* Configures 32-bit Timer 5 as a 20kHz time base */
	TIMER_Init(TIMER_5);
	TIMER_Config(TIMER_5, TIMER_PERIODIC_UP);
	TIMER_ConfigPrescaler(TIMER_5, 20000);
	/* Run timer */
	TIMER_Enable(TIMER_5);
}
/**
 * Simply returns the current timer counter value
 * used for the run-time statistics time base.
 */
uint32_t getRunTimeCounterValue()
{
	/* Returns the counter register */
//	return timerRTS.Instance->CNT;
	return TIMER_GetCount(TIMER_5);
}

/* USER CODE END 4 */


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
