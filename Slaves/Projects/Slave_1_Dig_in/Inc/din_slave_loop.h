/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DIN_SLAVE_LOOP_H
#define __DIN_SLAVE_LOOP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "uart.h"
#include "GPIO.h"

/* Exported constants --------------------------------------------------------*/

/* Command list with lexicode 3distance*/
#define SCAN_SLAVE		0b00001111  // 15
#define READ_SLAVE		0b00110011	// 33
#define HOLD_INPUTS		0b01101010	// 106
#define HOLD_OUTPUTS	0b10010110	// 150
#define STOP_SLAVE		0b10101011	// 171
#define START_SLAVE		0b11011011	// 219

/* Exported types ------------------------------------------------------------*/

/* ###############################################
 * #### Message frames for different commands ####
 * ############################################### */

typedef struct{
	uint8_t address;
	uint8_t command;
	uint16_t crc;
	uint8_t zeros[28];
}command_slave_t;

// Master -> Slave one byte data transfare
typedef struct {
	uint8_t address;
	uint8_t command;
	uint8_t data;
	uint16_t crc;
	uint8_t zeros[27];
}data_1x8bit_t;

/* Exported variables ------------------------------------------------------- */

/* ###########################
 * #### Message variables ####
 * ########################### */
command_slave_t msg_command;
data_1x8bit_t msg_1x8b;

// This data holds the actual pin states
uint8_t din_pins_states;

/* Exported functions ------------------------------------------------------- */

void din_slave_loop_thread();

#endif /* __DIN_SLAVE_LOOP_H*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
