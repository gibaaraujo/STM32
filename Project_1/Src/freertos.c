/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */     
#include "stm32f4xx_nucleo_144.h"
#include "GUI.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"
#include "FRAMEWIN.h"
#include "DIALOG.h"
/* USER CODE END Includes */

/* Variables -----------------------------------------------------------------*/
osThreadId defaultTaskHandle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;
osMessageQId myQueue01Handle;

/* USER CODE BEGIN Variables */
extern uint32_t value;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

/* USER CODE END Variables */

/* Function prototypes -------------------------------------------------------*/
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);
void StartTask04(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* Hook prototypes */

/* Init FreeRTOS */

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

   /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of myTask02 */
  osThreadDef(myTask02, StartTask02, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask03, StartTask03, osPriorityBelowNormal, 0, 512);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* definition and creation of myTask03 */
  osThreadDef(myTask04, StartTask04, osPriorityLow, 0, configMINIMAL_STACK_SIZE);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMessageQDef(myQueue01, 16, uint16_t);
  myQueue01Handle = osMessageCreate(osMessageQ(myQueue01), 0);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
}

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{
	/* USER CODE BEGIN StartDefaultTask */
	osEvent event;

	/* Infinite loop */
	for(;;)
	{
		/* ADC conversion started with DMA transfer */
//		HAL_ADC_Start_DMA(&hadc1, &value, 1);
//		BSP_LED_Toggle(LED1);
//		osDelay(100);

		event = osSignalWait(0, osWaitForever);
		if (event.value.signals & 1)
		{
			/* ADC conversion started with DMA transfer */
			HAL_ADC_Start_DMA(&hadc1, &value, 1);
			BSP_LED_Toggle(LED1);
		}
	}
	/* USER CODE END StartDefaultTask */
}

/* StartTask02 function */
void StartTask02(void const * argument)
{
	/* USER CODE BEGIN StartTask02 */

//	  FRAMEWIN_Handle hFrameWin;
//	  WM_HWIN         hChildWin;
//
//	  //
//	  // Create frame window
//	  //
//	  hFrameWin = FRAMEWIN_Create("Task 1", NULL, WM_CF_SHOW | WM_CF_STAYONTOP,
//	                              20, 170, 200, 50);
//	  //
//	  // Create child window
//	  //
//	  hChildWin = WM_CreateWindowAsChild(0, 0, 0, 0, WM_GetClientWindow(hFrameWin),
//	                                     WM_CF_SHOW , 0, 0);
//	  FRAMEWIN_SetActive(hFrameWin, 0);
//
//	  for(;;)
//	  {
//		  int i;
//		  int nx = 8;
//		  int ny = 9;
//		  //
//		  // Move window continously
//		  //
//		  for (i = 0; i < ny; i++) {
//			  WM_MoveWindow(hFrameWin, 0, -2);
//		      HAL_Delay(1);
//		  }
//		  for (i = 0; i < nx; i++) {
//			  WM_MoveWindow(hFrameWin, 2, 0);
//		      HAL_Delay(1);
//		  }
//		  for (i = 0; i < ny; i++) {
//		      WM_MoveWindow(hFrameWin, 0, 2);
//		      HAL_Delay(1);
//		    }
//		  for (i = 0; i < nx; i++) {
//		      WM_MoveWindow(hFrameWin, -2, 0);
//		      HAL_Delay(1);
//		  }
//
//		  BSP_LED_Toggle(LED2);
//		  osDelay(50);
// 	  }
//
	uint32_t tick = osKernelSysTick();

	/* Infinite loop */
    for(;;)
    {
    	BSP_LED_Toggle(LED2);
		osDelayUntil(&tick, 50);
    }
/* USER CODE END StartTask02 */
}

/* StartTask03 function */
void StartTask03(void const * argument)
{
	/* USER CODE BEGIN StartTask03 */
	uint32_t counter = 0;

#ifdef DEBUG_WIN
	extern WM_HWIN CreateProjeto_1();
	extern WM_HWIN hTimeStr;
	extern WM_HWIN hAdcStr;
	extern WM_HWIN hTempStr;
	extern WM_HWIN hGraph;

	GUI_SetBkColor(GUI_WHITE);
	GUI_SetColor(GUI_RED);
	CreateProjeto_1();
#endif

#ifdef DEBUG_TOUCH
	#define NUM_CALIB_POINTS  5  // Number of points for reference
    static int _aRefX[NUM_CALIB_POINTS];
    static int _aRefY[NUM_CALIB_POINTS];
    GUI_PID_STATE TouchState;
    int	xPhys, xSize;
    int	yPhys, ySize;
    int	i;

    GUI_CURSOR_Show();
    GUI_CURSOR_Select(&GUI_CursorCrossL);
    GUI_SetBkColor(GUI_WHITE);
    GUI_SetColor(GUI_BLUE);
    GUI_SetPenSize(1);
    GUI_Clear();

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
    for (i = 0; i < NUM_CALIB_POINTS; i++)
    {
      GUI_DrawCircle(_aRefX[i], _aRefY[i], 5);
    }
#endif

	/* Infinite loop */
	for(;;)
	{
#ifdef DEBUG_WIN
		char str[15];

//		GUI_DispDecAt(value, 15, 15, 4);
//		GUI_DispStringAt("0000", 60, 10);

		sprintf(str, "%04d", (int)value);
		TEXT_SetText(hAdcStr, str);
		TEXT_SetText(hTempStr, "25ºC");

		TEXT_SetText(hTimeStr, "00:00:00.0");
#endif

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

		/* Draw pixel path */
		GUI_SetColor(GUI_BLACK);
		//GUI_DrawPixel(TouchState.x, TouchState.y);
		//GUI_DrawPoint(TouchState.x, TouchState.y);
		GUI_DrawLineTo(TouchState.x, TouchState.y);
		//GUI_DrawLineRel(TouchState.x, TouchState.y);
#endif

		/* Updates GUI */
		GUI_TOUCH_Exec();
		GUI_Delay(10);
	}
	/* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Application */

/* StartTask04 function */
void StartTask04(void const * argument)
{
    for(;;)
    {
		BSP_LED_Toggle(LED3);
		osDelay(30);
    }
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
