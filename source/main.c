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

#include "../include/GPIO_SS.h"
#include "../include/BB_Setup.h"

#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>



#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev1.0";
static uint8_t mode=SPI_MODE_1;
static uint8_t bits = 16;
static uint32_t speed = 18000000;
static uint16_t delay = 0;


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
		puts("");
}


  
/****************************************************************
 * Our main function with a little more than "Hello world!" only
 ****************************************************************/
 int main(void)
{ 
	Default_Setup_GPIO_BB();

	printf("sleep 10 second\n");
	sleep(10);
	int check;
	check= gpio_get_value_interrupt(fd_GPIO_SPI_INT_Ch1, 0);

	if(check==-1){
				printf("interrupt no happen\n");
			}else{

				printf("interrupt happen %d\n", check);
			}
	printf("sleep 10 second\n");
	sleep(10);
	check= gpio_get_value_interrupt(fd_GPIO_SPI_INT_Ch1, 100);

		if(check==-1){
			printf("interrupt no happen \n");
		}else{

			printf("interrupt happen %d\n", check);
		}

	// ret;
	//int fd;

	//fd=spi_device_open("/dev/spidev1.0");
	//set_spi_settings(fd, SPI_MODE_1, 16 , 16000000, 0);
	//static void transfer(int fd)
	//{

	   int ret = 0;
		int fd;

		fd = open(device, O_RDWR);
		if (fd < 0)
			pabort("can't open device");

		/*
		 * spi mode
		 */
		ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
		if (ret == -1)
			pabort("can't set spi mode");

		ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
		if (ret == -1)
			pabort("can't get spi mode");

		/*
		 * bits per word
		 */
		ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
		if (ret == -1)
			pabort("can't set bits per word");

		ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
		if (ret == -1)
			pabort("can't get bits per word");

		/*
		 * max speed hz
		 */
		ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
		if (ret == -1)
			pabort("can't set max speed hz");

		ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
		if (ret == -1)
			pabort("can't get max speed hz");

		printf("spi mode: %d\n", mode);
		printf("bits per word: %d\n", bits);
		printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

		gpio_set_value(GPIO_SPI_CS_Ch1, LOW , fd_GPIO_SPI_CS_Ch1 );



		transfer(fd);

		gpio_set_value(GPIO_SPI_CS_Ch1, HIGHT , fd_GPIO_SPI_CS_Ch1 );

		close(fd);
		gpio_fd_close(fd_GPIO_SPI_CS_Ch1);


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

