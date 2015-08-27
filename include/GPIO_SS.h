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

typedef enum {
	GPIO_SPI_CS_Ch1,
	GPIO_SPI_CS_Ch2,
	GPIO_SPI_CS_Ch3,
	GPIO_SPI_INT_Ch1,
	GPIO_SPI_INT_Ch2,
	GPIO_SPI_INT_Ch3,
	GPIO_SPI_CS_Disp,
	GPIO_Sync_Ch1_Ch2_Ch3,
	GPIO_SPI_Reset_Ch1,
	GPIO_SPI_Reset_Ch2,
	GPIO_SPI_Reset_Ch3,
	GPIO_SPI_CS_Col,
	GPIO_MAX
} channel_indexes_t;

static int gpio_channel_numbers[GPIO_MAX] = { 115, 117, 48, 7, 26, 27, 86, 89, 65, 70, 71, 49 };



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
