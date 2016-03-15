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
#include <unistd.h>
#include <stdint.h>
//#include </usr/local/Cellar/libconfig/1.5/include/libconfig.h>
#include <linux/spi/spidev.h>
//#include "../include/main.h"
#include "../include/GPIO_SS.h"
#include "../include/SPI_SS.h"
#include "../include/BB_Setup.h"
#include "../include/Config_pars.h"
#include "../include/analog_ch_setup.h"
#include "../include/M41T64_Clock.h"
#include "../include/Eth_fn.h"

/*Спшуля писала
int ConfigCompare ( config_t *Config1, config_t *Config2 )
{
	return memcpy( Config1, Config2, sizeof(&Config1));
}
*/

//#include <getopt.h>
//#include <sys/ioctl.h>
//#include <linux/types.h>
//#include <string.h>
//#include <errno.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <poll.h>
/*
static void pabort(const char *s){
	perror(s);
	abort();
}
  */


/****************************************************************
 *Main function
 ****************************************************************/
 int main(void){

	char console_buffer[30];

	int ret=0;
    uint32_t i, j;


	ret = Default_Setup_GPIO_BB();
	if (ret==-1){
		perror("Default_Setup_GPIO_BB() - FAILUR");
		return EXIT_FAILURE;
	}

	ret = Defult_setup_M41T64(I2C_BUS_NUMBER, ADDR_I2C_SLAVE_M41T64); //Set 4096 Hz
	if (ret==-1){
		perror("Defult_setup_M41T64() - FAILUR");
		return EXIT_FAILURE;
	}
	sleep(1);//To wait ADC ready

	int fd_SPI_BB;
	fd_SPI_BB = spi_device_open("/dev/spidev1.0");
	set_spi_settings(fd_SPI_BB, SPI_MODE_1, 0, 16, 18000000);

	//Read file configuration and pars

    struct settings_ch cfg_ch_old[3] = {0};
    struct settings_ch cfg_ch_new[3] = {0};


    FILE *fd_config_file = fopen("/kti_bb_ss/KTIVT_BB_SS.conf", "r");
    if(fd_config_file==NULL){
    	//сделать запись в лог фаил
    	printf("Configure file is not found\n");
    	return EXIT_FAILURE;
    }

    // read config
    parse_config(fd_config_file, cfg_ch_old, 3);
    //дать синхро импульс
    fclose(fd_config_file);

    /*
    fd_config_file = fopen("/kti_bb_ss/KTIVT_BB_SSnew.conf", "r");
    if(fd_config_file==NULL){
    	//сделать запись в лог фаил
    	printf("Configure file is not found\n");
    	return EXIT_FAILURE;
    }

    // read config
    parse_config(fd_config_file, cfg_ch_new, 3);
    fclose(fd_config_file);
*/
    //sent settings to analog channel
    ret = parse_sent_settings (fd_SPI_BB, cfg_ch_old, cfg_ch_new, sizeof(cfg_ch_old[0]) ,0, 3);
	if (ret==-1){
		perror("parse_sent_settings() - FAILUR");
		return EXIT_FAILURE;
	}

/*
	ret = parse_sent_settings (fd_SPI_BB, cfg_ch_old, cfg_ch_new, sizeof(cfg_ch_old[0]) ,1, 3);
	if (ret==-1){
		perror("parse_sent_settings() - FAILUR");
		return EXIT_FAILURE;
	}

*/

/*
    int testmemcmp;

    testmemcmp = memcmp(&cfg_ch_old[0], &cfg_ch_new[0], sizeof(cfg_ch_old[1]) );

    if (testmemcmp == 0){
    	printf("massive same\n");

    }else{

    	printf("massive not same\n");
    }
*/


    //добавить обработку полученных настроек обработку полученных настроек



	uint8_t test_buf_rx[] = {0x00,0x01, 0x01,0x03, 0x00,0x00,
							   0x00,0x01, 0x02,0x00, 0x00,0x00,
							   //0x00,0x00, 0x21,0x01, 0x00,0x00,
							   0x00,0x02, 0x01,0x03, 0x00,0x00,
							   0x00,0x02, 0x02,0x00, 0x00,0x00,
							 //  0x00,0x00, 0x21,0x01, 0x00,0x00,
							   0x00,0x03, 0x01,0x03, 0x00,0x00,
							   0x00,0x03, 0x02,0x00, 0x00,0x00,
							   0x00,0x00, 0x21,0x01, 0x00,0x00};

	uint8_t test_buf_tx[sizeof(test_buf_rx)] = {};
	printf("\n");

	pars_eth_command_parsel(fd_SPI_BB, test_buf_rx, test_buf_tx,  sizeof (test_buf_rx), cfg_ch_old);

	int k, size_buf_eth;
	size_buf_eth = (( sizeof(test_buf_rx)/6) + 1 );

	for(j = 1, i= 0, k=0; j < size_buf_eth ; j++ ){

		printf(" Eth Command -- Answer: ");

		for ( ; i < j*6 ; i++){
			printf("0x%.2X ", test_buf_rx[i]);
				}

		printf("-- ");

		for ( ; k < j*6 ; k++){
			printf("0x%.2X ", test_buf_tx[k]);
		}

		printf("\n");
	}

/*

	 gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , HIGHT);
	 gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , LOW);
	 printf("Enable Sync\n");

	 int j=0;

	 uint16_t tx_buf[8206] = {0x0000};
	 uint16_t rx_buf[8206] = {};


while(j!=36000){

	printf("Cicle = %d\n",j);
	j++;
	printf("Channel 1\n");
    while((gpio_get_value_interrupt(fd_GPIO_pin_input[GPIO_SPI_INT_Ch1],0)) != 1);
    printf("Wait interrup 1 get\n");

    ret = spi_read_data_ADC24 (fd_SPI_BB, GPIO_SPI_CS_Ch1, GPIO_SPI_INT_Ch1, tx_buf, rx_buf, sizeof(tx_buf), 0);
    if(ret != 0){
    	perror("Error SPI\n");
    }

    	for (i = 0; i < 20; i++){
    		printf("0x%.4X ", rx_buf[i]);
    	}
    	printf("\n");

    	//sleep(1);

    	for (i = 8186; i < 8206; i++){
    		printf("0x%.4X ", rx_buf[i]);
    	}
    	printf("\n");


    	printf("Channel 2\n");
        while((gpio_get_value_interrupt(fd_GPIO_pin_input[GPIO_SPI_INT_Ch2],0)) != 1);
        printf("Wait interrup 1 get\n");

        ret = spi_read_data_ADC24 (fd_SPI_BB, GPIO_SPI_CS_Ch2, GPIO_SPI_INT_Ch2, tx_buf, rx_buf, sizeof(tx_buf), 0);
        if(ret != 0){
        	perror("Error SPI\n");
        }

        	for (i = 0; i < 20; i++){
        		printf("0x%.4X ", rx_buf[i]);
        	}
        	printf("\n");

        	//sleep(1);

        	for (i = 8186; i < 8206; i++){
        		printf("0x%.4X ", rx_buf[i]);
        	}
        	printf("\n");

	printf("Channel 3\n");
            while((gpio_get_value_interrupt(fd_GPIO_pin_input[GPIO_SPI_INT_Ch3],0)) != 1);
            printf("Wait interrup 1 get\n");

            ret = spi_read_data_ADC24 (fd_SPI_BB, GPIO_SPI_CS_Ch3, GPIO_SPI_INT_Ch3, tx_buf, rx_buf, sizeof(tx_buf), 0);
            if(ret != 0){
            	perror("Error SPI\n");
            }

            	for (i = 0; i < 20; i++){
            		printf("0x%.4X ", rx_buf[i]);
            	}
            	printf("\n");

            	//sleep(1);

            	for (i = 8186; i < 8206; i++){
            		printf("0x%.4X ", rx_buf[i]);
            	}
            	printf("\n");




}*/


	 return EXIT_SUCCESS;

}





	//printf("> ");

	//fgets(console_buffer, sizeof(console_buffer), stdin);

	//printf("%s",console_buffer);


	//gpio_input_pin_numbers[GPIO_SPI_INT_Ch1]
/*	check= gpio_get_value_interrupt(fd_GPIO_pin_input[GPIO_SPI_INT_Ch1],0);

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
*/







