/*

 *
 * Version: 1.0
 * Date:	18.10.2013
 *
 * Copyright (c) 2013, jkuhlm - All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * In Eclipse add Include path
 *     C:\gcc-linaro\arm-linux-gnueabihf\libc\usr\include
 *
 */

//#include "../include/main.h"
#include <stdio.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include "../include/main.h"
#include "../include/GPIO_SS.h"
#include "../include/SPI_SS.h"
#include "../include/BB_Setup.h"

//#include <stdint.h>
//#include <unistd.h>
//#include <getopt.h>
//#include <sys/ioctl.h>
//#include <linux/types.h>
//#include <string.h>
//#include <errno.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <poll.h>

/*
 *
 * static void pabort(const char *s)
{
	perror(s);
	abort();
}

static void transfer(int fd){
	int ret;

		uint16_t tx1[] = {
				0x0000,0x0000
		};

		uint16_t rx1[ARRAY_SIZE(tx1)] = {0, };

		struct spi_ioc_transfer tr = {
			.tx_buf = (unsigned long)tx1,
			.rx_buf = (unsigned long)rx1,
			.len = 4,//ARRAY_SIZE(tx),
			.delay_usecs = delay,
			.speed_hz = speed,
			.bits_per_word = bits,
		};

		ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
		if (ret < 1)
			pabort("can't send spi message");

		for (ret = 0; ret < ARRAY_SIZE(tx1); ret++) {
			if (!(ret % 14))
				puts("");
			printf("%.4X ", rx1[ret]);
		}
*/

/*
		uint16_t tx2[] = {
						0x0000
				};

				uint16_t rx2[ARRAY_SIZE(tx2)] = {0, };


						tr.tx_buf = (unsigned long)tx2,
						tr.rx_buf = (unsigned long)rx2,
						tr.len = 4,//ARRAY_SIZE(tx),
						tr.delay_usecs = delay,
						tr.speed_hz = speed,
						tr.bits_per_word = bits,


				ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
				if (ret < 1)
					pabort("can't send spi message");

				for (ret = 0; ret < ARRAY_SIZE(tx2); ret++) {
					if (!(ret % 14))
						puts("");
					printf("%.2X ", rx2[ret]);
				}
*/
		//puts("");
//}


  
/****************************************************************
 * Our main function
 ****************************************************************/
 int main(void)
{ 
	/*Default_Setup_GPIO_BB();

	fd_SPI_BB = spi_device_open("/dev/spidev1.0");
	set_spi_settings(fd_SPI_BB, SPI_MODE_1, 16 , 16000000);
	uint16_t tx_buf[] = {0x0102,0x0304};
	uint16_t rx_buf[ARRAY_SIZE(tx_buf)] = {};
	spi_transfer (fd_SPI_BB, GPIO_SPI_CS_Ch1 ,tx_buf, rx_buf, sizeof(tx_buf), 0);
	int i;
	for (i = 0; i < ARRAY_SIZE(tx_buf); i++){
		printf("0x%.4X ", rx_buf[i]);
	}
	*/
	gpio_export(7);
	gpio_set_direction(7, INPUT_PIN);
	gpio_set_edge(7, "rising");
	int fd_GPIO_SPI_INT_Ch1;
	fd_GPIO_SPI_INT_Ch1 = gpio_fd_open_R_O(7);

	printf("sleep 10 second\n");
	sleep(10);
	int check;

	//gpio_input_pin_numbers[GPIO_SPI_INT_Ch1]
	check= gpio_get_value_interrupt(7, 0);

	if(check==-1){
				printf("interrupt no happen\n");
			}else{

				printf("interrupt happen %d\n", check);
			}
	printf("sleep 10 second\n");
	sleep(10);
	check= gpio_get_value_interrupt(7,0);

		if(check==-1){
			printf("interrupt no happen \n");
		}else{

			printf("interrupt happen %d\n", check);
		}
	printf("sleep 10 second\n");
	sleep(10);
			check= gpio_get_value_interrupt(7, 100);

				if(check==-1){
					printf("interrupt no happen \n");
				}else{

					printf("interrupt happen %d\n", check);
				}

	//close(fd_SPI_BB);
	//gpio_fd_close(fd_GPIO_pin_output[GPIO_SPI_CS_Ch1]);


	 return EXIT_SUCCESS;
}


