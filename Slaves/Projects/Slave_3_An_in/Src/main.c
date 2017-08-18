/** TOTORO PLC Project - Slave 3 - Analog In
  ******************************************************************************
  * @file    PLC\Slaves\Projects\Slave_3_An_in\Src\main.c
  * @author  Gyula Rasztovich
  * @version V1.0
  * @date    08-08-2017
  * @brief   Main program body for Slave Analog In board.
  * 		 Base: L4 cube empty template file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// uint8_t analog_pin = 0; //user input for which analog input pin is used; 0..5 equals A0..A5
// uint8_t digital_pin = 2; //user input for which digital input pin is used; 0..15 equals D0..D15
uint16_t pot_measurement[10];
uint8_t potnumber;
uint32_t sum;

/* Private function prototypes -----------------------------------------------*/
uint16_t pot_measure_avg(uint8_t potnumber);
static void SystemClock_Config(void);

int main(void)
{
	/* STM32L4xx HAL library initialization:
	   - Configure the Flash prefetch, Flash preread and Buffer caches
	   - Systick timer is configured by default as source of time base, but user
			 can eventually implement his proper time base source (a general purpose
			 timer for example or other time source), keeping in mind that Time base
			 duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
			 handled in milliseconds basis.
	   - Low Level Initialization
	 */
	HAL_Init();

	/* Configure the System clock to have a frequency of 80 MHz */
	SystemClock_Config();

	/* Initialize LED on board */
	BSP_LED_Init(LED2);

	// Init UART
	uart_init();
	uart_send("UART initialized\n\r");

	/* Add your application code here */
	// Init GPIO digital pin for LED
	for (uint8_t i = 2; i < 8; i++) {
		gpio_init_digital_pin(i, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);
	}
	uart_send("LEDs initialized\n\r");

	// Init GPIO analog pin for ADC (argument: A(x) pin)
	for (uint8_t i = 0; i < 6; i++) {
		gpio_init_analog_pin(i);
	}

	// Init ADC
	adc_init();
	uart_send("ADC initialized\n\r");

	uint8_t selected_pot = 0;
	uint32_t sum_all = 0;
	uint32_t sum_1 = 0;

	// loop - i measurement series for each potmeter
	uint8_t i = 1;
	while (i < 6) {
		uint16_t adc_measured_value[6];
		for (uint8_t j = 0; j < 6; j++) {
			if (adc_measure(j) > 4095 ) { // note: the value depends on the ADC resolution! its 2^<res>!
				BSP_LED_Toggle(LED2);
				uart_send("ADC measurement error!\n\r");
				return -1;
			} else if (adc_measure(j) > 4095/2 ) {
				BSP_LED_On(LED2);
				gpio_set_digital_pin(j+2);
			} else {
				BSP_LED_Off(LED2);
				gpio_reset_digital_pin(j+2);
			}

			char value_in_string[6];
			char counter[2];
			char pot_counter[2];
			adc_measured_value[j] = adc_measure(j);
			sum_all += adc_measured_value[j];
			sprintf(value_in_string, "%d", adc_measured_value[j]);
			sprintf(counter, "%d", i);
			sprintf(pot_counter, "%d", j+1);
			uart_send(counter);
			uart_send("th measurement value on #");
			uart_send(pot_counter);
			uart_send(" potentiometer: ");
			uart_send(value_in_string);
			uart_send("\n\r");
		}
		sum_1 += adc_measured_value[selected_pot];
		uart_receive();
		uart_send("\n\r");
		HAL_Delay(100);
		i++;
	}
	pot_measure_avg(selected_pot);
	adc_deinit();
	uart_send("ADC measurement finished.\n\r");
	char summ[5];
	sprintf(summ, "%d", pot_measure_avg(selected_pot));
	uart_send("Average of the last ten measurements on the selected potentiometer: ");
	uart_send(summ);
	uart_send(".\n\r");
	char summ_all[5];
	sprintf(summ_all, "%d", sum_all);
	uart_send("Sum of every pot meaurement: ");
	uart_send(summ_all);
	uart_send(".\n\r");
	char summ_1[5];
	sprintf(summ_1, "%d", sum_1);
	uart_send("Sum of the selected pot measurements: ");
	uart_send(summ_1);
	uart_send(".\n\r");
}
// writetofile function...





uint16_t pot_measure_avg(uint8_t potnumber)
{
	uint32_t sum = 0;
	for (uint8_t i = 0; i < 10; i++) {
		pot_measurement[i] = adc_measure(potnumber);
		HAL_Delay(250);
		sum += pot_measurement[i];
	}
	return (uint16_t)(sum/10);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
