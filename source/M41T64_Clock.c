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
#include "../include/main.h"


int i2c_device_open(char *device){

	int fd;

	fd = open(device, O_RDWR);
	if (fd < 0){
		perror("can't open i2c device");
		abort();
	}

	return fd;
}

int Defult_setup_M41T64(unsigned int fd_i2c, unsigned int addr_slave ){

//	unsigned int fd_i2c;
	char buf[2];
//	char buf_path_i2c_port[64] = {0};

//	snprintf(buf_path_i2c_port,sizeof(buf_path_i2c_port), "/dev/i2c-%d",i2c_port);

	//Open i2c bus 2 on the beaglebone black
//	fd_i2c = open(buf_path_i2c_port,O_RDWR);
//	if (fd_i2c < 0){
//		printf("Failed to open the bus i2c-1 .");
		// сделать запись в лог
//		return -1;
//	}

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
	buf[1] = 0b00010001; // 4096 need x8 for ADC  4096x8=32768

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

//	close(fd_i2c);
	printf("Setup default M41T64 - SUCCESS!\n");
	return 0;

}

int M41T64_set_SQW_clock (unsigned int fd_i2c, uint16_t clock_SQW, unsigned int addr_slave ){

	char buf[2];

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
			case 4096:// 4096 need x8 for ADC  4096x8=32768
				buf[1] = 0b00010001;
 				break;
			default:
				buf[1] = 0b00010001; // 4096 need x8 for ADC  4096x8=32768
				printf("Not correct value of Fd. Set 4096Hz\n");
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

//		close(fd_i2c);

		return 0;

}


