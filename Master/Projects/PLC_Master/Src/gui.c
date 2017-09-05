/* Includes ------------------------------------------------------------------*/
#include "gui.h"
#include "stm32746g_discovery_lcd.h"
#include "msg_queue.h"

void starting_screen()
{
	char num[2];

	BSP_LCD_Clear(LCD_COLOR_LIGHTBLUE);

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetBackColor(LCD_COLOR_LIGHTBLUE);

	BSP_LCD_DisplayStringAt(15, 13, "D_IN", LEFT_MODE);
	BSP_LCD_DisplayStringAt(70, 13, "D_OUT", LEFT_MODE);

	for (uint8_t i = 0; i < 9; i++) {
		BSP_LCD_DrawRect(1, (i * 30 + 1), 60, 30);
		BSP_LCD_DrawRect(61, (i * 30 + 1), 60, 30);
	}


	for (uint8_t i = 0; i < 8; i++) {

		sprintf(num, "%d", i+1);

		BSP_LCD_DisplayStringAt(16, i * 30 + 40, num, LEFT_MODE);
		BSP_LCD_FillCircle(46, i * 30 + 46, 10);

		BSP_LCD_DisplayStringAt(76, i * 30 + 40, num, LEFT_MODE);
		BSP_LCD_FillCircle(106, i * 30 + 46, 10);
	}
}



/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
