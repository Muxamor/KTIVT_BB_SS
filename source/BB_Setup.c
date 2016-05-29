/*
 * BB_Sutup.c
  *
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "../include/main.h"
#include "../include/GPIO_SS.h"
#include "../include/BB_Setup.h"



// Area for file descriptors of GPIO
unsigned int fd_GPIO_pin_output[ GPIO_out_MAX ];
unsigned int fd_GPIO_pin_input[ GPIO_in_MAX ];



/*********************************************
 * Default GPIO settings for BB
 ********************************************/

int Default_Setup_GPIO_BB (void){
	int i;
	int ret=0;

//Export GPIO to the Linux sys.
	for (i=0; i< GPIO_out_MAX; i++){
		ret=gpio_export(gpio_output_pin_numbers[i]);
		if (ret==-1){
			return -1;
		}

	}
	for (i=0; i< GPIO_in_MAX; i++){
		gpio_export(gpio_input_pin_numbers[i]);
		if (ret==-1){
			return -1;
		}
	}
	printf("GPIO  export - SUCCESS!\n");


//Set direction for GPIO to the Linux sys.
	for (i=0; i< GPIO_out_MAX; i++){
		gpio_set_direction(gpio_output_pin_numbers[i],OUTPUT_PIN);
		if (ret==-1){
			return -1;
		}
	}

	for (i=0; i< GPIO_in_MAX; i++){
		gpio_set_direction(gpio_input_pin_numbers[i],INPUT_PIN);
		if (ret==-1){
			return -1;
		}
	}
	printf("GPIO set direction - SUCCESS!\n");


/// Set edge to implementation interrupts for input GPIO
	for (i=0; i< GPIO_in_MAX; i++){
		gpio_set_edge(gpio_input_pin_numbers[i],"rising");
		if (ret==-1){
			return -1;
		}
	}
	printf("GPIO set edge - SUCCESS!\n");


//Open GPIO file to operate gpio.
	for (i=0; i< GPIO_out_MAX; i++){
		fd_GPIO_pin_output[i]=gpio_fd_open_R_W(gpio_output_pin_numbers[i]);
		if (ret==-1){
			return -1;
		}
	}

	for (i=0; i< GPIO_in_MAX; i++){
		fd_GPIO_pin_input[i]=gpio_fd_open_R_O(gpio_input_pin_numbers[i]);
		if (ret==-1){
			return -1;
		}
	}
	printf("Get FD - SUCCESS!\n");


// Set default value for output GPIO.
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Ch1] , HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Ch2] , HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Ch3] , HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Disp] , HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_CS_Col], HIGHT);
	gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3], LOW);// Прерывание сделать по спадающему фронту!!!!
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_Reset_Ch1],  HIGHT); //Disable analog channel 1
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_Reset_Ch2],  HIGHT); //Disable analog channel 1
	gpio_set_value(fd_GPIO_pin_output[GPIO_SPI_Reset_Ch3],  HIGHT); //Disable analog channel 1
	sleep(1);//wait until STM32F103 in analog channel loaded
	printf("Set default value gpio - SUCCESS!\n");


	return 0;
}

/*********************************************
 * Enable analog channel
 ********************************************/
int enable_analog_channel (gpio_name_output_pin  gpio_name){

		gpio_set_value(fd_GPIO_pin_output[gpio_name] , LOW);
		sleep(1);
	return 0;
}

/*********************************************
 * Disable analog channel
 ********************************************/
int disable_analog_channel (gpio_name_output_pin  gpio_name){

		gpio_set_value(fd_GPIO_pin_output[gpio_name] , HIGHT);

	return 0;
}

/*********************************************
 * Reset  analog channel
 ********************************************/
int reset_analog_channel (gpio_name_output_pin  gpio_name){

	gpio_set_value(fd_GPIO_pin_output[gpio_name] , HIGHT);
	sleep(1);
	gpio_set_value(fd_GPIO_pin_output[gpio_name] , LOW);

	return 0;
}




