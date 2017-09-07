/* Includes ------------------------------------------------------------------*/
#include "gui.h"
#include "stm32746g_discovery_lcd.h"
#include "lcd_log.h"

char analog[4];
char pwm_char[3];
char mode;
uint8_t temp_din = 0;
uint8_t temp_dout = 0;
uint8_t temp_mode = 10;

void starting_screen()
{
	char num[2];
	gui_status.message_flag = 0;

	BSP_LCD_Clear(LCD_COLOR_LIGHTGREEN);

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTGREEN);


	BSP_LCD_DisplayStringAt(15, 13, "D_IN", LEFT_MODE);
	BSP_LCD_DisplayStringAt(70, 13, "D_OUT", LEFT_MODE);
	BSP_LCD_DisplayStringAt(130, 13, "A_IN", LEFT_MODE);
	BSP_LCD_DisplayStringAt(185, 13, "A_OUT", LEFT_MODE);
	BSP_LCD_DisplayStringAt(250, 13, "D_PWM", LEFT_MODE);

	BSP_LCD_SetFont(&Font16);
	BSP_LCD_DisplayStringAt(310, 13, "Mode:", LEFT_MODE);
	BSP_LCD_SetFont(&Font12);

	// Digital in out rectangles
	for (uint8_t i = 0; i < 9; i++) {
		BSP_LCD_DrawRect(1, (i * 30 + 1), 60, 30);
		BSP_LCD_DrawRect(61, (i * 30 + 1), 60, 30);
	}

	// Analog in rectangles
	for (uint8_t i = 0; i < 7; i++) {
		if (i < 6) {
			sprintf(num, "%d:", i+1);
			BSP_LCD_DisplayStringAt(130, i * 30 + 40, num, LEFT_MODE);
		}
		BSP_LCD_DrawRect(121, (i * 30 + 1), 60, 30);
	}

	// Analog out rectangles
	for (uint8_t i = 0; i < 3; i++) {
		if (i < 2) {
			sprintf(num, "%d:", i+1);
			BSP_LCD_DisplayStringAt(190, i * 30 + 40, num, LEFT_MODE);
		}
		BSP_LCD_DrawRect(181, (i * 30 + 1), 60, 30);
	}

	// PWM out rectangles
	for (uint8_t i = 0; i < 4; i++) {
		if (i < 3) {
			sprintf(num, "%d:", i+1);
			BSP_LCD_DisplayStringAt(250, i * 30 + 40, num, LEFT_MODE);
		}
		BSP_LCD_DrawRect(241, (i * 30 + 1), 60, 30);
	}

	for (uint8_t i = 0; i < 8; i++) {

		sprintf(num, "%d:", i+1);

		BSP_LCD_DisplayStringAt(16, i * 30 + 40, num, LEFT_MODE);
		BSP_LCD_FillCircle(46, i * 30 + 46, 10);

		BSP_LCD_DisplayStringAt(76, i * 30 + 40, num, LEFT_MODE);
		BSP_LCD_FillCircle(106, i * 30 + 46, 10);
	}
}


void gui_display_status()
{

	// Display Digital in
	if (temp_din != gui_status.din_state) {

		temp_din = gui_status.din_state;

		for (uint8_t i = 0; i < 8; i++) {
			if (temp_din & (1 << i)) {
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				BSP_LCD_FillCircle(46, i * 30 + 46, 10);
			} else {
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_FillCircle(46, i * 30 + 46, 10);
			}
		}

		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	}

	// Display Digital Out
	if (temp_dout != gui_status.dout_state) {

		temp_dout = gui_status.dout_state;

		for (uint8_t i = 0; i < 8; i++) {
			if (temp_dout & (1 << i)) {
				BSP_LCD_SetTextColor(LCD_COLOR_RED);
				BSP_LCD_FillCircle(106, i * 30 + 46, 10);
			} else {
				BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
				BSP_LCD_FillCircle(106, i * 30 + 46, 10);
			}
		}

		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	}

	// Display Analog in
	for (uint8_t i = 0; i < 6; i++) {
		if (gui_status.ain_state[i] < 10) {
			sprintf(analog, "%d   ", gui_status.ain_state[i]);
		} else if (gui_status.ain_state[i] > 10 && gui_status.ain_state[i] < 100) {
			sprintf(analog, "%d  ", gui_status.ain_state[i]);
		} else if (gui_status.ain_state[i] > 100 && gui_status.ain_state[i] < 1000) {
			sprintf(analog, "%d ", gui_status.ain_state[i]);
		} else {
			sprintf(analog, "%d", gui_status.ain_state[i]);
		}

		BSP_LCD_DisplayStringAt(150, i * 30 + 40, analog, LEFT_MODE);
	}

	// Display Analog out
	for (uint8_t i = 0; i < 2; i++) {

		if (gui_status.aou_state[i] < 10) {
			sprintf(analog, "%d   ", gui_status.aou_state[i]);
		} else if (gui_status.aou_state[i] > 10 && gui_status.aou_state[i] < 100) {
			sprintf(analog, "%d  ", gui_status.aou_state[i]);
		} else if (gui_status.aou_state[i] > 100 && gui_status.aou_state[i] < 1000) {
			sprintf(analog, "%d ", gui_status.aou_state[i]);
		} else {
			sprintf(analog, "%d", gui_status.aou_state[i]);
		}

		BSP_LCD_DisplayStringAt(210, i * 30 + 40, analog, LEFT_MODE);
	}

	// Digital PWM duty
	for (uint8_t i = 0; i < 3; i++) {

		if (gui_status.pwm[i] < 10) {
			sprintf(pwm_char, "%d  ", gui_status.pwm[i]);
		} else if (gui_status.pwm[i] > 10 && gui_status.pwm[i] < 100) {
			sprintf(pwm_char, "%d ", gui_status.pwm[i]);
		} else {
			sprintf(pwm_char, "%d", gui_status.pwm[i]);
		}

		BSP_LCD_DisplayStringAt(270, i * 30 + 40, pwm_char, LEFT_MODE);
	}

	// Display Mode
	if (temp_mode != gui_status.mode) {
		temp_mode = gui_status.mode;
		sprintf(mode, "%d", temp_mode);
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_DisplayStringAt(370, 13, mode, LEFT_MODE);
		BSP_LCD_SetFont(&Font12);
	}

	//Display Message
	if (gui_status.message_flag) {
		gui_status.message_flag = 0;
		LCD_UsrLog("gui - Message flag: %d", gui_status.message_flag);
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_SetTextColor(LCD_COLOR_RED);
		BSP_LCD_DisplayStringAt(310, 40, gui_status.message, LEFT_MODE);
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	}

}


/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
