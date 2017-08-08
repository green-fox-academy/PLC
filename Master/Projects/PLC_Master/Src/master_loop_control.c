/* Includes ------------------------------------------------------------------*/
#include "master_loop_control.h"
#include "modbus.h"
#include "cmsis_os.h"
#include "main.h"
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/*
 * Loop  cycle:
 * 	1 - CPU check
 * 	2 - SLaves check
 * 	3 - Scan Inputs (update input tables)
 * 	4 - Execute Program (update output tables)
 * 	5 - Update outputs (send data to slaves)
 */

void control_slaves_thread(void const * argument)
{

	scan_slaves();

	while (1) {

		/* */
		// system_check();
		// slaves_check();
		scan_inputs();
		execute_program();
		upadte_outputs();

		osDelay(1);

	}
}

void scan_slaves()
{
	for (uint8_t i = 1; i < 5; i++) {
		digital_input_slaves_address[i]
	}

}

uint8_t* create_message(frame)
{

}






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
