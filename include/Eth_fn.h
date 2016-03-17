/*
 * Eth_fn.h
 *
 *  Created on: 14 марта 2016 г.
 *      Author: Ivan
 */

#ifndef INCLUDE_ETH_FN_H_
#define INCLUDE_ETH_FN_H_
int sendall(int fd_s, char *buf, int len, int flags);
int pars_eth_command_parcel(int fd_SPI, int fd_socket, uint8_t *rx_buf_eth,  uint8_t *tx_buf_eth, uint32_t *real_rx_tx_buf_eth_size, struct settings_ch *settings_channels);

#endif /* INCLUDE_ETH_FN_H_ */
