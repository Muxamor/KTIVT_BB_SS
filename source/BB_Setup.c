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
#include "../include/BB_Setup.h"
#include "../include/GPIO_SS.h"

int channel_fds[GPIO_MAX];

int Setup_GPIO_BB (void){
	int i;
	int size_mass;

	 for (i=0; i< GPIO_MAX ;i++){
	  	gpio_export(gpio_channel_indexes[i]);
	 }
	 printf("GPIO  export - SUCCESS!\n");
	 
	int gpio_direction[GPIO_MAX] = {
		OUTPUT_PIN, OUTPUT_PIN, OUTPUT_PIN, OUTPUT_PIN, OUTPUT_PIN, OUTPUT_PIN, OUTPUT_PIN, OUTPUT_PIN, OUTPUT_PIN,
		INPUT_PIN, INPUT_PIN, INPUT_PIN
	};
	for (i=0; i< GPIO_MAX ;i++){
		gpio_set_direction(gpio_channel_indexes[i],gpio_direction[i]);
	 }
	printf("GPIO set direction - SUCCESS!\n");

// Set edge to implementation interrupts for input GPIO.
	gpio_set_edge(gpio_channel_indexes[GPIO_SPI_INT_Ch1], "rising");
	gpio_set_edge(gpio_channel_indexes[GPIO_SPI_INT_Ch2], "rising");
	gpio_set_edge(gpio_channel_indexes[GPIO_SPI_INT_Ch3], "rising");
	printf("GPIO set edge - SUCCESS!\n");

//Open GPIO file to operate gpio.
	for (i=0; i< GPIO_MAX ;i++){
		if (gpio_direction[i] == OUTPUT_PIN ){
			channels_fds[i] = gpio_fd_open_R_W(GPIO_SPI_CS_Ch1);
		} else {
			channels_fds[i] = gpio_fd_open_R_O(GPIO_SPI_INT_Ch1);
		}
	 }
	printf("Get FD - SUCCESS!\n");

// Set default value for output GPIO.
	// also do that in the loop
	gpio_set_value(GPIO_SPI_CS_Ch1, HIGHT , fd_GPIO_SPI_CS_Ch1 );
	gpio_set_value(GPIO_SPI_CS_Ch2, HIGHT , fd_GPIO_SPI_CS_Ch2 );
	gpio_set_value(GPIO_SPI_CS_Ch3, HIGHT , fd_GPIO_SPI_CS_Ch3 );
	gpio_set_value(GPIO_SPI_CS_Disp, HIGHT , fd_GPIO_SPI_CS_Disp );
	gpio_set_value(GPIO_SPI_CS_Col, HIGHT , fd_GPIO_SPI_CS_Col );
	gpio_set_value(GPIO_Sync_Ch1_Ch2_Ch3, HIGHT , fd_GPIO_Sync_Ch1_Ch2_Ch3 ); // Проверить возможно  нужно по умолчанию ставить в ноль!!!!
	gpio_set_value(GPIO_SPI_Reset_Ch1, HIGHT , fd_GPIO_SPI_Reset_Ch1 );
	gpio_set_value(GPIO_SPI_Reset_Ch2, HIGHT , fd_GPIO_SPI_Reset_Ch2 );
	gpio_set_value(GPIO_SPI_Reset_Ch3, HIGHT , fd_GPIO_SPI_Reset_Ch3 );



   /*
 	gpio_fd_close(fd_GPIO_SPI_CS_Ch1);
	gpio_fd_close(fd_GPIO_SPI_CS_Ch2);
	gpio_fd_close(fd_GPIO_SPI_CS_Ch3);
	gpio_fd_close(fd_GPIO_SPI_INT_Ch1);
	gpio_fd_close(fd_GPIO_SPI_INT_Ch2);
	gpio_fd_close(fd_GPIO_SPI_INT_Ch3);
	gpio_fd_close(fd_GPIO_SPI_CS_Disp);
	gpio_fd_close(fd_GPIO_SPI_Reset_Ch1);
	gpio_fd_close(fd_GPIO_SPI_Reset_Ch2);
	gpio_fd_close(fd_GPIO_SPI_Reset_Ch3);
	gpio_fd_close(fd_GPIO_SPI_CS_Col);
	*/


}
