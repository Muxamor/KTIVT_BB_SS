/*
 * Eth_fn.c
 *
 *  Created on: 14 марта 2016 г.
 *      Author: Ivan
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "../include/GPIO_SS.h"
#include "../include/SPI_SS.h"
#include "../include/BB_Setup.h"
#include "../include/Config_pars.h"
#include "../include/Eth_fn.h"


int sendall(int fd_s, char *buf, int len, int flags){
    int total = 0;
    int n;

    while(total < len)
    {
        n = send(fd_s, buf+total, len-total, flags);
        if(n == -1) { break; }
        total += n;
    }

    return (n==-1 ? -1 : total);
}

int pars_eth_command_parcel(int fd_SPI, int fd_socket, uint8_t *rx_buf_eth,  uint8_t *tx_buf_eth, uint32_t *real_rx_tx_buf_eth_size, struct settings_ch *settings_channels){

	uint16_t tx_mk_buf[2];
 	uint16_t rx_mk_buf[2];
 	uint16_t number_channel;
 	uint8_t number_command;
 	int32_t ret;
 	//uint16_t quantity_command_eth_parsel;
 	uint16_t tmp;
 	uint8_t tmp1;

 	uint32_t i,j;
 	uint32_t current_size_rx_eth_parsl;

 	gpio_name_output_pin gpio_pin_res;
 	gpio_name_output_pin gpio_pin_spi_cs;
 	gpio_name_output_pin gpio_pin_spi_int;
 	gpio_name_output_pin gpio_pin_res_array[] = { 0x00, GPIO_SPI_Reset_Ch1, GPIO_SPI_Reset_Ch2, GPIO_SPI_Reset_Ch3 };
 	gpio_name_output_pin gpio_pin_spi_cs_array[] = { 0x00, GPIO_SPI_CS_Ch1, GPIO_SPI_CS_Ch2, GPIO_SPI_CS_Ch3 };
 	gpio_name_output_pin gpio_pin_spi_int_array[] = { 0x00, GPIO_SPI_INT_Ch1, GPIO_SPI_INT_Ch2, GPIO_SPI_INT_Ch3 };

 	//Read size parcel from Eth
 	ret = recv(fd_socket, rx_buf_eth, 4, 0);//rx_buf_eth //&current_size_rx_eth_parsl
 	if(ret <= 0){
 		return -1;
 	}

 	current_size_rx_eth_parsl = rx_buf_eth[0];
 	current_size_rx_eth_parsl = (current_size_rx_eth_parsl<<8)|(rx_buf_eth[1]);
 	current_size_rx_eth_parsl = (current_size_rx_eth_parsl<<8)|(rx_buf_eth[2]);
 	current_size_rx_eth_parsl = (current_size_rx_eth_parsl<<8)|(rx_buf_eth[3]);

 	printf(" Size command parcel from Eth: %d\n",current_size_rx_eth_parsl );

    //Realloc  rx_buf_eth  for eth parcel if size parcel more than current size rx_buf_eth
 	if(current_size_rx_eth_parsl > (*real_rx_tx_buf_eth_size)){

 		*real_rx_tx_buf_eth_size = current_size_rx_eth_parsl+16; //+16 for type and size for tx to eth parcel
 		rx_buf_eth = (uint8_t*)realloc(rx_buf_eth, *real_rx_tx_buf_eth_size);
 		if (rx_buf_eth==NULL) {
 			free (rx_buf_eth);
 			printf (" Error reallocating memory rx_buf_eth in eth fun /n");
 			exit (1);
 		}
 		//дописать проверку на ошибки

 		tx_buf_eth = (uint8_t*)realloc(tx_buf_eth, *real_rx_tx_buf_eth_size);
 		if (tx_buf_eth==NULL) {
 			free (tx_buf_eth);
 			printf (" Error reallocating memory tx_buf_ethin eth fun /n");
 			exit (1);
 		}
 		//дописать проверку на ошибки
 	}

 	//Read receive command from eth
 	ret = recv(fd_socket, rx_buf_eth, current_size_rx_eth_parsl, 0);
 	if(ret <= 0){
 		return -1;
 	}

 	//Только для тестирования потом удалить  после тестов
 	int p;
	for(p =0; p < current_size_rx_eth_parsl; p++ ){

		if(p==0){
			printf(" Receive from eth  command: ");
		}else if(p%6 == 0x00){
			printf(" \n");
			printf(" Receive from eth  command: ");
		}
 		printf("0x%.2X ", rx_buf_eth[p]);

 	}
	printf(" \n");

//////////////////////////////////////////////////////////////////////////////

 	//Write type answer parcel to Eth
 	j = 0;
 	tx_buf_eth[j] = 0x00;
 	j++;
 	tx_buf_eth[j] = 0x00;
 	j++;
 	tx_buf_eth[j] = 0x00;
 	j++;
 	tx_buf_eth[j] = 0x02; // Type mean that  parcel is answer
 	//Space for size answer parcel to Eth
 	j++;
 	j++;
 	j++;
 	j++;
 	// set j to write answer from analog channel
 	j++;

 	for(i=0 ; i < current_size_rx_eth_parsl ; i++){

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


 		if(number_command == 0x22){ //Stop command
 			// дописать проверку возврощаемых значений полноценную
 			number_channel = 0x0000;
 			spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch1, GPIO_SPI_INT_Ch1, tx_mk_buf,rx_mk_buf, sizeof(tx_mk_buf), 0 );
 			spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch2, GPIO_SPI_INT_Ch2, tx_mk_buf,rx_mk_buf, sizeof(tx_mk_buf), 0 );
 			ret = spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch3, GPIO_SPI_INT_Ch3, tx_mk_buf,rx_mk_buf, sizeof(tx_mk_buf), 0 );

 			if(ret == -1){
 				rx_mk_buf[0] = 0x0002;
 				rx_mk_buf[1] = 0x0000;
 			}

 		}else if(number_command == 0x21){// Start Sync Enable
 			//дописать обработку флагов реально
 			number_channel = 0x0000;
 			rx_mk_buf[0] = 0x0001;
 			rx_mk_buf[1] = 0x0000;

 			if( tx_mk_buf[0] == 0x2101 ){
 				gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , HIGHT);
 				gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , LOW);
 				printf(" Enable Sync\n");

 			}else if(tx_mk_buf[0] == 0x2108 ){

 			}else{
 				rx_mk_buf[0] = 0x0002;
 				rx_mk_buf[1] = 0x0000;
 			}

 		}else if(number_command == 0x2E){ // Enable/Disable analog channel

 			number_channel = 0x0000;
 			rx_mk_buf[0] = 0x0001;
 			rx_mk_buf[1] = 0x0000;

 			if(((tx_mk_buf[0]) == 0x2E01) || ((tx_mk_buf[0]) == 0x2E02) || ((tx_mk_buf[0]) == 0x2E03)){

 				gpio_pin_res = gpio_pin_res_array[((uint8_t)tx_mk_buf[0])];

 				if(tx_mk_buf[1] == 0x0001){ //Enable channel
 					enable_analog_channel (gpio_pin_res);
 				}else if(tx_mk_buf[1] == 0x0000) {//Disable Channel
 					disable_analog_channel (gpio_pin_res);
 				}else{
 					rx_mk_buf[0] = 0x0002;
 					rx_mk_buf[1] = 0x0000;
 				}

 			}else{
 				rx_mk_buf[0] = 0x0002;
 				rx_mk_buf[1] = 0x0000;
 			}


 		}else{

 			ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, tx_mk_buf,rx_mk_buf, sizeof(tx_mk_buf), 0 );

 			if ( ret == -1 ){
 				//сделать запись в лог файл
 				printf(" Ch%d SPI Eth data write ERROR\n", number_channel );
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

 	//Write size of answer parcel
 	if(current_size_rx_eth_parsl != (j-1)){

 		tx_buf_eth[4] = (uint8_t)(current_size_rx_eth_parsl>>24);
 		tx_buf_eth[5] = (uint8_t)(current_size_rx_eth_parsl>>16);
 		tx_buf_eth[6] = (uint8_t)(current_size_rx_eth_parsl>>8);
 		tx_buf_eth[7] = (uint8_t)current_size_rx_eth_parsl;

 		printf(" Size answer parcel: %d+8\n", current_size_rx_eth_parsl );
 	}

 	//Sent parcel with answer to eth
 	ret = sendall(fd_socket, tx_buf_eth, current_size_rx_eth_parsl+8,0);

 	// Временно принтим потом убрать
 	if(ret == current_size_rx_eth_parsl+8){
 		printf(" Parcel answer send size: %d\n", current_size_rx_eth_parsl+8 );
 	}else{
 		printf(" Error parcel answer send\n" );
 		return -1;
 	}

 	unsigned int k, size_buf_eth;
 	size_buf_eth = (( current_size_rx_eth_parsl/6) + 1 );

 	for(j = 1, i= 0, k=0; j < size_buf_eth ; j++ ){

 		printf(" Eth Command -- Answer: ");

 		for ( ; i < j*6 ; i++){
 			printf("0x%.2X ", rx_buf_eth[i]);
 		}

 		printf("-- ");

 		for ( ; k < j*6 ; k++){
 			printf("0x%.2X ", tx_buf_eth[k+8]);
 		}

 		printf("\n");
 	}
 ////////////////////////////////////////////////////////////////////////////
 	return 0;
 }