/*
int concole_command_parser (char *in_buffer, char size_in_buffer){

	char console_command [CONSOLE_MAS_SIZE] [12] = {
														"ch1", "ch2", "ch3", "ch_all","-p"//0,1,2,3,4
														"cfg", "reboot", "on", "off", "start", "stop", "default",//5,6,7,8,9,10,11
														"help"//12
													};

	int i;
	int lenght_in_buffer=0;
	int lenght_read_buffer=0;
	int number_of_channel=-1;
	int number_of_command=-1;
	char tmp_buffer[30];
	unsigned int send_buf[2];



	//Сalculate the length of the string
	for(i=0 ; in_buffer[i] != '\n'; i++);
	lenght_in_buffer = i;
	in_buffer[i]='\0';

	while(lenght_read_buffer <= lenght_in_buffer){

		for (; *in_buffer == ' ' && lenght_read_buffer <= lenght_in_buffer; in_buffer++, lenght_read_buffer++ ); // remove SPACE in recive string

		for (i=0; *in_buffer != ' '  && lenght_read_buffer <= lenght_in_buffer; in_buffer++, lenght_read_buffer++ ){

			tmp_buffer[i] = *in_buffer;
		}

		tmp_buffer[i]='\0';

		for (i=0; i!=CONSOLE_MAS_SIZE; i++){

			if(!strcmp((char const*)tmp_buffer, (char const*)console_command [i] )){

				number_of_command=i;
				break;
			}
		}

		if (number_of_command==-1){

			printf("Command \" %s \" not found \n", tmp_buffer);
			printf("Enter a command \" help \" to read the Help \n");

		}else{

			switch(number_of_command){

				case 0: //ch1
					number_of_channel=1;
					break;

				case 1://ch2
					number_of_channel=2;
					break;

				case 2://ch3
					number_of_channel=3;
					break;

				case 3://ch_all
					number_of_channel=0;
					break;

				case 4: //-p parcel

                    //First part of parsel
					if(number_of_channel==-1){
						printf("Do not enter the number of analog channel\n");
						printf("Enter a command \" help \" to read the Help \n");
						return -1;
					}

					for (; *in_buffer == ' ' && lenght_read_buffer <= lenght_in_buffer; in_buffer++, lenght_read_buffer++ ); // remove SPACE in recive string

					for (i=0; *in_buffer != ' '  && lenght_read_buffer <= lenght_in_buffer; in_buffer++, lenght_read_buffer++ ){

						tmp_buffer[i] = *in_buffer;
					}

					tmp_buffer[i]='\0';

					if( i==0 ){
						printf("Argument for \"-p\" not found \n");
						return -1;
					}
					if( tmp_buffer[0] != '0' && tmp_buffer[1] != 'x'){
						printf("Argument  %s for \"-p\" is not correct \n", tmp_buffer);
						break;
					}

					send_buf[0] = (unsigned int) strtol(tmp_buffer+2, NULL, 16); //+2 for remove 0x prefix

					 //Second part of parsel.
					for (; *in_buffer == ' ' && lenght_read_buffer <= lenght_in_buffer; in_buffer++, lenght_read_buffer++ ); // remove SPACE in recive string

					for (i=0; *in_buffer != ' '  && lenght_read_buffer <= lenght_in_buffer; in_buffer++, lenght_read_buffer++ ){

						tmp_buffer[i] = *in_buffer;
					}

					tmp_buffer[i]='\0';

					if( i==0 ){
						printf("Argument for \"-p\" not found \n");
						return -1;
					}

					if( tmp_buffer[0] != '0' && tmp_buffer[1] != 'x'){
						printf("Argument  %s for \"-p\" is not correct \n", tmp_buffer);
						break;
					}

					send_buf[1] = (unsigned int) strtol(tmp_buffer+2, NULL, 16); //+2 for remove 0x prefix
					//дописать отправку готовго буфера и обработку ответа
					break;


				case 5://cfg
					//прочитать всю конфигурацию

					break;

				case 6://reboot

					if(number_of_channel==-1){
						printf("Do not enter the number of analog channel\n");
						printf("Enter a command \" help \" to read the Help \n");
						return -1;
					}

					if (number_of_channel==1){
						reset_analog_channel(GPIO_SPI_Reset_Ch1);
						printf("Analog channel 1 -- OK\n");
					}else if (number_of_channel==2){
						reset_analog_channel(GPIO_SPI_Reset_Ch2);
						printf("Reboot analog channel 2 -- OK\n");
					}else if(number_of_channel==3){
						reset_analog_channel(GPIO_SPI_Reset_Ch3);
						printf("Reboot analog channel 3 -- OK\n");
					}else if(number_of_channel==0){
						reset_analog_channel(GPIO_SPI_Reset_Ch1);
						reset_analog_channel(GPIO_SPI_Reset_Ch2);
						reset_analog_channel(GPIO_SPI_Reset_Ch3);
						printf("Reboot analog channel 1,2,3 -- OK\n");
					}
					break;

				case 7://on

					if(number_of_channel==-1){
						printf("Do not enter the number of analog channel\n");
						printf("Enter a command \" help \" to read the Help \n");
						return -1;
					}

					if (number_of_channel==1){
						enable_analog_channel (GPIO_SPI_Reset_Ch1);
						printf("Analog channel 1 is ON\n");
					}else if (number_of_channel==2){
						enable_analog_channel (GPIO_SPI_Reset_Ch2);
						printf("Analog channel 2 is ON\n");
					}else if(number_of_channel==3){
						enable_analog_channel (GPIO_SPI_Reset_Ch3);
						printf("Analog channel 3 is ON\n");
					}else if(number_of_channel==0){
						enable_analog_channel (GPIO_SPI_Reset_Ch1);
						enable_analog_channel (GPIO_SPI_Reset_Ch2);
						enable_analog_channel (GPIO_SPI_Reset_Ch3);
						printf("Analog channel 1,2,3 is ON\n");
					}
					break;

				case 8://off

					if(number_of_channel==-1){
						printf("Do not enter the number of analog channel\n");
						printf("Enter a command \" help \" to read the Help \n");
						return -1;
					}

					if (number_of_channel==1){
						disable_analog_channel (GPIO_SPI_Reset_Ch1);
						printf("Analog channel 1 is OFF\n");
					}else if (number_of_channel==2){
						disable_analog_channel (GPIO_SPI_Reset_Ch2);
						printf("Analog channel 2 is OFF\n");
					}else if(number_of_channel==3){
						disable_analog_channel (GPIO_SPI_Reset_Ch3);
						printf("Analog channel 3 is OFF\n");
					}else if(number_of_channel==0){
						disable_analog_channel (GPIO_SPI_Reset_Ch1);
						disable_analog_channel (GPIO_SPI_Reset_Ch2);
						disable_analog_channel (GPIO_SPI_Reset_Ch3);
						printf("Analog channel 1,2,3 is OFF\n");
					}
					break;

				case 9://start
					break;

				case 10://stop
					break;

				case 11:// default
					break;

				case 12://help
					printf("*******************HELP*******************************\n");
					printf("ch1 - \n");
					printf("ch2 - \n");
					printf("ch3 - \n");
					printf("ch_all - \n");
					printf("-p x x -  \n");
					printf("cfg - \n");
					printf("reboot - \n");
					printf("cfg - \n");
					printf("on - \n");
					printf("off - \n");
					printf("start - \n");
					printf("stop - \n");
					printf("default - \n");
					printf("help - \n");
					break;

				default:
					printf("Command not recognized!");
					break;
			}
		}


	}



}*/
