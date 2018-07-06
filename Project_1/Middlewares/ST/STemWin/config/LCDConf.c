/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf_FlexColor_Template.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

#include <GUI.h>
#include <GUIDRV_FlexColor.h>
#include "Kentec_SSD2119.h"
#include "touch.h"

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/
/*
 * LCD physical size
 */
#define XSIZE_PHYS  		LCD_H_MAX
#define YSIZE_PHYS  		LCD_V_MAX

/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   GUICC_565
  #error Color conversion not defined!
#endif
#ifndef   GUIDRV_FLEXCOLOR
  #error No display driver defined!
#endif

/*
 * Display orientation
 */
#define GUI_LANDSCAPE		GUI_ROTATION_180
#define GUI_PORTRAIT		GUI_ROTATION_CW
#define GUI_LANDSCAPE_INV	GUI_ROTATION_0	/* LCD default orientation */
#define GUI_PORTRAIT_INV	GUI_ROTATION_CCW

#define GUI_ORIENTATION		GUI_LANDSCAPE
/*
 * Touch calibration
 */
#define	ADC95X	3370	/* ADC X input at 95% screen point */
#define	ADC05X	 690	/* ADC X input at 5% screen point */
#define	ADC95Y	3490	/* ADC Y input at 95% screen point */
#define	ADC05Y	 540	/* ADC Y input at 5% screen point */
#define	ADC50X	2000	/* ADC X input at 50% screen point */
#define	ADC50Y	1960	/* ADC Y input at 50% screen point */

#define NUM_CALIB_POINTS	5

//#if (GUI_ORIENTATION == GUI_LANDSCAPE)
//	static int aRefX[] = {16, 304, 304, 16, 160};
//	static int aRefY[] = {12, 12, 228, 228, 120};
//	static int aSamX[] = {ADC05X, ADC95X, ADC95X, ADC05X, ADC50X};
//	static int aSamY[] = {ADC95Y, ADC95Y, ADC05Y, ADC05Y, ADC50Y};
//	static int xSize = XSIZE_PHYS;
//	static int ySize = YSIZE_PHYS;
//#elif (GUI_ORIENTATION == GUI_PORTRAIT)
//	static int aRefX[] = {12, 228, 228, 12, 120};
//	static int aRefY[] = {16, 16, 304, 304, 160};
//	static int aSamX[] = {ADC05Y, ADC95Y, ADC95Y, ADC05Y, ADC50Y};
//	static int aSamY[] = {ADC05X, ADC05X, ADC95X, ADC95X, ADC50X};
//	static int xSize = YSIZE_PHYS;
//	static int ySize = XSIZE_PHYS;
//#elif (GUI_ORIENTATION == GUI_LANDSCAPE_INV)
//	static int aRefX[] = {16, 304, 304, 16, 160};
//	static int aRefY[] = {12, 12, 228, 228, 120};
//	static int aSamX[] = {ADC95X, ADC05X, ADC05X, ADC95X, ADC50X};
//	static int aSamY[] = {ADC05Y, ADC05Y, ADC95Y, ADC95Y, ADC50Y};
//	static int xSize = XSIZE_PHYS;
//	static int ySize = YSIZE_PHYS;
//#elif (GUI_ORIENTATION == GUI_PORTRAIT_INV)
//	static int aRefX[] = {12, 228, 228, 12, 120};
//	static int aRefY[] = {16, 16, 304, 304, 160};
//	static int aSamX[] = {ADC95Y, ADC05Y, ADC05Y, ADC95Y, ADC50Y};
//	static int aSamY[] = {ADC95X, ADC95X, ADC05X, ADC05X, ADC50X};
//	static int xSize = YSIZE_PHYS;
//	static int ySize = XSIZE_PHYS;
//#endif

