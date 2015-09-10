/*
 * SPI_SS.h
 *
 *
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */

#ifndef INCLUDE_SPI_SS_H_
#define INCLUDE_SPI_SS_H_
#include <stdint.h>

int fd_SPI_BB;
struct spi_ioc_transfer SPI_trunsfer_struct;

int spi_device_open(char *device);
int set_spi_settings(int fd, uint8_t mode, uint8_t bits_in_word, uint32_t speed_SPI);
int spi_transfer (int fd, uint16_t SPI_channel, uint16_t tx_buffer[], uint16_t rx_buffer[], uint16_t size, uint16_t delay_SPI);


#endif /* INCLUDE_SPI_SS_H_ */
