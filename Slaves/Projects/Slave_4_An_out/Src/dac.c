/** TOTORO PLC Project - DAC init
  ******************************************************************************
  * @file    PLC\Slaves\Projects\Slave_4_An_out\Src\dac.c
  * @author  Gyula Rasztovich
  * @version V1.0
  * @date    23-08-2017
  * @brief   DAC init. Base: L4 cube empty template main.c file
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
#include "dac.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef dac_handle;
static DAC_ChannelConfTypeDef dac_channel_1;
static DAC_ChannelConfTypeDef dac_channel_2;
uint16_t A2_pin_output_voltage; // set A2 pin (PA4 pin) output voltage
uint16_t D13_pin_output_voltage; // set D13 pin (PA5 pin) output voltage

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// Set DAC handle struct
void dac_set_handle(void)
{
	// Enable DAC clock
	__HAL_RCC_DAC1_CLK_ENABLE();

	// Config DAC handle
	dac_handle.Instance = DAC;		//Register base address

	// Init DAC handle
	HAL_DAC_Init(&dac_handle);
}

// Init DAC channel 1
void dac_1_init(void) // voltage output on A2 pin (PA4), resolution: 12 bit
{
	dac_set_handle();

	// Config DAC channel 1
	dac_channel_1.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;	// Specifies whether the DAC mode.
	dac_channel_1.DAC_Trigger = DAC_TRIGGER_NONE;					// Specifies the external trigger for the selected DAC channel.
	dac_channel_1.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;		// Specifies whether the DAC channel output buffer is enabled or disabled.
	dac_channel_1.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;	// Specifies whether the DAC output is connected or not to on chip peripheral
	dac_channel_1.DAC_UserTrimming = DAC_TRIMMING_FACTORY;				// Specifies the trimming mode
	/* alternate settings
		dac_channel_1.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_ENABLE;
		dac_channel_1.DAC_Trigger = DAC_TRIGGER_NONE;
		dac_channel_1.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
		dac_channel_1.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
		dac_channel_1.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
		*/
		dac_channel_1.DAC_SampleAndHoldConfig.DAC_SampleTime = 20;
		dac_channel_1.DAC_SampleAndHoldConfig.DAC_HoldTime = 10;
		dac_channel_1.DAC_SampleAndHoldConfig.DAC_RefreshTime = 5;

	HAL_DAC_ConfigChannel(&dac_handle, &dac_channel_1, DAC_CHANNEL_1);	// Configures the selected DAC channel.

	// Set DAC channel 1 DHR register
	HAL_DAC_SetValue(&dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);	// Set the specified data holding register value for DAC channel.

	// Enable DAC channel 1
	HAL_DAC_Start(&dac_handle, DAC_CHANNEL_1);	// Enables DAC and starts conversion of channel.
}

// Init DAC channel 2
void dac_2_init(void)
{
	dac_set_handle();

	// Config DAC channel 2
	dac_channel_2.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;	// Specifies whether the DAC mode.
	dac_channel_2.DAC_Trigger = DAC_TRIGGER_NONE;					// Specifies the external trigger for the selected DAC channel.
	dac_channel_2.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;		// Specifies whether the DAC channel output buffer is enabled or disabled.
	dac_channel_2.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;	// Specifies whether the DAC output is connected or not to on chip peripheral
	dac_channel_2.DAC_UserTrimming = DAC_TRIMMING_FACTORY;				// Specifies the trimming mode
	/* alternate settings
		dac_channel_2.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_ENABLE;
		dac_channel_2.DAC_Trigger = DAC_TRIGGER_NONE;
		dac_channel_2.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
		dac_channel_2.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
		dac_channel_2.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
		dac_channel_2.DAC_SampleAndHoldConfig.DAC_SampleTime = 20;
		dac_channel_2.DAC_SampleAndHoldConfig.DAC_HoldTime = 10;
		dac_channel_2.DAC_SampleAndHoldConfig.DAC_RefreshTime = 5;
	*/
	HAL_DAC_ConfigChannel(&dac_handle, &dac_channel_1, DAC_CHANNEL_2);	// Configures the selected DAC channel.

	// Set DAC channel 2 DHR register
	HAL_DAC_SetValue(&dac_handle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0);	// Set the specified data holding register value for DAC channel.

	// Enable DAC channel 2
	HAL_DAC_Start(&dac_handle, DAC_CHANNEL_2);	// Enables DAC and starts conversion of channel.

}

void dac_1_setval(uint16_t dat1)
{
	HAL_DAC_SetValue(&dac_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dat1);
}

void dac_2_setval(uint16_t dat2)
{
	HAL_DAC_SetValue(&dac_handle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, dat2);
}

// Init both DAC channels
void dac_init()
{
	dac_1_init();
	dac_2_init();
}

// Deinit DAC
void dac_deinit(void)
{
	HAL_DAC_DeInit(&dac_handle);
	return;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
