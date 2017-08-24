/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void adc_init(void);
void adc_valid_measurement(void);

/* Global variables --------------------------------------------------------- */
uint16_t valid_data[6];

#endif /* __ADC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/