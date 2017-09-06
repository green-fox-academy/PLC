/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GUI_H
#define __GUI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

typedef struct {
	uint8_t din_state;
	uint8_t dout_state;
	uint16_t ain_state[6];
	uint16_t aou_state[3];
}gui_data;

gui_data gui_status;

/* Exported functions ------------------------------------------------------- */
void starting_screen();
void gui_display_status();



#endif /* __GUI_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
