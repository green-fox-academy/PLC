/** TOTORO PLC Project - Slave drivers - ADC init
  ******************************************************************************
  * @file    PLC\Slaves\Slave_drivers\Src\adc.c
  * @author  Gyula Rasztovich
  * @version V1.0
  * @date    23-08-2017
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
ADC_HandleTypeDef dac_handle;
ADC_ChannelConfTypeDef adc_channel[6];
uint16_t temp_data[10];
uint16_t temp_sum;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint16_t trimmed_mean();

/**
  * @brief  ADC1 initialization and configuration
  * @param  None
  * @retval None
  */
void adc_init(void)
{
	// Enable ADC clock
	__HAL_RCC_ADC_CLK_ENABLE();

	// Config ADC
	dac_handle.Instance 			= ADC1;						// Register base address - ADC1 for all
	dac_handle.Init.ClockPrescaler	= ADC_CLOCK_SYNC_PCLK_DIV2;	// Select ADC clock source
	dac_handle.Init.Resolution 		= ADC_RESOLUTION_12B;		// Resolution: measurement values will between 0...4095 ((2^12)-1)
	dac_handle.Init.DataAlign 		= ADC_DATAALIGN_RIGHT;		// Data right alignment
	dac_handle.Init.EOCSelection	= ADC_EOC_SINGLE_CONV;		// Specify which EOC (End Of Conversion) flag is used for conversion by polling and interruption
	/* Other optional ADC settings
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
	HAL_ADC_Init(&dac_handle);

	// Config ADC channel - channels are different for each pin, other settings are the same for each.
	adc_channel[0].Channel 	= ADC_CHANNEL_5;					// A0 - PA0 - ADC12_IN5 	Specify the channel to configure into ADC regular group.
	adc_channel[1].Channel 	= ADC_CHANNEL_6;					// A1 - PA1 - ADC12_IN6		ref: user manual's Table 23.
	adc_channel[2].Channel 	= ADC_CHANNEL_9;					// A2 - PA4 - ADC12_IN9
	adc_channel[3].Channel 	= ADC_CHANNEL_15;					// A3 - PB0 - ADC12_IN15
	adc_channel[4].Channel 	= ADC_CHANNEL_2;					// A4 - PC1 - ADC123_IN2
	adc_channel[5].Channel 	= ADC_CHANNEL_1;					// A5 - PC0 - ADC123_IN1
	for (uint8_t i = 0; i < 6; i++) {
		adc_channel[i].Rank 		= ADC_REGULAR_RANK_1;		// Specify the rank in the regular group sequencer.
		adc_channel[i].SamplingTime = ADC_SAMPLETIME_24CYCLES_5;// Sampling time value to be set for the selected channel.
		adc_channel[i].Offset 		= 0;						// Offset to be subtracted from the raw converted data.
	}
	/* Other optional channel settings
		adc_channel[i].SingleDiff   = ADC_SINGLE_ENDED;			// Select single-ended or differential input.
		adc_channel[i].OffsetNumber = ADC_OFFSET_1;				// Select the offset number
	}
	*/
}

/**
  * @brief  deinit ADC1
  * @param  None
  * @retval None
  */
void adc_deinit(void)
{
	HAL_ADC_Stop(&dac_handle);
	return;
}

/**
  * @brief  ADC measurement program.
  * @param  pin number of potmeter's connection to board between 0..5 (refers to A0..A5 pins)
  * @retval measurement value between 0...4095 (note: adc is initialized for 12 bit resolution)
  */
uint16_t adc_measure(uint8_t index)
{
	HAL_ADC_ConfigChannel(&dac_handle, &adc_channel[index]);
	HAL_ADC_Start(&dac_handle);
	HAL_ADC_PollForConversion(&dac_handle, HAL_MAX_DELAY);
	return HAL_ADC_GetValue(&dac_handle);
}

void adc_valid_measurement(void)
{
	for (uint8_t i = 0; i < 6; i++) {
		temp_sum = 0;
		for (uint8_t j = 0; j < 10; j++) {
			temp_data[j] = adc_measure(i);
			temp_sum += temp_data[j];
		}
		valid_data[i] = trimmed_mean();
	}
}

uint16_t trimmed_mean()
{
	uint16_t max = temp_data[0];
	for (uint8_t i = 0; i < 10; i++) {
		if (temp_data[i] > max) {
			max = temp_data[i];
		}
	}
	uint16_t min = temp_data[0];
	for (uint8_t i = 0; i < 10; i++) {
		if (temp_data[i] < min) {
			min = temp_data[i];
		}
	}
	return (temp_sum - min - max)/8;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
