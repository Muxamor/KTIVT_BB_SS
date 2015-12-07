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
#include "../include/GPIO_SS.h"
#include "../include/SPI_SS.h"
#include "../include/BB_Setup.h"
#include "../include/Config_pars.h"
#include "../include/main.h"

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
    uint32_t i;

	ret = Default_Setup_GPIO_BB();
	if (ret==-1){
		perror("Default_Setup_GPIO_BB() - FAILUR");
		return EXIT_FAILURE;
	}

	fd_SPI_BB = spi_device_open("/dev/spidev1.0");
	set_spi_settings(fd_SPI_BB, SPI_MODE_1, 16 , 18000000);

	//Чтение конфига и обратбота

    FILE *fd_config_file = fopen("KTIVT_BB_SS.conf", "r");
    if(fd_config_file==NULL){
    	printf("Configure file is not found");
    	return EXIT_FAILURE;
    }
    struct settings_ch cfg_ch_old[3];
    struct settings_ch cfg_ch_new[3];
    // read config
    parse_config(fd_config_file, cfg_ch_old, 3);

    //sent settings to analog channel



int parse_sent_settings (int fd_SPI, struct settings_ch *settings_old, struct settings_ch *settings_new, uint8_t compare_settings, uint8_t quantity_channels){

      uint8_t apply_settings[ quantity_channels ];

      uint16_t tx_buf[2] = { 0 };
      uint16_t rx_buf[ARRAY_SIZE(tx_buf)] = { 0 };
      unsigned int gpio_pin_res;
      unsigned int gpio_pin_spi_cs;
      unsigned int gpio_pin_spi_int;
      int ret;

      if( compare_settings == 0 ){
    	  uint8_t i, number_ch;

    	  for ( i=0, number_ch=0  ; i < quantity_channels; i++, number_ch++ ){

    		  switch (number_ch) {
    		  	  case 0:
    		  		  gpio_pin_res = GPIO_SPI_Reset_Ch1;
    		  		  gpio_pin_spi_cs = GPIO_SPI_CS_Ch1;
    		  		  gpio_pin_spi_int = GPIO_SPI_INT_Ch1;
    		  		  break;
    		  	  case 1:
    		  		  gpio_pin_res = GPIO_SPI_Reset_Ch2;
    		  		  gpio_pin_spi_cs = GPIO_SPI_CS_Ch2;
    		  		  gpio_pin_spi_int = GPIO_SPI_INT_Ch2;
    		  		  break;
    		  	  case 2:
    		  		  gpio_pin_res = GPIO_SPI_Reset_Ch3;
    		  		  gpio_pin_spi_cs = GPIO_SPI_CS_Ch3;
    		  		  gpio_pin_spi_int = GPIO_SPI_INT_Ch3;
    		  		  break;
    		  }

    		  //Check on/off channel
    		  if ( settings_old[number_ch].mode == 1 ){
    			  enable_analog_channel (gpio_pin_res);
    		  }else{
    			  disable_analog_channel (gpio_pin_res);
    			  continue;
    		  }

    		  // Write Input switching setting
    		  if(settings_old[number_ch].config_ch.input==inp_1_1){
    			  tx_buf[0]=0x0100;
    		  }else if(settings_old[number_ch].config_ch.input==inp_1_10){
    			  tx_buf[0]=0x0101;
    		  }else if(settings_old[number_ch].config_ch.input==inp_0V){
    			  tx_buf[0]=0x0102;
    		  }else if(settings_old[number_ch].config_ch.input==inp_cal){
    			  tx_buf[0]=0x0103;
    		  }else if(settings_old[number_ch].config_ch.input==z_state){
    			  tx_buf[0]=0x0104;
    		  }

    		  ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, tx_buf,rx_buf,  sizeof(tx_buf), 0 );

    		  if ( ret == -1 || rx_buf[0] != 0x0001){
    			  //сделать запись в лог файл
    			  printf("Set settings Input switch: FOULT");
    			  return -1;
    		  } else {
    			  //сделать запись  в лог файл
    			  printf("Set settings Input switch: OK");
    		  }

    		  //Write amplifier KU1


        }



      } else if( compare_settings == 1 ){
		// сравниваем новый конфиг и старый.

      }

	}




    //добавить обработку полученных настроек обработку полученных настроек
/*
    // write input parameter
    for(i = 0; i < 3; i++){
        printf("write(ch=%d, opt=KU1, val=%d)\n", i+1, cfg_old[i].ku1);
    }
*/

	//printf("> ");

	//fgets(console_buffer, sizeof(console_buffer), stdin);

	//printf("%s",console_buffer);


/*
	uint16_t tx_buf[] = {0x0104,0x0000};
	uint16_t rx_buf[ARRAY_SIZE(tx_buf)] = {};
	spi_transfer (fd_SPI_BB, GPIO_SPI_CS_Ch1 ,tx_buf, rx_buf, sizeof(tx_buf), 0);

	while((gpio_get_value_interrupt(fd_GPIO_pin_input[GPIO_SPI_INT_Ch1],0)) != 1);

	uint16_t tx_buf1[] = {0x0000,0x0000};
	uint16_t rx_buf1[ARRAY_SIZE(tx_buf1)] = {};
	spi_transfer (fd_SPI_BB, GPIO_SPI_CS_Ch1 ,tx_buf1, rx_buf1, sizeof(tx_buf1), 0);


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

*/
	 return EXIT_SUCCESS;
}