int set_seconds_M41T64(unsigned int fd_i2c, uint8_t val_seconds, unsigned int addr_slave){

	uint8_t buf[2];

	if ( val_seconds >= 0x60){
		printf("Failed RTC no correct value of seconds.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of seconds
	buf[0] = 0x01; // Address register in M41T64
	buf[1] = 0b01111111 & val_seconds;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x01.\n");
		// сделать запись в лог
		return -1;
	}

	return 0;
}



int get_seconds_M41T64(unsigned int fd_i2c, unsigned int addr_slave){

	uint8_t buf[2];
	uint8_t ret;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of seconds
	buf[0] = 0x01; // Address register in M41T64

	if( write(fd_i2c,buf,1) != 1) {
		printf("Failed to write to read  the i2c bus register 0x01.\n");
		// сделать запись в лог
		return -1;
	}

    if( read (fd_i2c,buf,1) !=1){
    	printf("Failed to read to the i2c bus register 0x01.\n");
    			// сделать запись в лог
    	return -1;
    }

    ret = 0b01111111 & buf[0];

	return ret;
}


int set_minutes_M41T64(unsigned int fd_i2c, uint8_t val_minutes, unsigned int addr_slave){

	uint8_t buf[2];

	if ( val_minutes >= 0x60){
		printf("Failed RTC no correct value of minutes.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of minutes
	buf[0] = 0x02; // Address register in M41T64
	buf[1] = 0b01111111 & val_minutes;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x02.\n");
		// сделать запись в лог
		return -1;
	}

	return 0;
}

int get_minutes_M41T64(unsigned int fd_i2c, unsigned int addr_slave){

	uint8_t buf[2];
	uint8_t ret;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of minutes
	buf[0] = 0x02; // Address register in M41T64

	if( write(fd_i2c,buf,1) != 1) {
		printf("Failed to write to read  the i2c bus register 0x01.\n");
		// сделать запись в лог
		return -1;
	}

    if( read (fd_i2c,buf,1) !=1){
    	printf("Failed to read to the i2c bus register 0x01.\n");
    			// сделать запись в лог
    	return -1;
    }

    ret = 0b01111111 & buf[0];

    return ret;

}

int set_hours_M41T64(unsigned int fd_i2c, uint8_t val_hours, unsigned int addr_slave){

	uint8_t buf[2];

	if ( val_hours >= 0x24){
		printf("Failed RTC no correct value of hours.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of hours
		buf[0] = 0x03; // Address register in M41T64
		buf[1] = 0b00111111 & val_hours;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x02.\n");
		// сделать запись в лог
		return -1;
	}

	return 0;
}

int get_hours_M41T64(unsigned int fd_i2c, unsigned int addr_slave){

	uint8_t buf[2];
	uint8_t ret;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of hours register
	buf[0] = 0x03; // Address register in M41T64

	if( write(fd_i2c,buf,1) != 1) {
		printf("Failed to write to read  the i2c bus register 0x01.\n");
		// сделать запись в лог
		return -1;
	}

    if( read (fd_i2c,buf,1) !=1){
    	printf("Failed to read to the i2c bus register 0x01.\n");
    			// сделать запись в лог
    	return -1;
    }

    ret = 0b00111111 & buf[0];

    return ret;

}

int set_day_M41T64(unsigned int fd_i2c, uint8_t val_day, unsigned int addr_slave){

	uint8_t buf[2];

	if ( val_day >= 0x32 || val_day == 0 ){
		printf("Failed RTC no correct value of hours.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of hours
		buf[0] = 0x05; // Address register in M41T64
		buf[1] = 0b00111111 & val_day;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x02.\n");
		// сделать запись в лог
		return -1;
	}

	return 0;
}

int get_day_M41T64(unsigned int fd_i2c, unsigned int addr_slave){

	uint8_t buf[2];
	uint8_t ret;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of hours register
	buf[0] = 0x05; // Address register in M41T64

	if( write(fd_i2c,buf,1) != 1) {
		printf("Failed to write to read  the i2c bus register 0x01.\n");
		// сделать запись в лог
		return -1;
	}

    if( read (fd_i2c,buf,1) !=1){
    	printf("Failed to read to the i2c bus register 0x01.\n");
    			// сделать запись в лог
    	return -1;
    }

    ret = 0b00111111 & buf[0];

    return ret;
}




int set_century_month_M41T64(unsigned int fd_i2c, uint8_t val, unsigned int addr_slave){

	uint8_t buf[2];



	//Write value of hours
		buf[0] = 0x06; // Address register in M41T64
		buf[1] = 0b11011111 & val;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x02.\n");
		// сделать запись в лог
		return -1;
	}

	return 0;
}

int get_century_month_M41T64(unsigned int fd_i2c, unsigned int addr_slave){

	uint8_t buf[2];
	uint8_t ret;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of hours register
	buf[0] = 0x06; // Address register in M41T64

	if( write(fd_i2c,buf,1) != 1) {
		printf("Failed to write to read  the i2c bus register 0x01.\n");
		// сделать запись в лог
		return -1;
	}

    if( read (fd_i2c,buf,1) !=1){
    	printf("Failed to read to the i2c bus register 0x01.\n");
    			// сделать запись в лог
    	return -1;
    }

    ret = 0b11011111 & buf[0];

    return ret;

}


int set_year_M41T64(unsigned int fd_i2c, uint8_t val_year, unsigned int addr_slave){

	uint8_t buf[2];

	if ( val_year >= 0x9A ){
		printf("Failed RTC no correct value of hours.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of year
		buf[0] = 0x07; // Address register in M41T64
		buf[1] = val_year;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	if( write(fd_i2c,buf,2) != 2) {
		printf("Failed to write to the i2c bus register 0x02.\n");
		// сделать запись в лог
		return -1;
	}

	return 0;
}

int get_year_M41T64(unsigned int fd_i2c, unsigned int addr_slave){

	uint8_t buf[2];
	uint8_t ret;

	//Set address for the device on the i2c bus
	if (ioctl(fd_i2c,I2C_SLAVE,addr_slave) < 0) {
		printf("Failed to acquire bus access and/or talk to slave.\n");
		// сделать запись в лог
		return -1;
	}

	//Write value of year register
	buf[0] = 0x07; // Address register in M41T64

	if( write(fd_i2c,buf,1) != 1) {
		printf("Failed to write to read  the i2c bus register 0x01.\n");
		// сделать запись в лог
		return -1;
	}

    if( read (fd_i2c,buf,1) !=1){
    	printf("Failed to read to the i2c bus register 0x01.\n");
    			// сделать запись в лог
    	return -1;
    }

    ret = buf[0];

    return ret;
}


