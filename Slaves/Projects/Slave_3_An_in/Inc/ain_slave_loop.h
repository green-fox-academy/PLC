/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AIN_SLAVE_LOOP_H
#define __AIN_SLAVE_LOOP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "uart.h"
#include "GPIO.h"
#include "adc.h"

/* Exported constants --------------------------------------------------------*/

/* Command list with lexicode 3distance*/
#define SCAN_SLAVE		0b00001111  // 15
#define READ_SLAVE		0b00110011	// 33
#define HOLD_INPUTS		0b01101010	// 106
#define HOLD_OUTPUTS	0b10010110	// 150
#define STOP_SLAVE		0b10101011	// 171
#define START_SLAVE		0b11011011	// 219

/* Exported types ------------------------------------------------------------*/

/* Exported variables ------------------------------------------------------- */

/* Exported functions ------------------------------------------------------- */

void ain_slave_loop_thread();

#endif /* __AIN_SLAVE_LOOP_H*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
