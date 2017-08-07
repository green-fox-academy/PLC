/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MASTER_LOOP_CONTROL_H
#define __MASTER_LOOP_CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported functions ------------------------------------------------------- */

void control_slaves_thread(void const * argument);
void master_logic_thread(void const * argument);

#endif /* __MASTER_LOOP_CONTROL_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
