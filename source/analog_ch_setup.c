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
#include "../include/SPI_SS.h"
#include "../include/Config_pars.h"
#include "../include/analog_ch_setup.h"
#include "../include/M41T64_Clock.h"

#define size_spi_tx_rx_buf_u16 2

 int write_a_ch_start_stop (int fd_SPI, uint16_t gpio_spi_cs, uint16_t gpio_spi_int, struct settings_ch *settings_channel,  uint8_t number_channel){

	uint16_t tx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	uint16_t rx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	int ret;

	if(settings_channel[number_channel].state == 0 ){ //State stop
		tx_buf[0]=0x2104;
	}else if( settings_channel[number_channel].state == 1 ){ //State start
		tx_buf[0]=0x2101;
 	}

	tx_buf[1]=0x0000;

	ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_spi_cs, gpio_spi_int, tx_buf,rx_buf, sizeof(tx_buf), 0 );

	if ( ret == -1 || rx_buf[0] != 0x0001){
		//сделать запись в лог файл
		printf("Ch%d send command START/STOP : FOULT  SPI error \n", number_channel+1 );
		return -1;
	}

	return 0;

 }

 int write_a_ch_input_switch(int fd_SPI, uint16_t gpio_spi_cs, uint16_t gpio_spi_int, struct settings_ch *settings_channel, uint8_t number_channel){

	uint16_t tx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	uint16_t rx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	int ret;

	// Write Input switching setting
	if(settings_channel[number_channel].config_ch.input==inp_1_1){
		tx_buf[0]=0x0100;
	}else if(settings_channel[number_channel].config_ch.input==inp_1_10){
		tx_buf[0]=0x0101;
	}else if(settings_channel[number_channel].config_ch.input==inp_0V){
		tx_buf[0]=0x0102;
	}else if(settings_channel[number_channel].config_ch.input==inp_cal){
		tx_buf[0]=0x0103;
	}else if(settings_channel[number_channel].config_ch.input==z_state){
		tx_buf[0]=0x0104;
	}else{
		printf("Ch%d setup Input switch: FOULT argument invalid \n", number_channel+1 );
		return -1;
	}
	tx_buf[1]=0x0000;

	ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_spi_cs, gpio_spi_int, tx_buf,rx_buf, sizeof(tx_buf), 0 );

	if ( ret == -1 || rx_buf[0] != 0x0001){
		//сделать запись в лог файл
		printf("Ch%d setup Input switch: FOULT  SPI error \n", number_channel+1 );
		return -1;
	}

	//сделать запись  в лог файл
	printf("Ch%d setup Input switch: OK\n", number_channel+1);

	return 0;
 }

 int write_a_ch_amplifier_ku1(int fd_SPI, uint16_t gpio_spi_cs, uint16_t gpio_spi_int, struct settings_ch *settings_channel, uint8_t number_channel){

	uint16_t tx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	uint16_t rx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	int ret;

	if( settings_channel[number_channel].config_ch.ku1 == 1 ){
		tx_buf[0]=0x0300;
	}else if( settings_channel[number_channel].config_ch.ku1 == 2 ){
		tx_buf[0]=0x0301;
	}else if( settings_channel[number_channel].config_ch.ku1 == 10 ){
		tx_buf[0]=0x0302;
	}else if( settings_channel[number_channel].config_ch.ku1 == 20 ){
		tx_buf[0]=0x0303;
	}else if( settings_channel[number_channel].config_ch.ku1 == 40 ){
		tx_buf[0]=0x0304;
	}else if( settings_channel[number_channel].config_ch.ku1 == 80 ){
		tx_buf[0]=0x0305;
	}else if( settings_channel[number_channel].config_ch.ku1 == 120 ){
		tx_buf[0]=0x0306;
	}else if( settings_channel[number_channel].config_ch.ku1 == 180 ){
		tx_buf[0]=0x0307;
	}else if( settings_channel[number_channel].config_ch.ku1 == 251 ){
		tx_buf[0]=0x0308;
	}else{
		printf("Ch%d setup gain of the amplifier KU1: FOULT argument invalid \n", number_channel+1 );
		return -1;
	}

	tx_buf[1]=0x0000;

	ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_spi_cs, gpio_spi_int, tx_buf,rx_buf, sizeof(tx_buf), 0 );

	if ( ret == -1 || rx_buf[0] != 0x0001){
		//сделать запись в лог файл
		printf("Ch%d setup gain of the amplifier KU1: FOULT SPI error\n", number_channel+1 );
		return -1;
	}

	//сделать запись  в лог файл
	printf("Ch%d setup gain of the amplifier KU1: OK\n", number_channel+1);

	return 0;
 }

 int write_a_ch_cutoff_freq_LPF(int fd_SPI, uint16_t gpio_spi_cs, uint16_t gpio_spi_int, struct settings_ch *settings_channel, uint8_t number_channel){

	uint16_t tx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	uint16_t rx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	int ret;
 //Write The cutoff frequency of the LPF

	if( settings_channel[number_channel].config_ch.fcut == 10 ){
		tx_buf[0]=0x0500;
	}else if( settings_channel[number_channel].config_ch.fcut == 20 ){
		tx_buf[0]=0x0501;
	}else if( settings_channel[number_channel].config_ch.fcut == 30 ){
		tx_buf[0]=0x0502;
	}else if( settings_channel[number_channel].config_ch.fcut == 40 ){
		tx_buf[0]=0x0503;
	}else if( settings_channel[number_channel].config_ch.fcut == 50 ){
		tx_buf[0]=0x0504;
	}else if(settings_channel[number_channel].config_ch.fcut == 60 ){
		tx_buf[0]=0x0505;
	}else if( settings_channel[number_channel].config_ch.fcut == 70 ){
		tx_buf[0]=0x0506;
	}else if( settings_channel[number_channel].config_ch.fcut == 80 ){
		tx_buf[0]=0x0507;
	}else if( settings_channel[number_channel].config_ch.fcut == 90 ){
		tx_buf[0]=0x0508;
	}else if( settings_channel[number_channel].config_ch.fcut == 130 ){
		tx_buf[0]=0x0509;
	}else if( settings_channel[number_channel].config_ch.fcut == 180 ){
		tx_buf[0]=0x050A;
	}else if( settings_channel[number_channel].config_ch.fcut == 230 ){
		tx_buf[0]=0x050B;
	}else if( settings_channel[number_channel].config_ch.fcut == 330 ){
		tx_buf[0]=0x050C;
	}else if( settings_channel[number_channel].config_ch.fcut == 430 ){
		tx_buf[0]=0x050D;
	}else if( settings_channel[number_channel].config_ch.fcut == 530 ){
		tx_buf[0]=0x050E;
	}else if( settings_channel[number_channel].config_ch.fcut == 630 ){
		tx_buf[0]=0x050F;
	}else if( settings_channel[number_channel].config_ch.fcut == 730 ){
		tx_buf[0]=0x0510;
	}else if( settings_channel[number_channel].config_ch.fcut == 830 ){
		tx_buf[0]=0x0511;
	}else if( settings_channel[number_channel].config_ch.fcut == 930 ){
		tx_buf[0]=0x0512;
	}else if( settings_channel[number_channel].config_ch.fcut == 1030 ){
		tx_buf[0]=0x0513;
	}else{
		printf("Ch%d setup cutoff frequency of the LPF Fcut: FOULT argument invalid \n", number_channel+1 );
		return -1;
	}

	tx_buf[1]=0x0000;

	ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_spi_cs, gpio_spi_int, tx_buf,rx_buf, sizeof(tx_buf), 0 );

	if ( ret == -1 || rx_buf[0] != 0x0001){
		//сделать запись в лог файл
		printf("Ch%d setup cutoff frequency of the LPF Fcut: FOULT SPI error\n", number_channel+1 );
		return -1;
	}

	//сделать запись  в лог файл
	printf("Ch%d setup cutoff frequency of the LPF Fcut: OK\n", number_channel+1);

	return 0;
 }

 int write_a_ch_amplifier_ku2(int fd_SPI, uint16_t gpio_spi_cs, uint16_t gpio_spi_int, struct settings_ch *settings_channel, uint8_t number_channel){

	uint16_t tx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	uint16_t rx_buf[size_spi_tx_rx_buf_u16] = { 0 };
	int ret;
 //Write amplifier KU2
	if( settings_channel[number_channel].config_ch.ku2 == 1 ){
		tx_buf[0]=0x0700;
	}else if( settings_channel[number_channel].config_ch.ku2 == 2 ){
		tx_buf[0]=0x0701;
	}else if( settings_channel[number_channel].config_ch.ku2 == 4 ){
		tx_buf[0]=0x0702;
	}else if( settings_channel[number_channel].config_ch.ku2 == 8 ){
		tx_buf[0]=0x0703;
	}else if( settings_channel[number_channel].config_ch.ku2 == 16 ){
		tx_buf[0]=0x0704;
	}else if( settings_channel[number_channel].config_ch.ku2 == 32 ){
		tx_buf[0]=0x0705;
	}else if( settings_channel[number_channel].config_ch.ku2 == 64 ){
		tx_buf[0]=0x0706;
	}else if( settings_channel[number_channel].config_ch.ku2 == 128 ){
		tx_buf[0]=0x0707;
	}else if( settings_channel[number_channel].config_ch.ku2 == 256 ){
		tx_buf[0]=0x0708;
	}else if( settings_channel[number_channel].config_ch.ku2 == 512 ){
		tx_buf[0]=0x0709;
	}else if( settings_channel[number_channel].config_ch.ku2 == 1024 ){
		tx_buf[0]=0x070A;
	}else if( settings_channel[number_channel].config_ch.ku2 == 2048 ){
		tx_buf[0]=0x070B;
	}else if( settings_channel[number_channel].config_ch.ku2 == 4096 ){
		tx_buf[0]=0x070C;
	}else{
		printf("Ch%d setup gain of the amplifier KU2: FOULT argument invalid \n", number_channel+1 );
		return -1;
	}

	tx_buf[1]=0x0000;

	ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_spi_cs, gpio_spi_int, tx_buf,rx_buf, sizeof(tx_buf), 0 );

	if ( ret == -1 || rx_buf[0] != 0x0001){
		//сделать запись в лог файл
		printf("Ch%d setup gain of the amplifier KU2: FOULT SPI error\n", number_channel+1 );
		return -1;
	}

	//сделать запись  в лог файл
	printf("Ch%d setup gain of the amplifier KU2: OK\n", number_channel+1);

	return 0;
 }


 int write_a_ch_sampling_freq_Fd(int fd_SPI, uint16_t gpio_spi_cs, uint16_t gpio_spi_int, struct settings_ch *settings_channel, uint8_t number_channel){

 	uint16_t tx_buf[size_spi_tx_rx_buf_u16] = { 0 };
 	uint16_t rx_buf[size_spi_tx_rx_buf_u16] = { 0 };
 	int ret;
 //Sampling frequency Fd
 	if( settings_channel[number_channel].config_ch.fd == 64 ){
 		tx_buf[0]=0x0900;
 	}else if( settings_channel[number_channel].config_ch.fd == 128 ){
 		tx_buf[0]=0x0901;
 	}else if( settings_channel[number_channel].config_ch.fd == 256 ){
 		tx_buf[0]=0x0902;
 	}else if( settings_channel[number_channel].config_ch.fd == 512 ){
 		tx_buf[0]=0x0903;
 	}else if( settings_channel[number_channel].config_ch.fd == 1024 ){
 		tx_buf[0]=0x0904;
 	}else if( settings_channel[number_channel].config_ch.fd == 2048 ){
 		tx_buf[0]=0x0905;
 	}else if( settings_channel[number_channel].config_ch.fd == 4096 ){
 		tx_buf[0]=0x0906;
 	}else{
 		printf("Ch%d setup sampling frequency Fd: FOULT argument invalid \n", number_channel+1 );
 		return -1;
 	}

 	tx_buf[1]=0x0000;

 	ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_spi_cs, gpio_spi_int, tx_buf,rx_buf, sizeof(tx_buf), 0 );

 	if ( ret == -1 || rx_buf[0] != 0x0001){
 		//сделать запись в лог файл
 		printf("Ch%d setup sampling frequency Fd: FOULT SPI error\n", number_channel+1 );
 		return -1;
 	}
 	//сделать запись  в лог файл
 	printf("Ch%d setup sampling frequency Fd: OK\n", number_channel+1);

 	return 0;
 }


 int  write_a_ch_decimation_Fres(int fd_SPI, uint16_t gpio_spi_cs, uint16_t gpio_spi_int, struct settings_ch *settings_channel, uint8_t number_channel){

 	uint16_t tx_buf[size_spi_tx_rx_buf_u16] = { 0 };
 	uint16_t rx_buf[size_spi_tx_rx_buf_u16] = { 0 };
 	int ret;
 //Sampling frequency Fd
 	if( settings_channel[number_channel].config_ch.fres == 1 ){
 		tx_buf[0]=0x3100;
 	}else if( settings_channel[number_channel].config_ch.fres == 2 ){
 		tx_buf[0]=0x3101;
 	}else if( settings_channel[number_channel].config_ch.fres == 4 ){
 		tx_buf[0]=0x3102;
 	}else if( settings_channel[number_channel].config_ch.fres == 8 ){
 		tx_buf[0]=0x3103;
 	}else if( settings_channel[number_channel].config_ch.fres == 16 ){
 		tx_buf[0]=0x3104;
 	}else if( settings_channel[number_channel].config_ch.fres == 32 ){
 		tx_buf[0]=0x3105;
 	}else if( settings_channel[number_channel].config_ch.fres == 64 ){
 		tx_buf[0]=0x3106;
 	}else{
 		printf("Ch%d setup software decimation Fres:: FOULT argument invalid \n", number_channel+1 );
 		return -1;
 	}

 	tx_buf[1]=0x0000;

 	ret = spi_transfer_command_analog_ch ( fd_SPI, gpio_spi_cs, gpio_spi_int, tx_buf,rx_buf, sizeof(tx_buf), 0 );

 	if ( ret == -1 || rx_buf[0] != 0x0001){
 		//сделать запись в лог файл
 		printf("Ch%d setup software decimation Fres:: FOULT SPI error\n", number_channel+1 );
 		return -1;
 	}
 	//сделать запись  в лог файл
 	printf("Ch%d setup sampling frequency Fd: OK\n", number_channel+1);

 	return 0;
 }


 int parse_sent_settings (int fd_SPI, struct settings_ch *settings_old, struct settings_ch *settings_new, uint16_t size_settings, uint8_t compare_settings, uint8_t quantity_channels){

          uint8_t apply_settings[ quantity_channels ];
          uint8_t i, number_ch;
         // uint16_t tx_buf[size_spi_tx_rx_buf_u16] = { 0 };
         // uint16_t rx_buf[size_spi_tx_rx_buf_u16] = { 0 };
          gpio_name_output_pin gpio_pin_res;
          gpio_name_output_pin gpio_pin_spi_cs;
          gpio_name_output_pin gpio_pin_spi_int;
          gpio_name_output_pin gpio_pin_res_array[] = { GPIO_SPI_Reset_Ch1, GPIO_SPI_Reset_Ch2, GPIO_SPI_Reset_Ch3 };
          gpio_name_output_pin gpio_pin_spi_cs_array[] = { GPIO_SPI_CS_Ch1, GPIO_SPI_CS_Ch2, GPIO_SPI_CS_Ch3 };
          gpio_name_output_pin gpio_pin_spi_int_array[] = { GPIO_SPI_INT_Ch1, GPIO_SPI_INT_Ch2, GPIO_SPI_INT_Ch3 };
          int ret;

          if( compare_settings == 0 ){


        	  for ( i=0, number_ch=0  ; i < quantity_channels; i++, number_ch++ ){

        		  gpio_pin_res = gpio_pin_res_array[number_ch];
        		  gpio_pin_spi_cs = gpio_pin_spi_cs_array[number_ch];
        		  gpio_pin_spi_int = gpio_pin_spi_int_array[number_ch];

        		  //Check on/off channel
        		  if ( settings_old[number_ch].mode == 1 ){
        			  enable_analog_channel (gpio_pin_res);
        			  //сделать запись в лог файл
        			  //sleep(1);//wait until STM32F103 in analog channel loaded
        			  printf("\nCh%d Mode: ON\n", number_ch+1 );
        		  }else{
        			  disable_analog_channel (gpio_pin_res);
        			  printf("\nCh%d Mode: OFF\n", number_ch+1 );
        			  continue;
        		  }

          // Write Input switching setting
        		  ret = write_a_ch_input_switch(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
        		  if(ret != 0){
        			  return -1;
        		  }

       	  //Write amplifier KU1
        		  ret = write_a_ch_amplifier_ku1(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
        		  if(ret != 0){
        			  return -1;
        		  }

   		  //Write The cutoff frequency of the LPF
        		  ret = write_a_ch_cutoff_freq_LPF(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
        		  if(ret != 0){
        			  return -1;
        		  }


       	  //Write amplifier KU2
        		  ret = write_a_ch_amplifier_ku2(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
        		  if(ret != 0){
        			  return -1;
        		  }

         //Sampling frequency Fd

        		  //Note Not use all the time set 4096Hz in the main
        		  if(settings_old[number_ch].config_ch.fd != 4096){
        			  printf("Ch%d setup sampling frequency Fd: No correct value\n", number_ch+1 );
        			  return -1;
        		  }
        		  /* ret = write_a_ch_sampling_freq_Fd(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
        		  if(ret != 0){
        			  return -1;
        		  }
        		  ret = M41T64_set_SQW_clock (I2C_BUS_NUMBER, settings_old[number_ch].config_ch.fd, ADDR_I2C_SLAVE_M41T64);
        		  if(ret != 0){
        			  return -1;
        		  } */

        // Software decimation Fres:
        		  ret = write_a_ch_decimation_Fres(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
        		  if(ret != 0){
        			  return -1;
        		  }


        		  //ДОПИСАТЬ ОСТАЛЬНЫЕ КОМАНДЫ

        		  // Send state: Start
        		  if ( settings_old[number_ch].state == 1 ){

        			  write_a_ch_start_stop (fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old,  number_ch);
        			  if(ret != 0){
        				  return -1;
        			  }
        			  printf("Ch%d state: START\n", number_ch+1);
        			  //сделать запись  в лог файл
        		  }else{
        			  printf("Ch%d state: STOP\n", number_ch+1);
        			  //сделать запись  в лог файл
        		  }

        	  }

          } else if( compare_settings == 1 ){

        	  printf("\n--------------------------Reconfigure analog channels.--------------------------\n" );

        	  for ( i=0, number_ch=0  ; i < quantity_channels; i++, number_ch++ ){

        		  gpio_pin_res = gpio_pin_res_array[number_ch];
        		  gpio_pin_spi_cs = gpio_pin_spi_cs_array[number_ch];
        		  gpio_pin_spi_int = gpio_pin_spi_int_array[number_ch];
        		  printf("\n");

        		  if( memcmp(&settings_old[number_ch], &settings_new[number_ch], size_settings) ==0) {
        			  //сделать запись в лог файл
        			  printf("\nCh%d The same settings as before.\n", number_ch+1 );
        			 continue;
        		  }

        		  //Check on/off channel

        		  if ( settings_new[number_ch].mode == 1 && settings_old[number_ch].mode == 0 ){
        			  enable_analog_channel (gpio_pin_res);
        			  //Write to the old configuration structure in the end of the cycle for
        			  //сделать запись в лог файл
        			  //sleep(1);//wait until STM32F103 in analog channel loaded
        			  printf("\nCh%d Mode: ON\n", number_ch+1 );
        		  }else if( settings_new[number_ch].mode == 0){
        			  disable_analog_channel (gpio_pin_res);
        			  settings_old[number_ch].mode = settings_new[number_ch].mode;
        			  //сделать запись в лог файл
        			  printf("\nCh%d Mode: OFF\n", number_ch+1 );
        			  continue;
        		  }
        		  if(settings_old[number_ch].mode == 1){
        			  printf("\nCh%d Mode: ON\n", number_ch+1 );
        		  }

        		  // Send state: Stop
        		  if ( settings_old[number_ch].state == 1 && settings_old[number_ch].mode == 1 ){//Channel was start and ON
        			  settings_old[number_ch].state = 0; // write stop state to old settings
        			  write_a_ch_start_stop (fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old,  number_ch);//send stop state to send settings to channel
        			  if(ret != 0){
        				  return -1;
        			  }
        		  }

        		  // Write Input switching setting
				  if(settings_old[number_ch].config_ch.input != settings_new[number_ch].config_ch.input || settings_old[number_ch].mode == 0 ){

					  settings_old[number_ch].config_ch.input = settings_new[number_ch].config_ch.input;

					  ret = write_a_ch_input_switch(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
					  if(ret != 0){
						  return -1;
					  }
				  }

				  //Write amplifier KU1
				  if(settings_old[number_ch].config_ch.ku1 != settings_new[number_ch].config_ch.ku1 || settings_old[number_ch].mode == 0){

					  settings_old[number_ch].config_ch.ku1 = settings_new[number_ch].config_ch.ku1;

					  ret = write_a_ch_amplifier_ku1(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
					  if(ret != 0){
						  return -1;
					  }
				  }

				  //Write The cutoff frequency of the LPF
				  if(settings_old[number_ch].config_ch.fcut != settings_new[number_ch].config_ch.fcut || settings_old[number_ch].mode == 0){

					  settings_old[number_ch].config_ch.fcut = settings_new[number_ch].config_ch.fcut;

					  ret = write_a_ch_cutoff_freq_LPF(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
					  if(ret != 0){
						  return -1;
					  }
				  }

				  //Write amplifier KU2
				  if(settings_old[number_ch].config_ch.ku2 != settings_new[number_ch].config_ch.ku2 || settings_old[number_ch].mode == 0){

					  settings_old[number_ch].config_ch.ku2 = settings_new[number_ch].config_ch.ku2;

					  ret = write_a_ch_amplifier_ku2(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
					  if(ret != 0){
						  return -1;
					  }
				  }

				  //Sampling frequency Fd
				  //Note Not use all the time set 4096Hz in the main
				  if(settings_new[number_ch].config_ch.fd != 4096){
					  printf("Ch%d setup sampling frequency Fd: No correct value\n", number_ch+1 );
					  return -1;
				  }

				  /*if(settings_old[number_ch].config_ch.fd != settings_new[number_ch].config_ch.fd || settings_old[number_ch].mode == 0){

					  settings_old[number_ch].config_ch.fd = settings_new[number_ch].config_ch.fd;

					  ret = write_a_ch_sampling_freq_Fd(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
					  if(ret != 0){
						  return -1;
					  }
					  ret = M41T64_set_SQW_clock(I2C_BUS_NUMBER, settings_old[number_ch].config_ch.fd, ADDR_I2C_SLAVE_M41T64);
					  if(ret != 0){
						  return -1;
					  }
				  }*/


				  // Software decimation Fres
				  if(settings_old[number_ch].config_ch.fres != settings_new[number_ch].config_ch.fres || settings_old[number_ch].mode == 0){

					  settings_old[number_ch].config_ch.fres = settings_new[number_ch].config_ch.fres;

					  ret = write_a_ch_decimation_Fres(fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old, number_ch);
					  if(ret != 0){
						  return -1;
					  }
				  }


				  //ДОПИСАТЬ ОСТАЛЬНЫЕ КОМАНДЫ

				  //Save mode from new to old settings
				  settings_old[number_ch].mode = settings_new[number_ch].mode;

				  // Send state: Start
			      if ( settings_new[number_ch].state == 1 ){
			    	  settings_old[number_ch].state = settings_new[number_ch].state;
			    	  write_a_ch_start_stop (fd_SPI, gpio_pin_spi_cs, gpio_pin_spi_int, settings_old,  number_ch);
			    	  if(ret != 0){
			    		  return -1;
			    	  }
			    	  printf("Ch%d state: START\n", number_ch+1);
			    	  //сделать запись  в лог файл
			      }else{
			    	  printf("Ch%d state: STOP\n", number_ch+1);
			    	  //сделать запись  в лог файл
			      }

        	  }

          }
          return 0;

    }

