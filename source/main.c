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


/*Для примера от Темыча */



#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>



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
 int main(int argc, char **argv){

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

///Пример от Темыча



	printf("Start connect \n");

	    int sock, n;
	    struct sockaddr_in addr;
	    struct addrinfo *res, *t;
	    struct addrinfo hints = { 0 };
	    char *service, *servername;

	    if( argc < 3 ){
	        printf("Need server name and port\n");
	        exit(1);
	    }

	    servername = strdup(argv[1]);
	    service = strdup(argv[2]);

	    hints.ai_family   = AF_UNSPEC;
	    hints.ai_socktype = SOCK_STREAM;
	    n = getaddrinfo(servername, service, &hints, &res);

	    if (n < 0) {
	        fprintf(stderr, "%s for %s:%s\n", gai_strerror(n), servername, service);
	        free(service);
	        free(servername);
	        printf("ERROR(%s:%d): getaddrinfo failed!\n",__FILE__,__LINE__);
	        exit(1);
	    }

	    for (t = res; t; t = t->ai_next) {
	        sock = socket(t->ai_family, t->ai_socktype, t->ai_protocol);
	        if (sock >= 0) {
	            if (!connect(sock, t->ai_addr, t->ai_addrlen))
	                break;
	            close(sock);
	            sock = -1;
	        }
	    }

	    freeaddrinfo(res);
	    free(servername);
	    free(service);

	    printf("Connect  success!\n");

    uint8_t buf[4] = { 0 };
    uint32_t tipe_eth_rx_parsel;
    uint32_t *size_rx_tx_buf_eth;
    uint32_t rx_tx_buf_eth_size = 1536;
    size_rx_tx_buf_eth = &rx_tx_buf_eth_size;

    uint8_t *rx_buf_eth_parcel = (uint8_t *)malloc(rx_tx_buf_eth_size);
    if(rx_buf_eth_parcel == NULL){
    	free (rx_buf_eth_parcel);
    	printf("error malloc: %s \n","rx_buf_eth_parcel");
    	exit(1);
    }
    uint8_t *tx_buf_eth_parcel = (uint8_t *)malloc(rx_tx_buf_eth_size);
    if(tx_buf_eth_parcel == NULL){
    	free (tx_buf_eth_parcel);
    	printf("error malloc: %s \n","tx_buf_eth_parcel");
    	exit(1);
    }

    struct pollfd pfd;
    int timeout = 10; // in milliseconds
    int rc;
    pfd.fd = sock;
    pfd.events = POLLIN | POLLHUP;
    pfd.revents = 0;

    // only for test
    //sleep(10);

       uint8_t bufttew[10]= {0xAA};

       send(sock, bufttew, sizeof(bufttew), 0);
       ///////////////////////////////////////////////////

while(1){

	if( ( rc = poll(&pfd, 1, timeout) ) < 0 ){
		if( rc == EINTR ){
			rc = 0;
		} else {
			printf("Fatal error in poll: %s:%d\n", __FILE__, __LINE__);
			exit(1);
		}
	}

	if( pfd.revents & POLLHUP ){//& (rc>0)

		printf(" Server close POLLHUP\n");
		close(sock);
		break;
	}



	if( pfd.revents & POLLIN ){// & (rc>0)

		ret = recv(sock, buf, 4, 0);
		if(ret > 0){

			tipe_eth_rx_parsel = buf[0];
			tipe_eth_rx_parsel = (tipe_eth_rx_parsel<<8)|(buf[1]);
			tipe_eth_rx_parsel = (tipe_eth_rx_parsel<<8)|(buf[2]);
			tipe_eth_rx_parsel = (tipe_eth_rx_parsel<<8)|(buf[3]);

			if(tipe_eth_rx_parsel == 0x0001){// получен пакет с коммандами из интеренета
				printf(" Got command parcel from Eth\n");
				ret = pars_eth_command_parcel(fd_SPI_BB, sock, rx_buf_eth_parcel, tx_buf_eth_parcel, size_rx_tx_buf_eth, cfg_ch_old);
				if(ret == -1){
					printf(" Error Eth command parser\n");
					close(sock);
					break;

				}
				printf(" Parser eth command FINISH work\n");

			}

		}else{
			printf(" Server close Eth connection\n");
			close(sock);
			break;
		}

	}






 }
/*
	uint8_t test_buf_rx[] = {0x00,0x01, 0x01,0x03, 0x00,0x00,
							   0x00,0x01, 0x02,0x00, 0x00,0x00,
							   //0x00,0x00, 0x21,0x01, 0x00,0x00,
							   0x00,0x02, 0x01,0x03, 0x00,0x00,
							   0x00,0x02, 0x02,0x00, 0x00,0x00,
							 //  0x00,0x00, 0x21,0x01, 0x00,0x00,
							   0x00,0x03, 0x01,0x03, 0x00,0x00,
							   0x00,0x03, 0x02,0x00, 0x00,0x00,
							   0x00,0x00, 0x21,0x01, 0x00,0x00};

	 gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , HIGHT);
	 gpio_set_value(fd_GPIO_pin_output[GPIO_Sync_Ch1_Ch2_Ch3] , LOW);
	 printf("Enable Sync\n");
*/

/*
	  j=0;
sleep(5);

	 uint16_t tx_buf[8206] = {0x0000};
	 uint16_t rx_buf[8206] = {};

	 uint16_t tx_buf_status[2] = {0x2800,0x0000};
	 uint16_t rx_buf_status[2] = {0x0000,0x0000};

while(j!=10){

	//printf("Cicle = %d\n",j);

	ret = spi_transfer_command_analog_ch ( fd_SPI_BB, GPIO_SPI_CS_Ch1, GPIO_SPI_INT_Ch1, tx_buf_status,rx_buf_status, sizeof(tx_buf_status), 0 );
	if(ret != 0){
	    perror("Error SPI\n");
	}
	if(rx_buf_status[0] != 0x2800){
		printf("0x%.4X \n", rx_buf_status[0]);

		j++;
		printf("Channel 1\n");

	    ret = spi_read_data_ADC24 (fd_SPI_BB, GPIO_SPI_CS_Ch1, GPIO_SPI_INT_Ch1, tx_buf, rx_buf, sizeof(tx_buf), 0);
	    if(ret != 0){
	    	perror("Error SPI\n");
	    }

	    for (i = 0; i < 20; i++){
	    	printf("0x%.4X ", rx_buf[i]);
	    }
	    printf("\n");
	    for (i = 8186; i < 8206; i++){
	    	printf("0x%.4X ", rx_buf[i]);
	    }
	    printf("\n");
	}



	ret = spi_transfer_command_analog_ch ( fd_SPI_BB, GPIO_SPI_CS_Ch2, GPIO_SPI_INT_Ch2, tx_buf_status,rx_buf_status, sizeof(tx_buf_status), 0 );
		if(ret != 0){
		    perror("Error SPI\n");
		}
		if(rx_buf_status[0] != 0x2800){
			printf("0x%.4X \n", rx_buf_status[0]);


			printf("Channel 2\n");

		    ret = spi_read_data_ADC24 (fd_SPI_BB, GPIO_SPI_CS_Ch2, GPIO_SPI_INT_Ch2, tx_buf, rx_buf, sizeof(tx_buf), 0);
		    if(ret != 0){
		    	perror("Error SPI\n");
		    }

		    for (i = 0; i < 20; i++){
		    	printf("0x%.4X ", rx_buf[i]);
		    }
		    printf("\n");
		    for (i = 8186; i < 8206; i++){
		    	printf("0x%.4X ", rx_buf[i]);
		    }
		    printf("\n");
	}


	ret = spi_transfer_command_analog_ch ( fd_SPI_BB, GPIO_SPI_CS_Ch3, GPIO_SPI_INT_Ch3, tx_buf_status,rx_buf_status, sizeof(tx_buf_status), 0 );
			if(ret != 0){
			    perror("Error SPI\n");
			}
			if(rx_buf_status[0] != 0x2800){
				printf("0x%.4X \n", rx_buf_status[0]);


				printf("Channel 3\n");

			    ret = spi_read_data_ADC24 (fd_SPI_BB, GPIO_SPI_CS_Ch3, GPIO_SPI_INT_Ch3, tx_buf, rx_buf, sizeof(tx_buf), 0);
			    if(ret != 0){
			    	perror("Error SPI\n");
			    }

			    for (i = 0; i < 20; i++){
			    	printf("0x%.4X ", rx_buf[i]);
			    }
			    printf("\n");
			    for (i = 8186; i < 8206; i++){
			    	printf("0x%.4X ", rx_buf[i]);
			    }
			    printf("\n");
	}


}
*/
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







