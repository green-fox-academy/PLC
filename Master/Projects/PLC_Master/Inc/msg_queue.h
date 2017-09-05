/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MSG_QUEUE_H
#define __MSG_QUEUE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported functions ------------------------------------------------------- */

/* Exported types ----------------------------------------------------------- */

typedef struct {
	uint8_t din_state;
	uint8_t dout_state;
	uint16_t ain_state[6];
	uint16_t aou_state[3];
};


#endif /* __MSG_QUEUE_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
