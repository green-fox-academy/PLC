/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MASTER_LOOP_CONTROL_H
#define __MASTER_LOOP_CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported constants --------------------------------------------------------*/

/* Command list with lexicode 3distance*/
#define SCAN_SLAVE		0b00001111  // 15
#define READ_SLAVE		0b00110011	// 33
#define WRITE_SLAVE		0b01010101	// 55
#define HOLD_INPUTS		0b01101010	// 106
#define HOLD_OUTPUTS	0b10010110	// 150
#define STOP_SLAVE		0b10101011	// 171
#define START_SLAVE		0b11011011	// 219
#define MODE_1			0b11101101	// 237
#define MODE_2			0b11110111	// 247


/* Exported types ------------------------------------------------------------*/

/* ################################################
 * #### Tables for store input and output data ####
 * ################################################ */

/* Struct for store digital slaves address, pinstate, status
 * status: 0 if it is Ok
 * status: 1 if address or command msg corrupted
 * status: 2 if CRC was corrupted
 * status: 3 if address or command and CRC was corrupted
 * status: 4 if message timed out
 *
 * mode:	MODE_1 if it is in GPIO input mode 			/ If its an input slave
 * mode:	MODE_2 if it is in PWM mode	 				/ If its an input slave
 * mode:	MODE_1 if it is in GPIO output mode 		/ If its an output slave
 * mode:	MODE_2 if it is in Freq measurement mode	/ If its an output slave
 * */
typedef struct {
	uint8_t slave_address;
	uint8_t digital_pins_state;
	uint8_t pwm_duty_arr[3];
	uint8_t slave_status;
	uint8_t mode;
}digital_table_t;

/* Struct for store analog slaves address, pinstate, status
 * status: 0 if it is Ok
 * status: 1 if address or command msg corrupted
 * status: 2 if CRC was corrupted
 * status: 3 if address or command and CRC was corrupted
 * status: 4 if message timed out
 * */
typedef struct {
	uint8_t slave_address;
	uint16_t analoge_pins_state[6];
	uint8_t slave_status;
}analog_table_t;


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

/* Master -> analog out slave
 * analog in slave -> master
 * analog out slave -> master
 */
typedef struct {
	uint8_t address;
	uint8_t command;
	uint16_t data[6];
	uint16_t crc;
	uint8_t zeros[16];
}data_6x16bit_t;

/* Exported variables ------------------------------------------------------- */

/* ###########################
 * #### Message variables ####
 * ########################### */
command_slave_t msg_command;
data_1x8bit_t msg_1x8b;
data_6x16bit_t msg_6x16b;


/* #####################################################
 * #### Table variables for store input/output data ####
 * ##################################################### */

/* This array stores data from digital input slaves */
digital_table_t digital_input_slaves[4];
uint8_t num_of_dig_in;

/* This array stores data for digital otput slaves */
digital_table_t digital_output_slaves[4];
uint8_t num_of_dig_out;

/* This array stores data from analog input slaves */
analog_table_t analog_input_slaves[4];
uint8_t num_of_an_in;

/* This array stores data for analog otput slaves */
analog_table_t analog_output_slaves[4];
uint8_t num_of_an_out;


/* Exported functions ------------------------------------------------------- */

void control_slaves_thread();

#endif /* __MASTER_LOOP_CONTROL_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
