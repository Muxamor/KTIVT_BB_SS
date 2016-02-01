/*
 * M41T64_Clock.c
 *
 * Version: 0.0
 * Date:	16.1.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int Defult_setup_M41T64(uint8_t i2c_port, int addr_slave ){

	unsigned int fd_i2c;
	char buf[2];
	char buf_path_i2c_port[64] = {0};

	snprintf(buf_path_i2c_port,sizeof(buf_path_i2c_port), "/dev/i2c-%d",i2c_port);

	//Open i2c bus 2 on the beaglebone black
	fd_i2c = open(buf_path_i2c_port,O_RDWR);
	if (fd_i2c < 0){
		printf("Failed to open the bus i2c-1 .");
		// сделать запись в лог
		return -1;
	}

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}


	//Disable SQW output
	buf[0] = 0x0A; // Address register in M41T64
	buf[1] = 0b00100001;

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x0A.\n");
		// сделать запись в лог
		return -1;
	}

	//SET ST=0
	buf[0] = 0x01; // Address register in M41T64
	buf[1] = 0b00000000;

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x0A.\n");
		// сделать запись в лог
		return -1;
	}

	//Set value of frequency for SQW output at M41T64
	buf[0] = 0x04; // Address register in M41T64
	buf[1] = 0b00000001;

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x0A.\n");
		// сделать запись в лог
		return -1;
	}

	close(fd_i2c);
	printf("Setup default M41T64 - SUCCESS!\n");
	return 0;

}

int M41T64_set_SQW_clock (uint8_t i2c_port, uint16_t clock_SQW, int addr_slave ){

	int fd_i2c;
	char buf[2];
	char buf_path_i2c_port[64];

	snprintf(buf_path_i2c_port,sizeof(buf_path_i2c_port), "/dev/i2c-%d",i2c_port);

	//Open i2c bus 2 on the beaglebone black
	fd_i2c = open(buf_path_i2c_port,O_RDWR);
	if (fd_i2c < 0){
		printf("Failed to open the bus i2c-1 .");
		// сделать запись в лог
		return -1;
	}

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	//Write to register of M41T64

	//Disable SQW output
	buf[0] = 0x0A; // Address register in M41T64
	buf[1] = 0b00100001;

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x0A.\n");
		// сделать запись в лог
		return -1;
	}

	//Set value of frequency for SQW output at M41T64
		buf[0] = 0x04; // Address register in M41T64

		switch(clock_SQW){
			case 64:  // 64 need x8 for ADC  64x8=512
				buf[1] = 0b01100001; //512Hz
				break;
			case 128:// 128 need x8 for ADC  128x8=1024
				buf[1] = 0b01010001; //1024Hz
				break;
			case 256:// 256 need x8 for ADC  256x8=2048
				buf[1] = 0b01000001; //2048Hz
				break;
			case 512:// 512 need x8 for ADC  512x8=4096
				buf[1] = 0b00110001; //4096Hz
				break;
			case 1024:// 1024 need x8 for ADC  1024x8=8192
				buf[1] = 0b00100001; //8192Hz
				break;
			case 4096:
				buf[1] = 0b00010001;
 				break;
			default:
				buf[1] = 0b01100001; //512Hz
				printf("Not correct value of Fd. Set 64Hz\n");
				break;
		}

		if( write(fd_i2c,buf,2) != 2) {
			printf("Failed to write to the i2c bus register 0x0A.\n");
			// сделать запись в лог
			return -1;
		}

		//Enable SQW output
			buf[0] = 0x0A; // Address register in M41T64
			buf[1] = 0b01100001;

			if( write(fd_i2c,buf,2) != 2) {
				printf("Failed to write to the i2c bus register 0x0A.\n");
				// сделать запись в лог
				return -1;
			}

		close(fd_i2c);

		return 0;

}
