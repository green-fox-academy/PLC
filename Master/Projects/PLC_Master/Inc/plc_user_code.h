/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLC_USER_CODE_H
#define __PLC_USER_CODE_H

/* Includes ------------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Logical operators*/
#define AND 	&&
#define OR		||
#define NOT		!

/* ######################################## *
 * ##### DIGITAL INPUT SLAVE PINSTATE ##### *
 * ######################################## */

// Digital input slave 1
#define DIN1 	(digital_input_slaves[0].digital_pins_state & 0b00000001)  //D8 on input board
#define DIN2 	(digital_input_slaves[0].digital_pins_state & 0b00000010)  //D9 on input board
#define DIN3 	(digital_input_slaves[0].digital_pins_state & 0b00000100)  //D10 on input board
#define DIN4 	(digital_input_slaves[0].digital_pins_state & 0b00001000)  //D11 on input board
#define DIN5 	(digital_input_slaves[0].digital_pins_state & 0b00010000)  //D12 on input board
#define DIN6 	(digital_input_slaves[0].digital_pins_state & 0b00100000)  //D13 on input board
#define DIN7 	(digital_input_slaves[0].digital_pins_state & 0b01000000)  //D14 on input board
#define DIN8	(digital_input_slaves[0].digital_pins_state & 0b10000000)  //D15 on input board

// Digital input slave 2
#define DIN9 	(digital_input_slaves[1].digital_pins_state & 0b00000001)  //D8 on input board
#define DIN10 	(digital_input_slaves[1].digital_pins_state & 0b00000010)  //D9 on input board
#define DIN11	(digital_input_slaves[1].digital_pins_state & 0b00000100)  //D10 on input board
#define DIN12 	(digital_input_slaves[1].digital_pins_state & 0b00001000)  //D11 on input board
#define DIN13 	(digital_input_slaves[1].digital_pins_state & 0b00010000)  //D12 on input board
#define DIN14 	(digital_input_slaves[1].digital_pins_state & 0b00100000)  //D13 on input board
#define DIN15 	(digital_input_slaves[1].digital_pins_state & 0b01000000)  //D14 on input board
#define DIN16	(digital_input_slaves[1].digital_pins_state & 0b10000000)  //D15 on input board

// Digital input slave 3
#define DIN17 	(digital_input_slaves[2].digital_pins_state & 0b00000001)  //D8 on input board
#define DIN18 	(digital_input_slaves[2].digital_pins_state & 0b00000010)  //D9 on input board
#define DIN19 	(digital_input_slaves[2].digital_pins_state & 0b00000100)  //D10 on input board
#define DIN20 	(digital_input_slaves[2].digital_pins_state & 0b00001000)  //D11 on input board
#define DIN21 	(digital_input_slaves[2].digital_pins_state & 0b00010000)  //D12 on input board
#define DIN22 	(digital_input_slaves[2].digital_pins_state & 0b00100000)  //D13 on input board
#define DIN23 	(digital_input_slaves[2].digital_pins_state & 0b01000000)  //D14 on input board
#define DIN24	(digital_input_slaves[2].digital_pins_state & 0b10000000)  //D15 on input board

// Digital input slave 4
#define DIN25 	(digital_input_slaves[3].digital_pins_state & 0b00000001)  //D8 on input board
#define DIN26 	(digital_input_slaves[3].digital_pins_state & 0b00000010)  //D9 on input board
#define DIN27 	(digital_input_slaves[3].digital_pins_state & 0b00000100)  //D10 on input board
#define DIN28 	(digital_input_slaves[3].digital_pins_state & 0b00001000)  //D11 on input board
#define DIN29 	(digital_input_slaves[3].digital_pins_state & 0b00010000)  //D12 on input board
#define DIN30 	(digital_input_slaves[3].digital_pins_state & 0b00100000)  //D13 on input board
#define DIN31 	(digital_input_slaves[3].digital_pins_state & 0b01000000)  //D14 on input board
#define DIN32	(digital_input_slaves[3].digital_pins_state & 0b10000000)  //D15 on input board

