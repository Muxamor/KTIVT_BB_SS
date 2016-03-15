/*
 * Eth_fn.c
 *
 *  Created on: 14 марта 2016 г.
 *      Author: Ivan
 */
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "../include/GPIO_SS.h"
#include "../include/SPI_SS.h"
#include "../include/BB_Setup.h"
#include "../include/Config_pars.h"
#include "../include/Eth_fn.h"


int pars_eth_command_parsel(int fd_SPI,  uint8_t *rx_buf_eth, uint8_t *tx_buf_eth, uint32_t size_rx, struct settings_ch *settings_channels){

	uint16_t tx_mk_buf[2];
 	uint16_t rx_mk_buf[2];
 	uint16_t number_channel;
 	uint8_t number_command;
 	uint8_t ret;
 	//uint16_t quantity_command_eth_parsel;
 	uint16_t tmp;
 	uint8_t tmp1;

 	int i,j;

 	gpio_name_output_pin gpio_pin_res;
 	gpio_name_output_pin gpio_pin_spi_cs;
 	gpio_name_output_pin gpio_pin_spi_int;
 	gpio_name_output_pin gpio_pin_res_array[] = { 0x00, GPIO_SPI_Reset_Ch1, GPIO_SPI_Reset_Ch2, GPIO_SPI_Reset_Ch3 };
 	gpio_name_output_pin gpio_pin_spi_cs_array[] = { 0x00, GPIO_SPI_CS_Ch1, GPIO_SPI_CS_Ch2, GPIO_SPI_CS_Ch3 };
 	gpio_name_output_pin gpio_pin_spi_int_array[] = { 0x00, GPIO_SPI_INT_Ch1, GPIO_SPI_INT_Ch2, GPIO_SPI_INT_Ch3 };


 	for(i=0, j=0; i < size_rx ; i++){

 		tmp =  rx_buf_eth[i];
 		tmp = tmp << 8;
 		i++;
 		tmp = tmp | (rx_buf_eth[i]);
 		number_channel = tmp;
 	    i++;

 		number_command = rx_buf_eth[i];

 		tmp = rx_buf_eth[i];
 		tmp = tmp << 8;
 		i++;
 		tmp = tmp | rx_buf_eth[i];
 		tx_mk_buf[0] = tmp;
 		i++;
 		tmp = rx_buf_eth[i];
 		tmp = tmp << 8;
 		i++;
 		tmp = tmp | (rx_buf_eth[i]);
 		tx_mk_buf[1] = tmp;

 		gpio_pin_spi_cs = gpio_pin_spi_cs_array[number_channel];
 		gpio_pin_spi_int = gpio_pin_spi_int_array[number_channel];


 		if((number_command == 0x21)|| (number_command == 0x22)){///этот кусок под вопросом переделать
 			spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch1, GPIO_SPI_INT_Ch1, tx_mk_buf,rx_mk_buf, sizeof(tx_mk_buf), 0 );
 			spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch2, GPIO_SPI_INT_Ch2, tx_mk_buf,rx_mk_buf, sizeof(tx_mk_buf), 0 );
 			spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch3, GPIO_SPI_INT_Ch3, tx_mk_buf,rx_mk_buf, sizeof(tx_mk_buf), 0 );

 			number_channel = 0x0000;

 			if (number_command == 0x21){
 				gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , HIGHT);
 				gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , LOW);
 				printf(" Enable Sync\n");
 			}

 		}else{

 			ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, tx_mk_buf,rx_mk_buf, sizeof(tx_mk_buf), 0 );

 			if ( ret == -1 ){
 				//сделать запись в лог файл
 				printf("Ch%d SPI Eth data write ERROR\n", number_channel );
 				return -1;
 			}
 		}

 		tmp1 =(uint8_t) (number_channel>>8);
 		tx_buf_eth[j] = tmp1;
 		j++;
 		tmp1 =(uint8_t) number_channel;
 		tx_buf_eth[j] = tmp1;
 		j++;

 		tmp1 =(uint8_t) (rx_mk_buf[0]>>8);
 		tx_buf_eth[j] = tmp1;
 		j++;
 		tmp1 =(uint8_t) rx_mk_buf[0];
 		tx_buf_eth[j] = tmp1;
 		j++;

 		tmp1 =(uint8_t) (rx_mk_buf[1]>>8);
 		tx_buf_eth[j] = tmp1;
 		j++;
 		tmp1 =(uint8_t) rx_mk_buf[1];
 		tx_buf_eth[j] = tmp1;
 		j++;

 	}

 	return 0;
 }

