/*
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 * In Eclipse add Include path Windows
 *     C:\gcc-linaro\arm-linux-gnueabihf\libc\usr\include
 *
 * In Eclipse add Include path Linux
 *     /usr/local/linaro/arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/include
 *
 */

//
#include <stdio.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include "../include/GPIO_SS.h"
#include "../include/SPI_SS.h"
#include "../include/BB_Setup.h"
#include "../include/main.h"



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
  
/****************************************************************
 * Our main function
 ****************************************************************/
 int main(void){

	 int ret=0;


	ret = Default_Setup_GPIO_BB();
	if (ret==-1){
		perror("Default_Setup_GPIO_BB()");
		return EXIT_FAILURE;
	}
	//Default_Setup_SPI_BB();

	fd_SPI_BB = spi_device_open("/dev/spidev1.0");
	set_spi_settings(fd_SPI_BB, SPI_MODE_1, 16 , 18000000);


	uint16_t tx_buf[] = {0x0104,0x0000};
	uint16_t rx_buf[ARRAY_SIZE(tx_buf)] = {};
	spi_transfer (fd_SPI_BB, GPIO_SPI_CS_Ch2 ,tx_buf, rx_buf, sizeof(tx_buf), 0);

	while((gpio_get_value_interrupt(fd_GPIO_pin_input[GPIO_SPI_INT_Ch2],0)) != 1);

	uint16_t tx_buf1[] = {0x0000,0x0000};
	uint16_t rx_buf1[ARRAY_SIZE(tx_buf1)] = {};
	spi_transfer (fd_SPI_BB, GPIO_SPI_CS_Ch2 ,tx_buf1, rx_buf1, sizeof(tx_buf1), 0);

	int i;
	for (i = 0; i < ARRAY_SIZE(tx_buf1); i++){
		printf("0x%.4X ", rx_buf1[i]);
	}
	printf("\n");

	printf("sleep 1 second\n");
	sleep(1);
	int check;

	//gpio_input_pin_numbers[GPIO_SPI_INT_Ch1]
	check= gpio_get_value_interrupt(fd_GPIO_pin_input[GPIO_SPI_INT_Ch1],0);

	if(check==-1){
				printf("interrupt no happen\n");
			}else{

				printf("interrupt happen %d\n", check);
			}
	printf("sleep 1 second\n");
	sleep(1);
	check= gpio_get_value_interrupt(fd_GPIO_pin_input[GPIO_SPI_INT_Ch1],0);

		if(check==-1){
			printf("interrupt no happen \n");
		}else{

			printf("interrupt happen %d\n", check);
		}
	printf("sleep 1 second\n");
	sleep(1);
			check= gpio_get_value_interrupt(fd_GPIO_pin_input[GPIO_SPI_INT_Ch1], 100);

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