#if (GUI_ORIENTATION == GUI_LANDSCAPE)
	static int aRefX[] = {16, 304, 304, 16, 160};
	static int aRefY[] = {12, 12, 228, 228, 120};
	static int aSamX[] = {677, 3398, 3210, 680, 1928};
	static int aSamY[] = {3472, 3380, 534, 542, 1986};
	static int xSize = XSIZE_PHYS;
	static int ySize = YSIZE_PHYS;
#elif (GUI_ORIENTATION == GUI_PORTRAIT)
	static int aRefX[] = {12, 228, 228, 12, 120};
	static int aRefY[] = {16, 16, 304, 304, 160};
	static int aSamX[] = {522, 3489, 3403, 514, 2033};
	static int aSamY[] = {705, 671, 3396, 3174, 1943};
	static int xSize = YSIZE_PHYS;
	static int ySize = XSIZE_PHYS;
#elif (GUI_ORIENTATION == GUI_LANDSCAPE_INV)
	static int aRefX[] = {16, 304, 304, 16, 160};
	static int aRefY[] = {12, 12, 228, 228, 120};
	static int aSamX[] = {3203, 704, 647, 3375, 1947};
	static int aSamY[] = {550, 563, 3456, 3413, 1998};
	static int xSize = XSIZE_PHYS;
	static int ySize = YSIZE_PHYS;
#elif (GUI_ORIENTATION == GUI_PORTRAIT_INV)
	static int aRefX[] = {12, 228, 228, 12, 120};
	static int aRefY[] = {16, 16, 304, 304, 160};
	static int aSamX[] = {3397, 471, 518, 3462, 1991};
	static int aSamY[] = {3394, 3189, 688, 645, 1936};
	static int xSize = YSIZE_PHYS;
	static int ySize = XSIZE_PHYS;
#endif

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_Config
*
* Function description:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*
*/
void LCD_X_Config(void)
{
  GUI_DEVICE * pDevice;
  CONFIG_FLEXCOLOR Config = {0};
  GUI_PORT_API PortAPI = {0};

  /*
   * Set display driver and color conversion
   */
  pDevice = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_565, 0, 0);

  /*
   * Display driver configuration
   */
  LCD_SetSize (XSIZE_PHYS, YSIZE_PHYS);
  LCD_SetVSize(VXSIZE_PHYS, VYSIZE_PHYS);

  /*
   * Controller configuration
   */
  Config.RegEntryMode = ENTRY_MODE_DEFAULT;
  Config.Orientation = GUI_ORIENTATION;
  GUIDRV_FlexColor_Config(pDevice, &Config);

  /*
   * Set controller and operation mode
   */
  PortAPI.pfWrite16_A0  = LCD_WriteReg;
  PortAPI.pfWrite16_A1  = LCD_WriteData;
  PortAPI.pfWriteM16_A1 = LCD_WriteDataMultiple;
  /*
   * The Read function is unavailable in this display, then
   * some functionality (like inverting pixels) will not work
   * properly. The next assignment is intended only to avoid
   * application crashes when using such features.
   */
  PortAPI.pfReadM16_A1  = LCD_ReadDataMultiple;

  GUIDRV_FlexColor_SetFunc(pDevice, &PortAPI,
		  	  	  	  	   GUIDRV_FLEXCOLOR_F66714,
						   GUIDRV_FLEXCOLOR_M16C0B16);

  /* Touch screen compile-time calibration (5 points) */
  GUI_TOUCH_CalcCoefficients(NUM_CALIB_POINTS,
		  	  	  	  	  	 aRefX, aRefY,
		  	  	  	  	     aSamX, aSamY,
							 xSize, ySize);

}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Function description:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData)
{
  int r;
  (void) LayerIndex;
  (void) pData;
  
  switch (Cmd)
  {
  	  case LCD_X_INITCONTROLLER:
  	  {
  		  LCD_Init();
  		  return 0;
  	  }
  	  default:
  		  r = -1;
  }
  return r;
}

/*************************** End of file ****************************/

