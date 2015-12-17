/*
 * analog_ch_setup.c
  *
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */
#include <stdio.h>
#include <stdint.h>
#include "../include/BB_Setup.h"
#include "../include/Config_pars.h"
#include "../include/analog_ch_setup.h"






 int parse_sent_settings (int fd_SPI, struct settings_ch *settings_old, struct settings_ch *settings_new, uint8_t compare_settings, uint8_t quantity_channels){

          uint8_t apply_settings[ quantity_channels ];

          uint16_t tx_buf[2] = { 0 };
          uint16_t rx_buf[2] = { 0 };
          gpio_name_output_pin gpio_pin_res;
          gpio_name_output_pin gpio_pin_spi_cs;
          gpio_name_output_pin gpio_pin_spi_int;
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
        			  //сделать запись в лог файл
        			  printf("Ch%d Mode: ON\n", number_ch+1 );
        		  }else{
        			  disable_analog_channel (gpio_pin_res);
        			  printf("Ch%d Mode: OFF\n", number_ch+1 );
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
        		  tx_buf[1]=0x0000;

        		  ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, tx_buf,rx_buf, sizeof(tx_buf), 0 );

        		  if ( ret == -1 || rx_buf[0] != 0x0001){
        			  //сделать запись в лог файл
        			  printf("Ch%d setup Input switch: FOULT\n", number_ch+1 );
        			  return -1;
        		  } else {
        			  //сделать запись  в лог файл
        			  printf("Ch%d setup Input switch: OK\n", number_ch+1);
        		  }

        		  //Write amplifier KU1


        	  }

        	  return 0;


          } else if( compare_settings == 1 ){
    		// сравниваем новый конфиг и старый.

          }
          return 0;

    }

