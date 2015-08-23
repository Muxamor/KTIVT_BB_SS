/*
 * GPIO_SS.h
 *
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev ivan.neskorodev@gmail.com
 *
 */
#include "../include/GPIO_SS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>




/*********************************************
 * gpio export
 ********************************************/

int gpio_export(unsigned int gpio){

	int fd, len;
	char buf[64];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0){
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio );

	write(fd,buf,len);

	close(fd);
	return 0;

}

/*********************************************
 * gpio unexport
 ********************************************/

int gpio_unexport(unsigned int gpio){

	int fd, len;
	char buf[64];

	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0){
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio );

	write(fd,buf,len);

	close(fd);
	return 0;

}

/******************************************************
 * Gpio set direction out or input
 *****************************************************/

int gpio_set_direction(unsigned int gpio, enum PIN_DIRECTION flag_direction ){

	int fd;
	char buf[64];

	snprintf(buf,sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction",gpio);

	fd=open(buf, O_WRONLY);
	if (fd<0){
		perror("gpio/direction");
		return fd;
	}

	if (flag_direction == OUTPUT_PIN){
		write(fd,"out",4);
	} else {
		write(fd,"in",3);
	}

	close(fd);
	return 0;
}

/******************************************************
 * Gpio set direction out or input
 *****************************************************/

int gpio_set_edge(unsigned int gpio, char *edge){

	int fd;
	char buf[64];

	snprintf(buf,sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge",gpio);

	fd=open(buf, O_WRONLY);
	if (fd<0){
		perror("gpio/edge");
		return fd;
	}


		write(fd, edge, strlen(edge)+1);


	close(fd);

	return 0;
}

/******************************************************
 * Gpio set direction value
 *****************************************************/
int gpio_set_value(unsigned int gpio, enum PIN_VALUE value, int fd ){

	//snprintf(buf,sizeof(buf),SYSFS_GPIO_DIR "/gpio%d/value",gpio);
	//fd=open(buf, O_WRONLY);
		//if (fd<0){
		//	perror("gpio/value");
		//}

	if(value == HIGHT){
		write(fd,"1",2);
	}else{
		write(fd,"0",2);
	}

	//close(fd);
	return 0;
}

/******************************************************
 * Gpio open file value Read Only
 *****************************************************/
int gpio_get_value_interrupt(int fd, int timeout){

	struct pollfd pollfd[1];
	char c;
	int err;

	pollfd[0].fd = fd;
	pollfd[0].events = POLLPRI|POLLERR;
	pollfd[0].revents = 0;

	err = poll(pollfd, 1, timeout);
	if( err != 1){
		return -1;
	}

	lseek(fd, 0, SEEK_SET);
	err = read(fd,&c,sizeof(c));
	if( err != 1 ){
	    return -1;
	}

	return 1;// c - '0';
}


/******************************************************
 * Gpio open file value Write Only
 *****************************************************/
int gpio_fd_open_W_O(unsigned int gpio){

	int fd;
	char buf[64];

	snprintf(buf,sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value",gpio);

	fd=open(buf, O_WRONLY);
	if (fd<0){
		perror("gpio/value write only");
		return fd;
	}

	return fd;
}

/******************************************************
 * Gpio open file value Read Only
 *****************************************************/
int gpio_fd_open_R_O(unsigned int gpio){

	int fd;
	char c;
	char buf[64];

	snprintf(buf,sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value",gpio);

	fd=open(buf, O_RDONLY );
	if (fd<0){
		perror("gpio/value read only");
		return fd;
	}

	read(fd, &c, sizeof(c));

	return fd;
}

/******************************************************
 * Gpio close file
 *****************************************************/
int gpio_fd_close(int fd){

	return close(fd);
}