/* ################################################# *
 * ##### WRITE DIGITAL OUTPUT SLAVE'S PINSTATE ##### *
 * ################################################# */

// Digital output slave 1
#define DOU1_ON		(digital_output_slaves[0].digital_pins_state |= 0b00000001) //D8 on output board
#define DOU1_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111110)	//D8 on output board
#define DOU2_ON		(digital_output_slaves[0].digital_pins_state |= 0b00000010)	//D9 on output board
#define DOU2_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111101)	//D9 on output board
#define DOU3_ON		(digital_output_slaves[0].digital_pins_state |= 0b00000100)	//D10 on output board
#define DOU3_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111011)	//D10 on output board
#define DOU4_ON		(digital_output_slaves[0].digital_pins_state |= 0b00001000)	//D11 on output board
#define DOU4_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11110111)	//D11 on output board
#define DOU5_ON		(digital_output_slaves[0].digital_pins_state |= 0b00010000)	//D12 on output board
#define DOU5_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11101111)	//D12 on output board
#define DOU6_ON		(digital_output_slaves[0].digital_pins_state |= 0b00100000)	//D13 on output board
#define DOU6_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11011111)	//D13 on output board
#define DOU7_ON		(digital_output_slaves[0].digital_pins_state |= 0b01000000)	//D14 on output board
#define DOU7_OFF	(digital_output_slaves[0].digital_pins_state &= 0b10111111)	//D14 on output board
#define DOU8_ON		(digital_output_slaves[0].digital_pins_state |= 0b10000000)	//D15 on output board
#define DOU8_OFF	(digital_output_slaves[0].digital_pins_state &= 0b01111111)	//D15 on output board

// Digital output slave 2
#define DOU9_ON		(digital_output_slaves[0].digital_pins_state |= 0b00000001) //D8 on output board
#define DOU9_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111110)	//D8 on output board
#define DOU10_ON	(digital_output_slaves[0].digital_pins_state |= 0b00000010)	//D9 on output board
#define DOU10_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111101)	//D9 on output board
#define DOU11_ON	(digital_output_slaves[0].digital_pins_state |= 0b00000100)	//D10 on output board
#define DOU11_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111011)	//D10 on output board
#define DOU12_ON	(digital_output_slaves[0].digital_pins_state |= 0b00001000)	//D11 on output board
#define DOU12_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11110111)	//D11 on output board
#define DOU13_ON	(digital_output_slaves[0].digital_pins_state |= 0b00010000)	//D12 on output board
#define DOU13_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11101111)	//D12 on output board
#define DOU14_ON	(digital_output_slaves[0].digital_pins_state |= 0b00100000)	//D13 on output board
#define DOU14_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11011111)	//D13 on output board
#define DOU15_ON	(digital_output_slaves[0].digital_pins_state |= 0b01000000)	//D14 on output board
#define DOU15_OFF	(digital_output_slaves[0].digital_pins_state &= 0b10111111)	//D14 on output board
#define DOU16_ON	(digital_output_slaves[0].digital_pins_state |= 0b10000000)	//D15 on output board
#define DOU16_OFF	(digital_output_slaves[0].digital_pins_state &= 0b01111111)	//D15 on output board

