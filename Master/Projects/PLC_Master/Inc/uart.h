#ifndef __UART_H_
#define __UART_H_

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdlib.h>

#define TXBUFFERSIZE        20			// transmit
#define RXBUFFERSIZE        20			// receive

typedef enum {NO_COMMAND, HELP, GET_VALUE, SET_VALUE} uart_command_t;
typedef enum {NO_ATTRIB, PULSE, ANGLE, POSITION, MANUAL_CONTROL, DATA_DISP, DEMO} uart_attrib_t;

typedef struct {
	uint8_t buffer[RXBUFFERSIZE];
	uint8_t* head_p;
	uint8_t* tail_p;
	uint8_t* read_p;
	uint8_t* write_p;
} circular_buffer_t;

typedef struct {
	uart_command_t command;
	uart_attrib_t attrib;
	uint8_t device_id;
	int16_t value;
	int16_t value_x;
	int16_t value_y;
	int16_t value_z;
	uint8_t error;
} uart_command_struct_t;

uart_command_struct_t c_params;
UART_HandleTypeDef uart_handle;

uint8_t char_buff;
uint8_t TX_buffer[TXBUFFERSIZE];
circular_buffer_t RX_buffer;
uint8_t command_buffer[RXBUFFERSIZE];

uint8_t command_in;





#endif /* __UART_H_ */
