/*
 * Eth_fn.h
 *
 *  Created on: 14 марта 2016 г.
 *      Author: Ivan
 */

#ifndef INCLUDE_ETH_FN_H_
#define INCLUDE_ETH_FN_H_


int pars_eth_command_parsel(int fd_SPI,  uint8_t *rx_buf_eth, uint8_t *tx_buf_eth, uint32_t size_rx, struct settings_ch *settings_channels);

#endif /* INCLUDE_ETH_FN_H_ */