// Digital output slave 3
#define DOU17_ON	(digital_output_slaves[0].digital_pins_state |= 0b00000001) //D8 on output board
#define DOU17_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111110)	//D8 on output board
#define DOU18_ON	(digital_output_slaves[0].digital_pins_state |= 0b00000010)	//D9 on output board
#define DOU18_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111101)	//D9 on output board
#define DOU19_ON	(digital_output_slaves[0].digital_pins_state |= 0b00000100)	//D10 on output board
#define DOU19_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111011)	//D10 on output board
#define DOU20_ON	(digital_output_slaves[0].digital_pins_state |= 0b00001000)	//D11 on output board
#define DOU20_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11110111)	//D11 on output board
#define DOU21_ON	(digital_output_slaves[0].digital_pins_state |= 0b00010000)	//D12 on output board
#define DOU21_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11101111)	//D12 on output board
#define DOU22_ON	(digital_output_slaves[0].digital_pins_state |= 0b00100000)	//D13 on output board
#define DOU22_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11011111)	//D13 on output board
#define DOU23_ON	(digital_output_slaves[0].digital_pins_state |= 0b01000000)	//D14 on output board
#define DOU23_OFF	(digital_output_slaves[0].digital_pins_state &= 0b10111111)	//D14 on output board
#define DOU24_ON	(digital_output_slaves[0].digital_pins_state |= 0b10000000)	//D15 on output board
#define DOU24_OFF	(digital_output_slaves[0].digital_pins_state &= 0b01111111)	//D15 on output board

// Digital output slave 4
#define DOU25_ON	(digital_output_slaves[0].digital_pins_state |= 0b00000001) //D8 on output board
#define DOU25_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111110)	//D8 on output board
#define DOU26_ON	(digital_output_slaves[0].digital_pins_state |= 0b00000010)	//D9 on output board
#define DOU26_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111101)	//D9 on output board
#define DOU27_ON	(digital_output_slaves[0].digital_pins_state |= 0b00000100)	//D10 on output board
#define DOU27_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11111011)	//D10 on output board
#define DOU28_ON	(digital_output_slaves[0].digital_pins_state |= 0b00001000)	//D11 on output board
#define DOU28_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11110111)	//D11 on output board
#define DOU29_ON	(digital_output_slaves[0].digital_pins_state |= 0b00010000)	//D12 on output board
#define DOU29_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11101111)	//D12 on output board
#define DOU30_ON	(digital_output_slaves[0].digital_pins_state |= 0b00100000)	//D13 on output board
#define DOU30_OFF	(digital_output_slaves[0].digital_pins_state &= 0b11011111)	//D13 on output board
#define DOU31_ON	(digital_output_slaves[0].digital_pins_state |= 0b01000000)	//D14 on output board
#define DOU31_OFF	(digital_output_slaves[0].digital_pins_state &= 0b10111111)	//D14 on output board
#define DOU32_ON	(digital_output_slaves[0].digital_pins_state |= 0b10000000)	//D15 on output board
#define DOU32_OFF	(digital_output_slaves[0].digital_pins_state &= 0b01111111)	//D15 on output board


/* Digital output pins value */
#define DOU1 	(dout_state & 0b00000001)  //D8 status on output board
#define DOU2 	(dout_state & 0b00000010)  //D9 status on output board
#define DOU3 	(dout_state & 0b00000100)  //D10 status on output board
#define DOU4 	(dout_state & 0b00001000)  //D11 status on output board
#define DOU5 	(dout_state & 0b00010000)  //D12 status on output board
#define DOU6 	(dout_state & 0b00100000)  //D13 status on output board
#define DOU7 	(dout_state & 0b01000000)  //D14 status on output board
#define DOU8	(dout_state & 0b10000000)  //D15 status on output board

/* ############################################## *
 * ##### GET ANALOG INPUT SLAVE'S PINSTATES ##### *
 * ############################################## */

// Analog input slave 1
#define AIN1		analog_input_slaves[0].analoge_pins_state[0] // A0 status on input board
#define AIN2		analog_input_slaves[0].analoge_pins_state[1] // A1 status on input board
#define AIN3		analog_input_slaves[0].analoge_pins_state[2] // A2 status on input board
#define AIN4		analog_input_slaves[0].analoge_pins_state[3] // A3 status on input board
#define AIN5		analog_input_slaves[0].analoge_pins_state[4] // A4 status on input board
#define AIN6		analog_input_slaves[0].analoge_pins_state[5] // A5 status on input board

