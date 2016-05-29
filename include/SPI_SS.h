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
#include <linux/spi/spidev.h>

#define SIZE_HEAD_PACKAGE_ADC 28 // head size of adc package in the byte


struct spi_ioc_transfer SPI_trunsfer_struct;


int spi_device_open(char *device);
int set_spi_settings(int fd, uint8_t mode, uint8_t msb_lsb_mode, uint8_t bits_in_word, uint32_t speed_SPI);
int spi_transfer (int fd, uint16_t SPI_channel, uint16_t tx_buffer[], uint16_t rx_buffer[], uint16_t size, uint16_t delay_SPI);
int spi_transfer_command_analog_ch ( int fd, uint16_t SPI_channel, uint16_t SPI_channel_int, uint16_t tx_buffer[], uint16_t rx_buffer[], uint16_t size, uint16_t delay_SPI );
int spi_read_data_ADC24 (int fd, uint16_t SPI_channel, uint16_t SPI_channel_int, uint16_t tx_buffer[], uint16_t rx_buffer[], uint16_t size, uint16_t delay_SPI);
int sent_command_all_analog_ch (int fd_SPI, uint16_t *tx_buf );
#endif /* INCLUDE_SPI_SS_H_ */
