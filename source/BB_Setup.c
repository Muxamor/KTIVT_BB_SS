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
#include <stdio.h>



int Default_Setup_GPIO_BB (void){


	/*int i;
	int size_mass;
	unsigned int GPIO_PIN_NUMBER [] = {GPIO_SPI_CS_Ch1,GPIO_SPI_CS_Ch2,GPIO_SPI_CS_Ch3,GPIO_SPI_CS_Disp,GPIO_SPI_CS_Col,
			 	 	 	 	 	 	 	GPIO_Sync_Ch1_Ch2_Ch3,
										GPIO_SPI_INT_Ch1, GPIO_SPI_INT_Ch2, GPIO_SPI_INT_Ch3,
										GPIO_SPI_Reset_Ch1, GPIO_SPI_Reset_Ch2, GPIO_SPI_Reset_Ch3};

	 size_mass = (sizeof(GPIO_PIN_NUMBER) / sizeof(GPIO_PIN_NUMBER[0]));

	 for (i=0; i< size_mass ;i++){

	  gpio_export(GPIO_PIN_NUMBER[i]);
	 }*/

//Export GPIO to the Linux sys.
	gpio_export(GPIO_SPI_CS_Ch1);
	gpio_export(GPIO_SPI_CS_Ch2);
	gpio_export(GPIO_SPI_CS_Ch3);
	gpio_export(GPIO_SPI_CS_Disp);
	gpio_export(GPIO_SPI_CS_Col);
	gpio_export(GPIO_Sync_Ch1_Ch2_Ch3);
	gpio_export(GPIO_SPI_Reset_Ch1);
	gpio_export(GPIO_SPI_Reset_Ch2);
	gpio_export(GPIO_SPI_Reset_Ch3);
	gpio_export(GPIO_SPI_INT_Ch1);
	gpio_export(GPIO_SPI_INT_Ch2);
	gpio_export(GPIO_SPI_INT_Ch3);
	printf("GPIO  export - SUCCESS!\n");


//Set direction for GPIO to the Linux sys.
	gpio_set_direction(GPIO_SPI_CS_Ch1, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_CS_Ch2, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_CS_Ch3, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_CS_Disp, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_CS_Col, OUTPUT_PIN);
	gpio_set_direction(GPIO_Sync_Ch1_Ch2_Ch3, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_Reset_Ch1, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_Reset_Ch2, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_Reset_Ch3, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_INT_Ch1, INPUT_PIN);
	gpio_set_direction(GPIO_SPI_INT_Ch2, INPUT_PIN);
	gpio_set_direction(GPIO_SPI_INT_Ch3, INPUT_PIN);
	printf("GPIO set direction - SUCCESS!\n");

// Set edge to implementation interrupts for input GPIO.
	gpio_set_edge(GPIO_SPI_INT_Ch1, "rising");
	gpio_set_edge(GPIO_SPI_INT_Ch2, "rising");
	gpio_set_edge(GPIO_SPI_INT_Ch3, "rising");
	printf("GPIO set edge - SUCCESS!\n");

//Open GPIO file to operate gpio.


	fd_GPIO_SPI_CS_Ch1 = gpio_fd_open_R_W(GPIO_SPI_CS_Ch1);
	fd_GPIO_SPI_CS_Ch2 = gpio_fd_open_R_W(GPIO_SPI_CS_Ch2);
	fd_GPIO_SPI_CS_Ch3 = gpio_fd_open_R_W(GPIO_SPI_CS_Ch3);
	fd_GPIO_SPI_CS_Disp = gpio_fd_open_R_W(GPIO_SPI_CS_Disp);
	fd_GPIO_SPI_CS_Col = gpio_fd_open_R_W(GPIO_SPI_CS_Col);
	fd_GPIO_Sync_Ch1_Ch2_Ch3 = gpio_fd_open_R_W(GPIO_Sync_Ch1_Ch2_Ch3);
	fd_GPIO_SPI_Reset_Ch1 = gpio_fd_open_R_W(GPIO_SPI_Reset_Ch1);
	fd_GPIO_SPI_Reset_Ch2 = gpio_fd_open_R_W(GPIO_SPI_Reset_Ch2);
	fd_GPIO_SPI_Reset_Ch3 = gpio_fd_open_R_W(GPIO_SPI_Reset_Ch3);
	fd_GPIO_SPI_INT_Ch1 = gpio_fd_open_R_O(GPIO_SPI_INT_Ch1);
	fd_GPIO_SPI_INT_Ch2 = gpio_fd_open_R_O(GPIO_SPI_INT_Ch2);
	fd_GPIO_SPI_INT_Ch3 = gpio_fd_open_R_O(GPIO_SPI_INT_Ch3);
	printf("Get FD - SUCCESS!\n");

// Set default value for output GPIO.
	gpio_set_value(GPIO_SPI_CS_Ch1, HIGHT , fd_GPIO_SPI_CS_Ch1 );
	gpio_set_value(GPIO_SPI_CS_Ch2, HIGHT , fd_GPIO_SPI_CS_Ch2 );
	gpio_set_value(GPIO_SPI_CS_Ch3, HIGHT , fd_GPIO_SPI_CS_Ch3 );
	gpio_set_value(GPIO_SPI_CS_Disp, HIGHT , fd_GPIO_SPI_CS_Disp );
	gpio_set_value(GPIO_SPI_CS_Col, HIGHT , fd_GPIO_SPI_CS_Col );
	gpio_set_value(GPIO_Sync_Ch1_Ch2_Ch3, HIGHT , fd_GPIO_Sync_Ch1_Ch2_Ch3 ); // Проверить возможно  нужно по умолчанию ставить в ноль!!!!
	gpio_set_value(GPIO_SPI_Reset_Ch1, HIGHT , fd_GPIO_SPI_Reset_Ch1 );
	gpio_set_value(GPIO_SPI_Reset_Ch2, HIGHT , fd_GPIO_SPI_Reset_Ch2 );
	gpio_set_value(GPIO_SPI_Reset_Ch3, HIGHT , fd_GPIO_SPI_Reset_Ch3 );
	printf("Set default value gpio - SUCCESS!\n");


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
	return 0;


}