// Analog input slave 2
#define AIN7		analog_input_slaves[1].analoge_pins_state[0] // A0 status on input board
#define AIN8		analog_input_slaves[1].analoge_pins_state[1] // A1 status on input board
#define AIN9		analog_input_slaves[1].analoge_pins_state[2] // A2 status on input board
#define AIN10		analog_input_slaves[1].analoge_pins_state[3] // A3 status on input board
#define AIN11		analog_input_slaves[1].analoge_pins_state[4] // A4 status on input board
#define AIN12		analog_input_slaves[1].analoge_pins_state[5] // A5 status on input board

// Analog input slave 3
#define AIN13		analog_input_slaves[2].analoge_pins_state[0] // A0 status on input board
#define AIN14		analog_input_slaves[2].analoge_pins_state[1] // A1 status on input board
#define AIN15		analog_input_slaves[2].analoge_pins_state[2] // A2 status on input board
#define AIN16		analog_input_slaves[2].analoge_pins_state[3] // A3 status on input board
#define AIN17		analog_input_slaves[2].analoge_pins_state[4] // A4 status on input board
#define AIN18		analog_input_slaves[2].analoge_pins_state[5] // A5 status on input board

// Analog input slave 4
#define AIN19		analog_input_slaves[3].analoge_pins_state[0] // A0 status on input board
#define AIN20		analog_input_slaves[3].analoge_pins_state[1] // A1 status on input board
#define AIN21		analog_input_slaves[3].analoge_pins_state[2] // A2 status on input board
#define AIN22		analog_input_slaves[3].analoge_pins_state[3] // A3 status on input board
#define AIN23		analog_input_slaves[3].analoge_pins_state[4] // A4 status on input board
#define AIN24		analog_input_slaves[3].analoge_pins_state[5] // A5 status on input board

/* ############################################## *
 * ##### GET ANALOG OUTPUT SLAVE'S PINSTATES #### *
 * ############################################## */

// Analog output slave 1
#define AOU1		analog_output_slaves[0].analoge_pins_state[0]
#define AOU2		analog_output_slaves[0].analoge_pins_state[1]
#define AOU3		analog_output_slaves[0].analoge_pins_state[2]
#define AOU4		analog_output_slaves[0].analoge_pins_state[3]
#define AOU5		analog_output_slaves[0].analoge_pins_state[4]
#define AOU6		analog_output_slaves[0].analoge_pins_state[5]

// Analog output slave 2
#define AOU7		analog_output_slaves[1].analoge_pins_state[0]
#define AOU8		analog_output_slaves[1].analoge_pins_state[1]
#define AOU9		analog_output_slaves[1].analoge_pins_state[2]
#define AOU10		analog_output_slaves[1].analoge_pins_state[3]
#define AOU11		analog_output_slaves[1].analoge_pins_state[4]
#define AOU12		analog_output_slaves[1].analoge_pins_state[5]

// Analog output slave 3
#define AOU13		analog_output_slaves[2].analoge_pins_state[0]
#define AOU14		analog_output_slaves[2].analoge_pins_state[1]
#define AOU15		analog_output_slaves[2].analoge_pins_state[2]
#define AOU16		analog_output_slaves[2].analoge_pins_state[3]
#define AOU17		analog_output_slaves[2].analoge_pins_state[4]
#define AOU18		analog_output_slaves[2].analoge_pins_state[5]

// Analog output slave 4
#define AOU19		analog_output_slaves[3].analoge_pins_state[0]
#define AOU20		analog_output_slaves[3].analoge_pins_state[1]
#define AOU21		analog_output_slaves[3].analoge_pins_state[2]
#define AOU22		analog_output_slaves[3].analoge_pins_state[3]
#define AOU23		analog_output_slaves[3].analoge_pins_state[4]
#define AOU24		analog_output_slaves[3].analoge_pins_state[5]

/* Command list with lexicode 3distance*/

/* Exported types ------------------------------------------------------------*/

/* Exported variables ------------------------------------------------------- */

// This data holds the actual pin states

/* Exported functions ------------------------------------------------------- */


#endif /* __PLC_USER_CODE_H*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
