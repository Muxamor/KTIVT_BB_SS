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


#include <stdint.h>
//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
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

int spi_transfer_command_analog_ch ( int fd, uint16_t SPI_channel, uint16_t SPI_channel_int, uint16_t tx_buffer[], uint16_t rx_buffer[], uint16_t size, uint16_t delay_SPI ) {

	int ret=0;

	ret = spi_transfer (fd, SPI_channel, tx_buffer, rx_buffer, size, delay_SPI);

    if( ret == -1 ){
       	return -1;
    }

    /// сделать тайм аут на ожидание ответа посылать 3 раза если ответ нету и истек тайм аут
    while((gpio_get_value_interrupt(fd_GPIO_pin_input[SPI_channel_int],0)) != 1);

    tx_buffer[0] = 0x0000;
    tx_buffer[1] = 0x0000;

    ret = spi_transfer (fd, SPI_channel ,tx_buffer, rx_buffer, size, 0);

    if( ret == -1 ){
    	return -1;
    }

    return 0;

}

int spi_read_data_ADC24 (int fd, uint16_t SPI_channel, uint16_t SPI_channel_int, uint16_t tx_buffer[], uint16_t rx_buffer[], uint16_t size, uint16_t delay_SPI){

	int ret;
	uint32_t rest_size = 0;
	uint32_t current_size = 0;
	uint16_t ptr=0;


	uint16_t tx_buf_tmp[2] = {0x2400,0x0000};
	uint16_t rx_buf_tmp[2] = {};
	//spi_transfer (fd_SPI_BB, SPI_channel ,tx_buf_tmp, rx_buf_tmp, sizeof(tx_buf_tmp), 0);


	gpio_set_value(fd_GPIO_pin_output[SPI_channel] , LOW); //Enable chip select

	SPI_trunsfer_struct.tx_buf=(unsigned long)tx_buf_tmp;
	SPI_trunsfer_struct.rx_buf=(unsigned long)rx_buf_tmp;
	SPI_trunsfer_struct.len = 4;  // Need write quantity of bits in a parcel.
	SPI_trunsfer_struct.delay_usecs = delay_SPI;

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &SPI_trunsfer_struct);

	if (ret < 1){
		return -1;
	}

	while((gpio_get_value_interrupt(fd_GPIO_pin_input[SPI_channel_int],0)) != 1);///Дописать ожидание и выдавать ошибку если прирывание не дождался

	if(SIZE_HEAD_PACKAGE_ADC >= size ){
		return -1;
	}


	//Read head data package
	SPI_trunsfer_struct.tx_buf=(unsigned long)tx_buffer;
	SPI_trunsfer_struct.rx_buf=(unsigned long)rx_buffer;
	SPI_trunsfer_struct.len = SIZE_HEAD_PACKAGE_ADC;  // Size head of data package in byte
	SPI_trunsfer_struct.delay_usecs = delay_SPI;

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &SPI_trunsfer_struct);
	if (ret < 1){
			return -1;
	}

	rest_size = size - SIZE_HEAD_PACKAGE_ADC;
	ptr = SIZE_HEAD_PACKAGE_ADC / 2;

	do{

		if( rest_size >= 4096){ // 4096 byte is maximal size of SPI message
			current_size = 4096;
		}else{
			current_size = rest_size;
		}

		SPI_trunsfer_struct.tx_buf=(unsigned long)(&tx_buffer[ptr]);
		SPI_trunsfer_struct.rx_buf=(unsigned long)(&rx_buffer[ptr]);
		SPI_trunsfer_struct.len = current_size;  // Size head of data package in byte

		ret = ioctl(fd, SPI_IOC_MESSAGE(1), &SPI_trunsfer_struct);
		if (ret < 1){
			return -1;
		}
		ptr = ptr + (current_size / 2);
		rest_size = rest_size - current_size;

	}while(rest_size != 0);

	gpio_set_value(fd_GPIO_pin_output[SPI_channel] , HIGHT);

	return 0;
}

int spi_device_open(char *device){

	int fd;

	fd = open(device, O_RDWR);
	if (fd < 0){
		perror("can't open device");
		abort();
	}

	return fd;
}


int set_spi_settings(int fd, uint8_t mode, uint8_t msb_lsb_mode, uint8_t bits_in_word, uint32_t speed_SPI){
	int ret = 0;

// Set SPI mode
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1){
		perror("can't set spi mode");
		abort();
	}

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1){
		perror("can't get spi mode");
		abort();
	}

//Set LSB or MSB to set MSB write 0  for LSBwrite 1

	ret = ioctl(fd, SPI_IOC_WR_LSB_FIRST, &msb_lsb_mode);
	 if (ret == -1){
		perror("can't set LSB/MSB_WR");
			abort();
		}

	ret = ioctl(fd, SPI_IOC_RD_LSB_FIRST, &msb_lsb_mode);
		if (ret == -1){
			perror("can't get LSB/MSB_RD");
			abort();
		}

// Set bits per word for SPI
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_in_word);
	if (ret == -1){
		perror("can't set bits per word");
		abort();
	}

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits_in_word);
	if (ret == -1){
		perror("can't get bits per word");
		abort();
	}

	SPI_trunsfer_struct.bits_per_word=bits_in_word;//Global struct

// Set max speed for SPI
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_SPI);
	if (ret == -1){
		perror("can't set max speed hz");
		abort();
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed_SPI);
	if (ret == -1){
		perror("can't get max speed hz");
		abort();
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


