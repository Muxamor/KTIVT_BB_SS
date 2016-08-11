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
#include "../include/main.h"
#include <linux/spi/spidev.h>
#include "../include/GPIO_SS.h"
#include "../include/SPI_SS.h"
#include "../include/BB_Setup.h"
#include "../include/Config_pars.h"
#include "../include/analog_ch_setup.h"
#include "../include/M41T64_Clock.h"
#include "../include/Eth_fn.h"


/*Для примера от Темыча */

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>

//#include <getopt.h>
//#include <sys/ioctl.h>
//#include <linux/types.h>
//#include <string.h>

//#include <unistd.h>
//#include <fcntl.h>

/*
static void pabort(const char *s){
	perror(s);
	abort();
}
  */


/****************************************************************
 *Main function
 ****************************************************************/
 int main(int argc, char **argv){

	char console_buffer[30];

	int ret=0;
    uint32_t i, j;


	ret = Default_Setup_GPIO_BB();
	if (ret==-1){
		perror("Default_Setup_GPIO_BB() - FAILUR");
		return EXIT_FAILURE;
	}

	//Open i2c bus 2 on the beaglebone black
	unsigned int fd_i2c;
	fd_i2c = i2c_device_open("/dev/i2c-1");

	ret = Defult_setup_M41T64(fd_i2c, ADDR_I2C_SLAVE_M41T64); //Set 4096 Hz
	if (ret==-1){
		perror("Defult_setup_M41T64() - FAILUR");
		return EXIT_FAILURE;
	}
	sleep(1);//To wait ADC ready

	//Open SPI bus on the beaglebone black
	int fd_SPI_BB;
	fd_SPI_BB = spi_device_open("/dev/spidev1.0");
	set_spi_settings(fd_SPI_BB, SPI_MODE_1, 0, 16, 18000000);

	//Read file configuration and pars
    struct settings_ch cfg_ch_old[3] = {0};
    struct settings_ch cfg_ch_new[3] = {0};

    struct settings_brd cfg_brd_old;
    struct settings_brd cfg_brd_new;
    memset(&cfg_brd_old, 0, sizeof(cfg_brd_old));
    memset(&cfg_brd_new, 0, sizeof(cfg_brd_new));


    FILE *fd_config_file = fopen("/kti_bb_ss/KTIVT_BB_SS.conf", "r");
    if(fd_config_file==NULL){
    	//сделать запись в лог фаил
    	printf("Configure file is not found\n");
    	return EXIT_FAILURE;
    }

    // read configure file
    ret = parse_config(fd_config_file, &cfg_brd_old, cfg_ch_old, 3);
    fclose(fd_config_file);
	if (ret==-1){
		perror("Config file no correct"); // сдлеать запись в лог
		return EXIT_FAILURE;
	}

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

    //set settings at analog channel
    //дописать команду синк!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    ret = parse_set_settings_analog_ch(fd_SPI_BB, cfg_ch_old, cfg_ch_new, sizeof(cfg_ch_old[0]) , NO_COMPARE_SETTINGS, 3);
	if (ret==-1){
		perror("parse_sent_settings() - FAILUR");
		return EXIT_FAILURE;
	}//дописать команду синк

/*
	ret = parse_sent_settings (fd_SPI_BB, cfg_ch_old, cfg_ch_new, sizeof(cfg_ch_old[0]) ,1, 3);
	if (ret==-1){
		perror("parse_sent_settings() - FAILUR");
		return EXIT_FAILURE;
	}

    int testmemcmp;

    testmemcmp = memcmp(&cfg_ch_old[0], &cfg_ch_new[0], sizeof(cfg_ch_old[1]) );

    if (testmemcmp == 0){
    	printf("massive same\n");

    }else{

    	printf("massive not same\n");
    }
*/
#ifdef DEBUG_MODE
	printf("Start Eth connect \n");// сделать запись в лог
#endif

	    struct sockaddr_storage far_client_addr;
	    socklen_t far_addr_size;

	    int sock, sock_ser, n;
	    //struct sockaddr_in addr;
	    struct addrinfo *res, *t;
	    struct addrinfo hints = { 0 };

	    if(cfg_brd_old.eth_type_connection == 0x00){ // client mode type connection

	    	memset(&hints, 0, sizeof hints);

	    	hints.ai_family   = AF_UNSPEC;
	    	hints.ai_socktype = SOCK_STREAM;

	    	n = getaddrinfo(cfg_brd_old.dname, cfg_brd_old.port, &hints, &res);

	    	if (n < 0) {
	    		fprintf(stderr, "%s for %s:%s\n", gai_strerror(n), cfg_brd_old.dname, cfg_brd_old.port);// сделать запись в лог
	    		printf("ERROR(%s:%d): getaddrinfo failed!\n",__FILE__,__LINE__);// сделать запись в лог
	    		exit(1);
	    	}

	    	for (t = res; t; t = t->ai_next) {
	    		sock = socket(t->ai_family, t->ai_socktype, t->ai_protocol);
	    		if (sock >= 0) {
	    			if (connect(sock, t->ai_addr, t->ai_addrlen) == 0){
	    				break;
	    			}
	    			close(sock);
	    			sock = -1;
	    		}
	    	}

	    	freeaddrinfo(res);

	    	if( sock == -1 ){
	    		printf("Eth connect client mode ERROR!\n");// сделать запись в лог
	    		exit(1);
	    	}

	    	printf("Eth client mode connect success!\n"); // сделать запись в лог

	    }else if( cfg_brd_old.eth_type_connection == 0x01 ){ // server mode type connection

	    	memset(&hints, 0, sizeof hints);
	    	hints.ai_family   = AF_UNSPEC;
	    	hints.ai_socktype = SOCK_STREAM;
	    	hints.ai_flags = AI_PASSIVE;

	    	n = getaddrinfo( NULL, cfg_brd_old.port, &hints, &res);

	    	if (n < 0) {
	    		fprintf(stderr, "%s for NULL :%s\n", gai_strerror(n), cfg_brd_old.port);// сделать запись в лог
	    		printf("ERROR(%s:%d): getaddrinfo failed!\n",__FILE__,__LINE__);// сделать запись в лог
	    		exit(1);
	    	}

	    	sock_ser = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	    	if( sock_ser < 0 ){
	    		printf("ERROR(%s:%d): getaddrinfo failed!\n",__FILE__,__LINE__);
	    		close(sock_ser);
	    		close(sock);
	    		freeaddrinfo(res);
	    		exit(1);
	    	}

	    	n = bind(sock_ser, res->ai_addr, res->ai_addrlen);
			if( n < 0 ){
				printf("ERROR(%s:%d): getaddrinfo failed!\n",__FILE__,__LINE__);
				close(sock_ser);
				close(sock);
				freeaddrinfo(res);
				exit(1);
			}

	    	n = listen(sock_ser, 0);
	    	if( n < 0 ){
	    		printf("ERROR(%s:%d): getaddrinfo failed!\n",__FILE__,__LINE__);
	    		close(sock_ser);
	    		close(sock);
	    		freeaddrinfo(res);
	    		exit(1);
	    	}

	    	far_addr_size = sizeof far_client_addr;
	    	sock = accept(sock_ser, (struct sockaddr *)&far_client_addr, &far_addr_size);
	    	if( sock < 0 ){
	    		printf("ERROR(%s:%d): getaddrinfo failed!\n",__FILE__,__LINE__);
	    		close(sock_ser);
	    		close(sock);
	    		freeaddrinfo(res);
	    		exit(1);
	    	}



	    	printf("Eth: Client connect success!\n"); // сделать запись в лог

	    	char hoststr[NI_MAXHOST];
	    	char portstr[NI_MAXSERV];

	    	n = getnameinfo((struct sockaddr *)&far_client_addr, far_addr_size, hoststr, sizeof(hoststr), portstr, sizeof(portstr), NI_NUMERICHOST | NI_NUMERICSERV);
	    	if (n == 0){
	    		printf("New connection IP: %s Port: %s \n", hoststr, portstr);
	    	}

	    	freeaddrinfo(res);
	    }else{
	    	printf("ERROR(%s:%d): getaddrinfo failed!\n",__FILE__,__LINE__);
	    	exit(1);
	    }

    uint8_t buf[4] = { 0 };
    uint32_t tipe_eth_rx_parsel;
    uint32_t *size_rx_tx_buf_eth;
    uint32_t rx_tx_buf_eth_size = 1536;
    size_rx_tx_buf_eth = &rx_tx_buf_eth_size;

    uint8_t *rx_buf_eth_parcel = (uint8_t *)malloc(rx_tx_buf_eth_size);
    if(rx_buf_eth_parcel == NULL){
    	free (rx_buf_eth_parcel);
    	printf("error malloc: %s \n","rx_buf_eth_parcel"); // сделать запись в лог
    	exit(1);
    }
    uint8_t *tx_buf_eth_parcel = (uint8_t *)malloc(rx_tx_buf_eth_size);
    if(tx_buf_eth_parcel == NULL){
    	free (tx_buf_eth_parcel);
    	printf("error malloc: %s \n","tx_buf_eth_parcel"); // сделать запись в лог
    	exit(1);
    }


    struct pollfd pfd;
    int timeout = 10; // in milliseconds
    int rc;
    pfd.fd = sock;
    pfd.events = POLLIN | POLLHUP;
    pfd.revents = 0;

while(1){

	if( ( rc = poll(&pfd, 1, timeout) ) < 0 ){
		if( rc == EINTR ){
			rc = 0;
		} else {
			printf("Fatal error in poll: %s:%d\n", __FILE__, __LINE__);
			exit(1);
		}
	}

	if( pfd.revents & POLLHUP ){

		printf("Far device close POLLHUP\n");
		close(sock);
		break;
	}


	if( pfd.revents & POLLIN ){

		ret = recv(sock, buf, 4, 0); //&tipe_eth_rx_parsel
		if(ret > 0){

			tipe_eth_rx_parsel = buf[0];
			tipe_eth_rx_parsel = (tipe_eth_rx_parsel<<8)|(buf[1]);
			tipe_eth_rx_parsel = (tipe_eth_rx_parsel<<8)|(buf[2]);
			tipe_eth_rx_parsel = (tipe_eth_rx_parsel<<8)|(buf[3]);

			if(tipe_eth_rx_parsel == 0x0001){// get parsel from server
#ifdef DEBUG_MODE
				printf("Got command parcel from Eth\n");
#endif
				ret = pars_eth_command_parcel(fd_SPI_BB, fd_i2c, sock, rx_buf_eth_parcel, tx_buf_eth_parcel, size_rx_tx_buf_eth, cfg_ch_old, &cfg_brd_old);
				if(ret == -1){
					printf("Error Eth command parser\n");
					close(sock);
					break;

				}
#ifdef DEBUG_MODE
				printf("Parser eth command FINISH work\n");
#endif
			}

		}else{
#ifdef DEBUG_MODE
			printf("Far device close Eth connection\n");
#endif
			close(sock);
			break;
		}

	}

	//send data earthquake emulation
	if(cfg_ch_old[0].state == 2 || cfg_ch_old[2].state == 2 || cfg_ch_old[3].state == 2 ){

		sleep (1); // sleep 1 second
		ret = send_earthquake_emul_data_to_Eth(sock, cfg_brd_old);
		if(ret != 0){
			printf("Error Send data ADC to Eth\n");// запись в лог
		}
	}

//	send data analog channels
	if(cfg_ch_old[0].state == 1 || cfg_ch_old[2].state == 1 || cfg_ch_old[3].state == 1 ){

		ret = send_ADC_data_to_Eth(fd_SPI_BB, 3, sock, cfg_ch_old );
		if(ret != 0){
			printf("Error Send data ADC to Eth\n");// запись в лог
		}
	}


}

     free(rx_buf_eth_parcel);
     free(tx_buf_eth_parcel);
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







