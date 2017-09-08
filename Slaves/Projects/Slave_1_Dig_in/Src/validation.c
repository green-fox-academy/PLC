#include "validation.h"
#include "GPIO.h"

uint8_t pin_validation(uint8_t from_pin_index, uint8_t to_pin_index)
{
	uint8_t pins_states = 0;
	uint8_t data_task = 0;

	for (uint8_t i = from_pin_index; i  <= to_pin_index; i++) {

		for (int j = 0; j < 10; j++) {
			data_task += gpio_read_digital_pin(i);
		}

		if (data_task == 10) {
			pins_states += (1 << (i - from_pin_index));
		}

		data_task = 0;
	}
	return pins_states;
}
