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
// #define something1		0b11101101	// 237
// #define something2		0b11110111	// 247

// Defines for logic process

/* Logical operators*/
#define AND 	&&
#define OR		||
#define NOT		!

/* Digital input pins value */
#define DIN1 	(din_state & 0b00000001)  //D8 on input board
#define DIN2 	(din_state & 0b00000010)  //D9 on input board
#define DIN3 	(din_state & 0b00000100)  //D10 on input board
#define DIN4 	(din_state & 0b00001000)  //D11 on input board
#define DIN5 	(din_state & 0b00010000)  //D12 on input board
#define DIN6 	(din_state & 0b00100000)  //D13 on input board
#define DIN7 	(din_state & 0b01000000)  //D14 on input board
#define DIN8	(din_state & 0b10000000)  //D15 on input board

/* Digital output pins value */
#define DOU1 	(dout_state & 0b00000001)  //D8 status on output board
#define DOU2 	(dout_state & 0b00000010)  //D9 status on output board
#define DOU3 	(dout_state & 0b00000100)  //D10 status on output board
#define DOU4 	(dout_state & 0b00001000)  //D11 status on output board
#define DOU5 	(dout_state & 0b00010000)  //D12 status on output board
#define DOU6 	(dout_state & 0b00100000)  //D13 status on output board
#define DOU7 	(dout_state & 0b01000000)  //D14 status on output board
#define DOU8	(dout_state & 0b10000000)  //D15 status on output board

/* Write digital output pins */
#define DOU1_ON		(dout_state |= 0b00000001)  //D8 on output board
#define DOU1_OFF	(dout_state &= 0b11111110)	//D8 on output board
#define DOU2_ON		(dout_state |= 0b00000010)	//D9 on output board
#define DOU2_OFF	(dout_state &= 0b11111101)	//D9 on output board
#define DOU3_ON		(dout_state |= 0b00000100)	//D10 on output board
#define DOU3_OFF	(dout_state &= 0b11111011)	//D10 on output board
#define DOU4_ON		(dout_state |= 0b00001000)	//D11 on output board
#define DOU4_OFF	(dout_state &= 0b11110111)	//D11 on output board
#define DOU5_ON		(dout_state |= 0b00010000)	//D12 on output board
#define DOU5_OFF	(dout_state &= 0b11101111)	//D12 on output board
#define DOU6_ON		(dout_state |= 0b00100000)	//D13 on output board
#define DOU6_OFF	(dout_state &= 0b11011111)	//D13 on output board
#define DOU7_ON		(dout_state |= 0b01000000)	//D14 on output board
#define DOU7_OFF	(dout_state &= 0b10111111)	//D14 on output board
#define DOU8_ON		(dout_state |= 0b10000000)	//D15 on output board
#define DOU8_OFF	(dout_state &= 0b01111111)	//D15 on output board

/* Analog input value of pins */
#define AIN1		ain_state[0]
#define AIN2		ain_state[1]
#define AIN3		ain_state[2]
#define AIN4		ain_state[3]
#define AIN5		ain_state[4]
#define AIN6		ain_state[5]

/* Write analoge output pins */
#define AOU1=		aout_state[0]=
#define AOU2=		aout_state[1]=
#define AOU3=		aout_state[2]=
#define AOU4=		aout_state[3]=
#define AOU5=		aout_state[4]=
#define AOU6=		aout_state[5]=


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
 * */
typedef struct {
	uint8_t slave_address;
	uint8_t digital_pins_state;
	uint8_t slave_status;
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
}analog_tale_t;


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
	uint8_t zeros[16]
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
analog_tale_t analog_input_slaves[4];
uint8_t num_of_an_in;

/* This array stores data for analog otput slaves */
analog_tale_t analog_output_slaves[4];
uint8_t num_of_an_out;


/* Exported functions ------------------------------------------------------- */

void control_slaves_thread(void const * argument);

#endif /* __MASTER_LOOP_CONTROL_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
