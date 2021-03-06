/*
 * analog_ch_setup.h
*
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */

#ifndef INCLUDE_ANALOG_CH_SETUP_H_
#define INCLUDE_ANALOG_CH_SETUP_H_

#define COMPARE_SETTINGS 1
#define NO_COMPARE_SETTINGS 0

 int parse_set_settings_analog_ch(int fd_SPI, struct settings_ch *settings_old, struct settings_ch *settings_new, uint16_t size_settings, uint8_t compare_settings, uint8_t quantity_channels);
 int setup_time_date_a_ch (int fd_SPI,unsigned int fd_I2C);
#endif /* INCLUDE_ANALOG_CH_SETUP_H_ */
