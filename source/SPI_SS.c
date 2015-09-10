/*
 * SPI_SS.c
 *
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */


//#include <stdint.h>
//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
//#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "../include/SPI_SS.h"
#include "../include/GPIO_SS.h"
#include "../include/BB_Setup.h"

//#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
//#define ARRAY_SIZE_bite(a) (sizeof(a))

///static const char *device = "/dev/spidev1.0";
//static uint8_t mode=SPI_MODE_1;
//static uint8_t bits = 8;
//static uint32_t speed = 500000;
//static uint16_t delay = 0;

int spi_transfer (int fd, uint16_t SPI_channel, uint16_t tx_buffer[], uint16_t rx_buffer[], uint16_t size, uint16_t delay_SPI){

	int ret;

	SPI_trunsfer_struct.tx_buf=(unsigned long)tx_buffer;
	SPI_trunsfer_struct.rx_buf=(unsigned long)rx_buffer;
	SPI_trunsfer_struct.len = size;  // Need write quantity of bits in a parcel.
	SPI_trunsfer_struct.delay_usecs = delay_SPI;

	gpio_set_value(fd_GPIO_pin_output[SPI_channel] , LOW);

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &SPI_trunsfer_struct);

	gpio_set_value(fd_GPIO_pin_output[SPI_channel] , HIGHT);

	if (ret < 1){
		return -1;
	}

	return 0;
}


int spi_device_open(char *device){

	int fd;

	fd = open(device, O_RDWR);
	if (fd < 0){
		perror("can't open device");
		return EXIT_FAILURE;
	}

	return fd;
}


int set_spi_settings(int fd, uint8_t mode, uint8_t bits_in_word, uint32_t speed_SPI){
	int ret = 0;

// Set SPI mode
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1){
		perror("can't set spi mode");
	    return EXIT_FAILURE;
	}

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1){
		perror("can't get spi mode");
		return EXIT_FAILURE;
	}


// Set bits per word for SPI
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_in_word);
	if (ret == -1){
		perror("can't set bits per word");
		return EXIT_FAILURE;
	}

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits_in_word);
	if (ret == -1){
		perror("can't get bits per word");
		return EXIT_FAILURE;
	}

	SPI_trunsfer_struct.bits_per_word=bits_in_word;//Global struct

// Set max speed for SPI
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_SPI);
	if (ret == -1){
		perror("can't set max speed hz");
		return EXIT_FAILURE;
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed_SPI);
	if (ret == -1){
		perror("can't get max speed hz");
		return EXIT_FAILURE;
	}

	SPI_trunsfer_struct.speed_hz = speed_SPI;//Global struct



// Print report of the settings SPI port.
	printf("Setup SPI - SUCCESS!\n");
	printf("Setting SPI:\n");
	printf("--spi mode: %d\n", mode);
	printf("--bits per word: %d\n", bits_in_word);
	printf("--max speed: %d Hz (%d KHz)\n", speed_SPI, speed_SPI/1000);

	return 0;
}


