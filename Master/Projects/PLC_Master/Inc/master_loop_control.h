/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MASTER_LOOP_CONTROL_H
#define __MASTER_LOOP_CONTROL_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/* Exported constants --------------------------------------------------------*/

/* Command list */
#define SCAN_SLAVE	3
#define READ_DATA	14
#define WRITE_DATA	48

/* Exported types ------------------------------------------------------------*/

/* Struct for store digital slave pin state*/
typedef struct {
	uint8_t slave_address;
	uint8_t digital_pins_state;
}slave_digital_t;

/* Struct for store analoge slave pin state*/
typedef struct {
	uint8_t slave_address;
	uint16_t analoge_pins_state[6];
}slave_analog_t;

/* Struct for frame format */
typedef struct {
	uint8_t address;
	uint8_t command;
	uint8_t* data;
	uint8_t data_length;
	uint16_t crc;
}frame_t;

typedef struct {
	uint8_t address;
	uint8_t command;
	uint8_t data[2];
	uint16_t crc;
}frame_out_digital_in_t;

typedef struct {
	uint8_t address;
	uint8_t command;
	uint8_t data[2];
	uint16_t crc;
}frame_out_digital_out_t;

typedef struct {
	uint8_t address;
	uint8_t command;
	uint16_t data[6];
	uint16_t crc;
}frame_analog_out_t;

typedef struct {
	uint8_t address;
	uint8_t command;
	uint8_t range[2];
	uint16_t crc;
}frame_analog_in_t;

/* Exported variables ------------------------------------------------------- */

/* This array stores data from digital input slaves */
slave_digital_t digital_input_slaves[4];

/* This array stores data for digital otput slaves */
slave_digital_t digital_output_slaves[4];

/* This array stores data from analog input slaves */
slave_analog_t analog_input_slaves[4];

/* This array stores data for analog otput slaves */
slave_analog_t analog_output_slaves[4];

/* Theese arrays contains the addresses of the slaves */
const uint8_t digital_input_slaves_address[]  = {1,2,3,4};
const uint8_t digital_output_slaves_address[] = {5,6,7,8};
const uint8_t analog_input_slaves_address[]   = {9,10,11,12};
const uint8_t analog_output_slaves_address[]  = {13,14,15,16};


/* Exported functions ------------------------------------------------------- */

void control_slaves_thread(void const * argument);
void master_logic_thread(void const * argument);

#endif /* __MASTER_LOOP_CONTROL_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
