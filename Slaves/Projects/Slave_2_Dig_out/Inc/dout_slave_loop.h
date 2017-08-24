/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DOUT_SLAVE_LOOP_H
#define __DOUT_SLAVE_LOOP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "uart.h"
#include "GPIO.h"

/* Exported constants --------------------------------------------------------*/

/* Command list with lexicode 3distance*/
#define SCAN_SLAVE		0b00001111  // 15
#define WRITE_SLAVE		0b01010101	// 55
#define HOLD_INPUTS		0b01101010	// 106
#define HOLD_OUTPUTS	0b10010110	// 150
#define STOP_SLAVE		0b10101011	// 171
#define START_SLAVE		0b11011011	// 219
#define MODE_1			0b11101101	// 237
#define MODE_2			0b11110111	// 247

/* Exported types ------------------------------------------------------------*/

/* Exported variables ------------------------------------------------------- */

// This data holds the actual pin states
uint8_t slave_mode;

uint8_t pwm_set_arr[3];

/* Exported functions ------------------------------------------------------- */

void dout_slave_loop_thread();

#endif /* __DOUT_SLAVE_LOOP_H*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
