/*
 * BB_Sutup.c
  *
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */

#include <stdio.h>
#include <unistd.h>
#include "../include/GPIO_SS.h"
#include "../include/BB_Setup.h"


/*********************************************
 * Default GPIO settings for BB
 ********************************************/

int Default_Setup_GPIO_BB (void){
	int i;
	int ret=0;

//Export GPIO to the Linux sys.
	for (i=0; i< GPIO_out_MAX; i++){
		ret=gpio_export(gpio_output_pin_numbers[i]);
		if (ret==-1){
			return -1;
		}

	}
	for (i=0; i< GPIO_in_MAX; i++){
		gpio_export(gpio_input_pin_numbers[i]);
		if (ret==-1){
			return -1;
		}
	}
	printf("GPIO  export - SUCCESS!\n");


//Set direction for GPIO to the Linux sys.
	for (i=0; i< GPIO_out_MAX; i++){
		gpio_set_direction(gpio_output_pin_numbers[i],OUTPUT_PIN);
		if (ret==-1){
			return -1;
		}
	}

	for (i=0; i< GPIO_in_MAX; i++){
		gpio_set_direction(gpio_input_pin_numbers[i],INPUT_PIN);
		if (ret==-1){
			return -1;
		}
	}
	printf("GPIO set direction - SUCCESS!\n");


/// Set edge to implementation interrupts for input GPIO
	for (i=0; i< GPIO_in_MAX; i++){
		gpio_set_edge(gpio_input_pin_numbers[i],"rising");
		if (ret==-1){
			return -1;
		}
	}
	printf("GPIO set edge - SUCCESS!\n");


//Open GPIO file to operate gpio.
	for (i=0; i< GPIO_out_MAX; i++){
		fd_GPIO_pin_output[i]=gpio_fd_open_R_W(gpio_output_pin_numbers[i]);
		if (ret==-1){
			return -1;
		}
	}

	for (i=0; i< GPIO_in_MAX; i++){
		fd_GPIO_pin_input[i]=gpio_fd_open_R_O(gpio_input_pin_numbers[i]);
		if (ret==-1){
			return -1;
		}
	}
	printf("Get FD - SUCCESS!\n");


// Set default value for output GPIO.
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Ch1] , HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Ch2] , HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Ch3] , HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Disp] , HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Col], HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3], HIGHT);// Проверить возможно  нужно по умолчанию ставить в ноль!!!!
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_Reset_Ch1], LOW);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_Reset_Ch2], LOW);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_Reset_Ch3], LOW);

	printf("Set default value gpio - SUCCESS!\n");


   /*
 	gpio_fd_close(fd_GPIO_SPI_CS_Ch1);

	*/
	return 0;
}

/*********************************************
 * Enable analog channel
 ********************************************/
int enable_analog_channel (unsigned int gpio){

		gpio_set_value(fd_GPIO_pin_output[gpio] , LOW);

	return 0;
}

/*********************************************
 * Disable analog channel
 ********************************************/
int disable_analog_channel (unsigned int gpio){

		gpio_set_value(fd_GPIO_pin_output[gpio] , HIGHT);

	return 0;
}

/*********************************************
 * Reset  analog channel
 ********************************************/
int reset_analog_channel (unsigned int gpio){

	gpio_set_value(fd_GPIO_pin_output[gpio] , LOW);
	usleep(5);
	gpio_set_value(fd_GPIO_pin_output[gpio] , HIGHT);

	return 0;
}


