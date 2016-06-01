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
#include "../include/M41T64_Clock.h"
#include "../include/Eth_fn.h"
#include "../include/main.h"


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


int pars_eth_coomand_for_BB( int fd_SPI,unsigned int fd_I2C , uint8_t num_command,  uint16_t *tx_buf,  uint16_t *rx_buf, uint16_t size ){

	int ret, ret1 ,ret2;
	gpio_name_output_pin gpio_pin_res;
	gpio_name_output_pin gpio_pin_res_array[] = { 0x00, GPIO_SPI_Reset_Ch1, GPIO_SPI_Reset_Ch2, GPIO_SPI_Reset_Ch3 };
	uint16_t tmp_tx_buf[2];
	tmp_tx_buf[0] = tx_buf[0];
	tmp_tx_buf[1] = tx_buf[1];


	switch (num_command){

		case 0x22: // Stop command
			ret = spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch1, GPIO_SPI_INT_Ch1, tmp_tx_buf,rx_buf, size, 0 );
		 	ret1 = spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch2, GPIO_SPI_INT_Ch2, tmp_tx_buf,rx_buf, size, 0 );
		 	ret2 = spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch3, GPIO_SPI_INT_Ch3, tmp_tx_buf,rx_buf, size, 0 );

		 	if(ret == -1 || ret1 == -1 || ret2 == -1  ){
		 		return -1;
		 	}
		 	break;

		case 0x21: // Start Sync Enable
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			ret = setup_time_date_a_ch (fd_SPI, fd_I2C);
			if (ret == -1 ){
				tmp_tx_buf[0] = 0x2200;
				tmp_tx_buf[1] = 0x0000;
				spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch1, GPIO_SPI_INT_Ch1, tmp_tx_buf,rx_buf, size, 0 );
				spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch2, GPIO_SPI_INT_Ch2, tmp_tx_buf,rx_buf, size, 0 );
				spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch3, GPIO_SPI_INT_Ch3, tmp_tx_buf,rx_buf, size, 0 );
				return -1;
			}

			if( tmp_tx_buf[0] == 0x2101 ){
				gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , HIGHT);
				gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , LOW);
				printf(" Enable Sync\n");

			}else if(tmp_tx_buf[0] == 0x2108 ){
				//TO DO дописать запуск по времени
			}else{
				return -1;
			}
			break;

		case 0x2E:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			// TO DO записать состояние канала в структуру настроек
			if(((tmp_tx_buf[0]) == 0x2E01) || ((tmp_tx_buf[0]) == 0x2E02) || ((tmp_tx_buf[0]) == 0x2E03)){

				gpio_pin_res = gpio_pin_res_array[((uint8_t)tmp_tx_buf[0])];

				if(tmp_tx_buf[1] == 0x0001){ //Enable channel
					enable_analog_channel (gpio_pin_res);
				}else {//Disable Channel
					disable_analog_channel (gpio_pin_res);
				}
			}else{
				return -1;
			}
			break;
		///Заглушка
		case 0x2F:
			if( tmp_tx_buf[0] == 0x2F01){
				rx_buf[0] = 0x2F01;
				rx_buf[1] = 0x0001;
			}else if( tmp_tx_buf[0] == 0x2F02 ){
				rx_buf[0] = 0x2F02;
				rx_buf[1] = 0x0001;
			}else if( tmp_tx_buf[0] == 0x2F03 ){
				rx_buf[0] = 0x2F03;
				rx_buf[1] = 0x0001;
			}else{
				return -1;
			}
			break;
	    ////
        ///Заглушка
		case 0x19: // Дописать запоминания серийного номер на BB
			ret = spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch1, GPIO_SPI_INT_Ch1, tmp_tx_buf,rx_buf, size, 0 );
		 	ret1 = spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch2, GPIO_SPI_INT_Ch2, tmp_tx_buf,rx_buf, size, 0 );
		 	ret2 = spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch3, GPIO_SPI_INT_Ch3, tmp_tx_buf,rx_buf, size, 0 );

		 	if(ret == -1 || ret1 == -1 || ret2 == -1  ){
		 		return -1;
		 	}
			break;
		case 0x1A: // Дописать запоминания серийного номер на BB
			spi_transfer_command_analog_ch ( fd_SPI, GPIO_SPI_CS_Ch1, GPIO_SPI_INT_Ch1, tmp_tx_buf,rx_buf,size, 0 );
			break;
        //////

		case 0x35:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			ret = set_ml_seconds_M41T64(fd_I2C,(uint8_t) tmp_tx_buf[0], ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}
			break;

		case 0x36:
			ret = get_ml_seconds_M41T64(fd_I2C, ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}else{
				rx_buf[0] = 0x3600|(uint8_t)ret;
				rx_buf[1] = 0x0000;
			}
			break;

		case 0x37:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			ret = set_seconds_M41T64(fd_I2C,(uint8_t) tmp_tx_buf[0], ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}
			break;

		case 0x38:
			ret = get_seconds_M41T64(fd_I2C, ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}else{
				rx_buf[0] = 0x3800|(uint8_t)ret;
				rx_buf[1] = 0x0000;
			}
			break;

		case 0x39:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			ret = set_minutes_M41T64(fd_I2C,(uint8_t) tmp_tx_buf[0], ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}
			break;

		case 0x3A:
			ret = get_minutes_M41T64(fd_I2C, ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}else{
				rx_buf[0] = 0x3A00|(uint8_t)ret;
				rx_buf[1] = 0x0000;
			}
			break;

		case 0x3B:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			ret = set_hours_M41T64(fd_I2C,(uint8_t) tmp_tx_buf[0], ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}
			break;

		case 0x3C:
			ret = get_hours_M41T64(fd_I2C, ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}else{
				rx_buf[0] = 0x3C00|(uint8_t)ret;
				rx_buf[1] = 0x0000;
			}
			break;

		case 0x3D:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			ret = set_day_M41T64(fd_I2C,(uint8_t) tmp_tx_buf[0], ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}
			break;

		case 0x3E:
			ret = get_day_M41T64(fd_I2C, ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}else{
				rx_buf[0] = 0x3E00|(uint8_t)ret;
				rx_buf[1] = 0x0000;
			}
			break;

		case 0x3F:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			ret = set_century_month_M41T64(fd_I2C,(uint8_t) tmp_tx_buf[0], ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}
			break;

		case 0x40:
			ret = get_century_month_M41T64(fd_I2C, ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}else{
				rx_buf[0] = 0x4000|(uint8_t)ret;
				rx_buf[1] = 0x0000;
			}
			break;

		case 0x41:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			ret = set_year_M41T64(fd_I2C,(uint8_t) tmp_tx_buf[0], ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}
			break;

		case 0x42:
			ret = get_year_M41T64(fd_I2C, ADDR_I2C_SLAVE_M41T64);
			if(ret == -1){
				return -1;
			}else{
				rx_buf[0] = 0x4200|(uint8_t)ret;
				rx_buf[1] = 0x0000;
			}
			break;

		///Заглушки. Дописать по мере реализации

		case 0x18:
			rx_buf[0] = 0x1800;
			rx_buf[1] = 0x0000;
			break;
		case 0x27:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x2A:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x2B:
			rx_buf[0] = 0x2B03;
			rx_buf[1] = 0x0000;
			break;
		case 0x2C:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x2D:
			rx_buf[0] = 0x2D02;
			rx_buf[1] = 0x0000;
			break;
		case 0x43:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x44:
			rx_buf[0] = 0x4400;
			rx_buf[1] = 0x0000;
			break;
		case 0x45:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x46:
			rx_buf[0] = 0x4600;
			rx_buf[1] = 0x0000;
			break;
		case 0x47:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x48:
			rx_buf[0] = 0x4800;
			rx_buf[1] = 0x0000;
			break;
		case 0x49:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x4A:
			rx_buf[0] = 0x4A00;
			rx_buf[1] = 0x0000;
			break;
		case 0x4B:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x4C:
			rx_buf[0] = 0x4C00;
			rx_buf[1] = 0x0000;
			break;
		case 0x4D:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x4E:
			rx_buf[0] = 0x4E00;
			rx_buf[1] = 0x0000;
			break;
		case 0x4F:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x50:
			rx_buf[0] = 0x5000;
			rx_buf[1] = 0x0000;
			break;
		case 0x51:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x52:
			rx_buf[0] = 0x5200;
			rx_buf[1] = 0x0000;
			break;
		case 0x53:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x54:
			rx_buf[0] = 0x5400;
			rx_buf[1] = 0x0000;
			break;
		case 0x55:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x56:
			rx_buf[0] = 0x5600;
			rx_buf[1] = 0x0000;
			break;
		case 0x57:
			rx_buf[0] = 0x0001;
			rx_buf[1] = 0x0000;
			break;
		case 0x58:
			rx_buf[0] = 0x5800;
			rx_buf[1] = 0x0000;
			break;

		default:
			return -1;
			break;
	}

	return 0;

}



