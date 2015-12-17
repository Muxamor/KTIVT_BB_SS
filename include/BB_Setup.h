/*
 * BB_Setup.h
*
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */

#ifndef INCLUDE_BB_SETUP_H_
#define INCLUDE_BB_SETUP_H_

/****************************************************
*                 Constants                         *
****************************************************/


//list for output pins for seismic station KTIVT
typedef enum {
	GPIO_SPI_CS_Ch1,
	GPIO_SPI_CS_Ch2,
	GPIO_SPI_CS_Ch3,
	GPIO_SPI_CS_Col,
	GPIO_SPI_CS_Disp,
	GPIO_Sync_Ch1_Ch2_Ch3,
	GPIO_SPI_Reset_Ch1,
	GPIO_SPI_Reset_Ch2,
	GPIO_SPI_Reset_Ch3,
	GPIO_out_MAX
} gpio_name_output_pin;

// Area  with number of pins depends of  gpio_name_output_pin list
static unsigned int gpio_output_pin_numbers[GPIO_out_MAX] = { 115, 117, 48, 49, 86, 89, 65, 70, 71 };


//list for input pins for seismic station KTIVT
typedef enum {
	GPIO_SPI_INT_Ch1,
	GPIO_SPI_INT_Ch2,
	GPIO_SPI_INT_Ch3,
	GPIO_in_MAX
} gpio_name_input_pin;

// Area  with number of pins depends of  gpio_name_input_pin list
static unsigned int gpio_input_pin_numbers[GPIO_in_MAX] = { 7, 26, 27 };


// Area for file descriptors of GPIO
unsigned int fd_GPIO_pin_output[ GPIO_out_MAX ];
unsigned int fd_GPIO_pin_input[ GPIO_in_MAX ];



/***************************************************
*                Setup BB function                 *
***************************************************/
int Default_Setup_GPIO_BB (void);
int enable_analog_channel (unsigned int gpio);
int disable_analog_channel (unsigned int gpio);
int reset_analog_channel (unsigned int gpio);



#endif /* INCLUDE_BB_SETUP_H_ */
