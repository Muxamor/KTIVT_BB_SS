/*
 * GPIO_SS.h
 *
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev ivan.neskorodev@gmail.com
 *
 */

#ifndef GPIO_SS_H_
#define GPIO_SS_H_

/****************************************************
*                 Constants                         *
****************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"


enum PIN_DIRECTION{
	INPUT_PIN=0,
	OUTPUT_PIN=1
};

enum PIN_VALUE{
	LOW=0,
	HIGHT=1
};

/***************************************************
*                GPIO function                     *
***************************************************/
int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_direction(unsigned int gpio, enum PIN_DIRECTION flag_direction );
int gpio_set_edge(unsigned int gpio, char *edge);
int gpio_set_value(unsigned int gpio, enum PIN_VALUE value, int fd );
int gpio_get_value_interrupt(int fd, int timeout);
int gpio_fd_open_W_O(unsigned int gpio);
int gpio_fd_open_R_O(unsigned int gpio);
int gpio_fd_close(int fd);


#endif