int pars_eth_command_parcel(int fd_SPI,unsigned int fd_I2C, int fd_socket, uint8_t *rx_buf_eth,  uint8_t *tx_buf_eth, uint32_t *real_rx_tx_buf_eth_size, struct settings_ch *settings_channels){

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

 	gpio_name_output_pin gpio_pin_spi_cs;
 	gpio_name_output_pin gpio_pin_spi_int;
 	gpio_name_output_pin gpio_pin_spi_cs_array[] = { 0x00, GPIO_SPI_CS_Ch1, GPIO_SPI_CS_Ch2, GPIO_SPI_CS_Ch3 };
 	gpio_name_output_pin gpio_pin_spi_int_array[] = { 0x00, GPIO_SPI_INT_Ch1, GPIO_SPI_INT_Ch2, GPIO_SPI_INT_Ch3 };

 	//Read size parcel from Eth
 	ret = recv(fd_socket, rx_buf_eth, 4, 0);
 	if(ret <= 0){
 		return -1;
 	}

 	current_size_rx_eth_parsl = rx_buf_eth[0];
 	current_size_rx_eth_parsl = (current_size_rx_eth_parsl<<8)|(rx_buf_eth[1]);
 	current_size_rx_eth_parsl = (current_size_rx_eth_parsl<<8)|(rx_buf_eth[2]);
 	current_size_rx_eth_parsl = (current_size_rx_eth_parsl<<8)|(rx_buf_eth[3]);

#ifdef DEBUG_MODE
 	printf(" Size command parcel from Eth: %d\n",current_size_rx_eth_parsl );
#endif
    //Realloc  rx_buf_eth  for eth parcel if size parcel more than current size rx_buf_eth
 	if(current_size_rx_eth_parsl > (*real_rx_tx_buf_eth_size)){

 		*real_rx_tx_buf_eth_size = current_size_rx_eth_parsl+16; //+16 for type and size for tx to eth parcel
 		rx_buf_eth = (uint8_t*)realloc(rx_buf_eth, *real_rx_tx_buf_eth_size);
 		if (rx_buf_eth==NULL) {
 			free (rx_buf_eth);
 			//сделать запись в лог фаил
 			printf (" Error reallocating memory rx_buf_eth in eth fun /n");
 			exit (1);
 		}

 		tx_buf_eth = (uint8_t*)realloc(tx_buf_eth, *real_rx_tx_buf_eth_size);
 		if (tx_buf_eth==NULL) {
 			free (tx_buf_eth);
 			//сделать запись в лог фаил
 			printf (" Error reallocating memory tx_buf_ethin eth fun /n");
 			exit (1);
 		}

 	}

 	//Read receive command from eth
 	ret = recv(fd_socket, rx_buf_eth, current_size_rx_eth_parsl, 0);
 	if(ret <= 0){
 		return -1;
 	}

#ifdef DEBUG_MODE
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

#endif

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

 		tmp = rx_buf_eth[i] << 8;
 		i++;
 		number_channel = tmp | (rx_buf_eth[i]);
 	    i++;
 		number_command = rx_buf_eth[i];
 		tmp = rx_buf_eth[i] << 8;
 		i++;
 		tmp = tmp | rx_buf_eth[i];
 		tx_mk_buf[0] = tmp;
 		i++;
 		tmp = rx_buf_eth[i] << 8;
 		i++;
 		tmp = tmp | (rx_buf_eth[i]);
 		tx_mk_buf[1] = tmp;

 		gpio_pin_spi_cs = gpio_pin_spi_cs_array[number_channel];
 		gpio_pin_spi_int = gpio_pin_spi_int_array[number_channel];

 		if(number_channel == 0x0000){ // Command for BB

 			ret = pars_eth_coomand_for_BB( fd_SPI, fd_I2C, number_command,  tx_mk_buf,  rx_mk_buf, sizeof(tx_mk_buf));

 		}else{ // Command for analog channel

 			ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, tx_mk_buf,rx_mk_buf, sizeof(tx_mk_buf), 0 );
 		}

 		if ( ret == -1 ){
 			//сделать запись в лог файл
 			printf(" Ch%d SPI Eth data write ERROR\n", number_channel );
 			rx_mk_buf[0] = 0x0002;
 			rx_mk_buf[1] = 0x0000;
		}

 		tx_buf_eth[j] = (uint8_t) (number_channel>>8);
 		j++;
 		tx_buf_eth[j] = (uint8_t) number_channel;
 		j++;
 		tx_buf_eth[j] = (uint8_t) (rx_mk_buf[0]>>8);
 		j++;
 		tx_buf_eth[j] = (uint8_t) rx_mk_buf[0];
 		j++;
 		tx_buf_eth[j] = (uint8_t) (rx_mk_buf[1]>>8);
 		j++;
 		tx_buf_eth[j] = (uint8_t) rx_mk_buf[1];
 		j++;

 	}

 	//Write size of answer parcel
 	tx_buf_eth[4] = (uint8_t)(current_size_rx_eth_parsl>>24);
 	tx_buf_eth[5] = (uint8_t)(current_size_rx_eth_parsl>>16);
 	tx_buf_eth[6] = (uint8_t)(current_size_rx_eth_parsl>>8);
 	tx_buf_eth[7] = (uint8_t)current_size_rx_eth_parsl;