/*
 *
	gpio_export(GPIO_SPI_CS_Ch1);
	gpio_export(GPIO_SPI_CS_Ch2);
	gpio_export(GPIO_SPI_CS_Ch3);
	gpio_export(GPIO_SPI_INT_Ch1);
	gpio_export(GPIO_SPI_INT_Ch2);
	gpio_export(GPIO_SPI_INT_Ch3);
	gpio_export(GPIO_SPI_CS_Disp);
	gpio_export(GPIO_SPI_Reset_Ch1);
	gpio_export(GPIO_SPI_Reset_Ch2);
	gpio_export(GPIO_SPI_Reset_Ch3);
	gpio_export(GPIO_SPI_CS_Col);
	printf("gpio_export SUCCESS!\n");

	gpio_set_direction(GPIO_SPI_CS_Ch1, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_CS_Ch2, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_CS_Ch3, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_INT_Ch1, INPUT_PIN);
	gpio_set_direction(GPIO_SPI_INT_Ch2, INPUT_PIN);
	gpio_set_direction(GPIO_SPI_INT_Ch3, INPUT_PIN);
	gpio_set_direction(GPIO_SPI_CS_Disp, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_Reset_Ch1, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_Reset_Ch2, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_Reset_Ch3, OUTPUT_PIN);
	gpio_set_direction(GPIO_SPI_CS_Col, OUTPUT_PIN);
	printf("gpio_set_direction SUCCESS!\n");

	gpio_set_edge(GPIO_SPI_INT_Ch1, "rising");
	gpio_set_edge(GPIO_SPI_INT_Ch2, "rising");
	gpio_set_edge(GPIO_SPI_INT_Ch3, "rising");
	printf("gpio_set_edge SUCCESS!\n");


	int fd_GPIO_SPI_CS_Ch1;
	int fd_GPIO_SPI_CS_Ch2;
	int fd_GPIO_SPI_CS_Ch3;
	int fd_GPIO_SPI_INT_Ch1;
	int fd_GPIO_SPI_INT_Ch2;
	int fd_GPIO_SPI_INT_Ch3;
	int fd_GPIO_SPI_CS_Disp;
	int fd_GPIO_SPI_Reset_Ch1;
	int fd_GPIO_SPI_Reset_Ch2;
	int fd_GPIO_SPI_Reset_Ch3;
	int fd_GPIO_SPI_CS_Col;

	fd_GPIO_SPI_CS_Ch1 = gpio_fd_open_R_W(GPIO_SPI_CS_Ch1);
	fd_GPIO_SPI_CS_Ch2 = gpio_fd_open_R_W(GPIO_SPI_CS_Ch2);
	fd_GPIO_SPI_CS_Ch3 = gpio_fd_open_R_W(GPIO_SPI_CS_Ch3);

	fd_GPIO_SPI_INT_Ch1 = gpio_fd_open_R_O(GPIO_SPI_INT_Ch1);
	fd_GPIO_SPI_INT_Ch2 = gpio_fd_open_R_O(GPIO_SPI_INT_Ch2);
	fd_GPIO_SPI_INT_Ch3 = gpio_fd_open_R_O(GPIO_SPI_INT_Ch3);

	fd_GPIO_SPI_CS_Disp = gpio_fd_open_R_W(GPIO_SPI_CS_Disp);
	fd_GPIO_SPI_Reset_Ch1 = gpio_fd_open_R_W(GPIO_SPI_Reset_Ch1);
	fd_GPIO_SPI_Reset_Ch2 = gpio_fd_open_R_W(GPIO_SPI_Reset_Ch2);
	fd_GPIO_SPI_Reset_Ch3 = gpio_fd_open_R_W(GPIO_SPI_Reset_Ch3);
	fd_GPIO_SPI_CS_Col = gpio_fd_open_R_W(GPIO_SPI_CS_Col);

	gpio_set_value(GPIO_SPI_CS_Ch1, HIGHT , fd_GPIO_SPI_CS_Ch1 );
	gpio_set_value(GPIO_SPI_CS_Ch2, HIGHT , fd_GPIO_SPI_CS_Ch2 );
	gpio_set_value(GPIO_SPI_CS_Ch3, HIGHT , fd_GPIO_SPI_CS_Ch3 );
	gpio_set_value(GPIO_SPI_CS_Disp, HIGHT , fd_GPIO_SPI_CS_Disp );
	gpio_set_value(GPIO_SPI_Reset_Ch1, HIGHT , fd_GPIO_SPI_Reset_Ch1 );
	gpio_set_value(GPIO_SPI_Reset_Ch2, HIGHT , fd_GPIO_SPI_Reset_Ch2 );
	gpio_set_value(GPIO_SPI_Reset_Ch3, HIGHT , fd_GPIO_SPI_Reset_Ch3 );
	gpio_set_value(GPIO_SPI_CS_Col, HIGHT , fd_GPIO_SPI_CS_Col );

	//gpio_fd_close(fd_GPIO_SPI_CS_Ch1);
	gpio_fd_close(fd_GPIO_SPI_CS_Ch2);
	gpio_fd_close(fd_GPIO_SPI_CS_Ch3);
	//gpio_fd_close(fd_GPIO_SPI_INT_Ch1);
	gpio_fd_close(fd_GPIO_SPI_INT_Ch2);
	gpio_fd_close(fd_GPIO_SPI_INT_Ch3);
	gpio_fd_close(fd_GPIO_SPI_CS_Disp);
	gpio_fd_close(fd_GPIO_SPI_Reset_Ch1);
	gpio_fd_close(fd_GPIO_SPI_Reset_Ch2);
	gpio_fd_close(fd_GPIO_SPI_Reset_Ch3);
	gpio_fd_close(fd_GPIO_SPI_CS_Col);

	printf("gpio_set_value SUCCESS!\n");
 */

