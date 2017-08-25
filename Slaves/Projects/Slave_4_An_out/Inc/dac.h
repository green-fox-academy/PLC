
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DAC_H
#define __DAC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
uint16_t dac_pin_state[2];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void dac_init(void);
void dac_1_setval(uint16_t dat1);
void dac_2_setval(uint16_t dat2);

#endif /* __DAC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