#ifdef DEBUG_MODE
 		printf(" Size answer parcel: %d+8\n", current_size_rx_eth_parsl );
#endif


 	//Sent parcel with answer to eth
 	ret = sendall(fd_socket, tx_buf_eth, current_size_rx_eth_parsl+8,0);

#ifdef DEBUG_MODE
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
#endif

 	return 0;
 }


int send_ADC_data_to_Eth( int fd_SPI_BB, uint8_t number_channels, int fd_socket, struct settings_ch *cfg ){

	int ret;
	uint8_t j;
	uint16_t i;
	gpio_name_output_pin gpio_pin_spi_cs_array[] = { GPIO_SPI_CS_Ch1, GPIO_SPI_CS_Ch2, GPIO_SPI_CS_Ch3 };
	gpio_name_output_pin gpio_pin_spi_int_array[] = { GPIO_SPI_INT_Ch1, GPIO_SPI_INT_Ch2, GPIO_SPI_INT_Ch3 };

	uint16_t tx_buf_status[2] = {0x2800,0x0000};
	uint16_t rx_buf_status[2] = {0x0000,0x0000};

	uint16_t tx_buf[8206] = {0x0000};
	uint16_t rx_buf[8206+4] = {};

	for( j = 0 ; j < number_channels; j++){

		//Pass disable channels
		if (cfg[j].mode != 1){
			continue;
		}

		ret = spi_transfer_command_analog_ch ( fd_SPI_BB, gpio_pin_spi_cs_array[j], gpio_pin_spi_int_array[j], tx_buf_status,rx_buf_status, sizeof(tx_buf_status), 0 );
		if(ret != 0){
			printf("Error SPI, function: send_ADC_data_to_Eth\n");
			continue;
		}

		if(rx_buf_status[0] == 0x2801){

#ifdef DEBUG_MODE
			printf("Channel %d\n", j+1 );
			printf("0x%.4X \n", rx_buf_status[0]);
#endif

		    ret = spi_read_data_ADC24 (fd_SPI_BB, gpio_pin_spi_cs_array[j], gpio_pin_spi_int_array[j], tx_buf, &rx_buf[4], rx_buf_status[1], 0);
		    if(ret != 0){
		    	printf("Error SPI, function: spi_read_data_ADC24\n");
		    }
		    rx_buf[0] = 0x00;
		    rx_buf[1] = 0x03;
		    rx_buf[2] = 0x00;
		    rx_buf[3] = rx_buf_status[1];

#ifdef DEBUG_MODE
		    for (i = 0; i < 20; i++){
		    	printf("0x%.4X ", rx_buf[i]);
		    }
		    printf("\n");

		    for (i = (((rx_buf_status[1]+8)/2)-20); i < ((rx_buf_status[1]+8)/2); i++){
		    	printf("0x%.4X ", rx_buf[i]);
		    }
		    printf("\n");

		    printf("size parsel %d\n", rx_buf_status[1]+8);
#endif

		    for(i=0; i < ((rx_buf_status[1]+8)/2); i++ ){

		    	rx_buf[i] = REVERSE_LE_BE_u16(rx_buf[i]);
		    }

		    ret = sendall(fd_socket, (uint8_t*)rx_buf, rx_buf_status[1]+8,0);
		    if(ret !=  rx_buf_status[1]+8){
		    	printf("Error: not all ADC data send to Eth\n");
		    	return -1;
		    }

#ifdef DEBUG_MODE
		    printf("All ADC data send to Eth\n");
#endif

		}
	}

	return 0;
}


