/** TOTORO PLC Project - Slave drivers - ADC init
  ******************************************************************************
  * @file    PLC\Slaves\Slave_drivers\Src\adc.c
  * @author  Gyula Rasztovich
  * @version V1.0
  * @date    10-08-2017
  * @brief   adc init. Base: L4 cube empty template main.c file
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
#include "adc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef adc_handle;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  ADC1 initialization and configuration for A2 pin, 12 bit resolution
  * 		PIN A2 -> PA4 -> ADC12_IN9, GPIO_PIN: 4, GPIOA, Channel 9
  * @param  None
  * @retval None
  */
void adc_init(void)
{
	// Enable ADC clock
	__HAL_RCC_ADC_CLK_ENABLE();

	// Set up ADC
	adc_handle.Instance 			= ADC1;							// Register base address
	adc_handle.Init.ClockPrescaler	= ADC_CLOCK_SYNC_PCLK_DIV2;		// Select ADC clock source
	adc_handle.Init.Resolution 		= ADC_RESOLUTION_12B;			// Resolution: measurement values will between 0...4095 ((2^12)-1)
	adc_handle.Init.DataAlign 		= ADC_DATAALIGN_RIGHT;			// Specify ADC data alignment in conversion data register (right or left).
	/* Other optional settings
		adc_handle.State= HAL_ADC_STATE_RESET;					// ADC communication state (bit-map of ADC states)
		adc_handle.Init.ScanConvMode= ADC_SCAN_DISABLE;			// Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1)
		adc_handle.Init.EOCSelection= ADC_EOC_SEQ_CONV;			// Specify which EOC (End Of Conversion) flag is used for conversion by polling and interruption
		adc_handle.Init.LowPowerAutoWait= DISABLE;				// Select the dynamic low power Auto Delay
		adc_handle.Init.ContinuousConvMode= DISABLE;			// Continuous mode disabled to have only 1 conversion at each conversion trig
		adc_handle.Init.NbrOfConversion= 1;						// Parameter discarded because sequencer is disabled
		adc_handle.Init.DiscontinuousConvMode= DISABLE;			// Parameter discarded because sequencer is disabled
		adc_handle.Init.NbrOfDiscConversion= DISABLE;			// Parameter discarded because sequencer is disabled
		adc_handle.Init.ExternalTrigConv= ADC_SOFTWARE_START;	// Software start to trig the 1st conversion manually, without external event
		adc_handle.Init.DMAContinuousRequests= DISABLE;			// ADC with DMA transfer: continuous requests to DMA disabled (default state) since DMA is not used in this example.
		adc_handle.Init.Overrun= ADC_OVR_DATA_OVERWRITTEN;		// Select the behavior in case of overrun: data overwritten or preserved
	*/

	// Init ADC
	HAL_ADC_Init(&adc_handle);

	// Set up ADC channel
	ADC_ChannelConfTypeDef adc_channel;
	adc_channel.Channel 		= ADC_CHANNEL_9;				// Specify the channel to configure into ADC regular group, according to User manual's Table 23. Arduino connectors on NUCLEO-L476RG
	adc_channel.Rank 			= ADC_REGULAR_RANK_1;			// Specify the rank in the regular group sequencer.
	adc_channel.SamplingTime 	= ADC_SAMPLETIME_24CYCLES_5;	// Sampling time value to be set for the selected channel.
	adc_channel.Offset 			= 0;							// Define the offset to be subtracted from the raw converted data.
	/* Other optional channel settings
		adc_channel.SingleDiff   	= ADC_SINGLE_ENDED;			// Select single-ended or differential input.
		adc_channel.OffsetNumber 	= ADC_OFFSET_1;				// Select the offset number
	 */
	HAL_ADC_ConfigChannel(&adc_handle, &adc_channel);
}

/**
  * @brief  ADC measurement program.
  * @param  None
  * @retval measurement value between 0...4095 (note: adc is initialized for 12 bit resolution)
  */
uint32_t adc_measure()
{
	//uint32_t measurement = 0;
	HAL_ADC_Start(&adc_handle);
	HAL_ADC_PollForConversion(&adc_handle, HAL_MAX_DELAY);
	//measurement = HAL_ADC_GetValue(&adc_handle);
	//return measurement;
	return HAL_ADC_GetValue(&adc_handle);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
