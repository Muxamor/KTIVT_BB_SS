/*
 * M41T64_Clock.h
 * Version: 0.0
 * Date:	16.1.2016
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */

#ifndef INCLUDE_M41T64_CLOCK_H_
#define INCLUDE_M41T64_CLOCK_H_

#define I2C_BUS_NUMBER 1
#define ADDR_I2C_SLAVE_M41T64 0x68

int i2c_device_open(char *device);
int Defult_setup_M41T64(unsigned int fd_i2c, unsigned int addr_slave );
int M41T64_set_SQW_clock (int fd_i2c, uint16_t clock_SQW, unsigned int addr_slave );
int get_ml_seconds_M41T64(unsigned int fd_i2c, unsigned int addr_slave);
int set_seconds_M41T64(unsigned int fd_i2c, uint8_t val_seconds, unsigned int addr_slave);
int get_seconds_M41T64(unsigned int fd_i2c, unsigned int addr_slave);
int set_minutes_M41T64(unsigned int fd_i2c, uint8_t val_minutes, unsigned int addr_slave);
int get_minutes_M41T64(unsigned int fd_i2c, unsigned int addr_slave);
int set_hours_M41T64(unsigned int fd_i2c, uint8_t val_hours, unsigned int addr_slave);
int get_hours_M41T64(unsigned int fd_i2c, unsigned int addr_slave);
int set_day_M41T64(unsigned int fd_i2c, uint8_t val_day, unsigned int addr_slave);
int get_day_M41T64(unsigned int fd_i2c, unsigned int addr_slave);
int set_century_month_M41T64(unsigned int fd_i2c, uint8_t val, unsigned int addr_slave);
int get_century_month_M41T64(unsigned int fd_i2c, unsigned int addr_slave);
int set_year_M41T64(unsigned int fd_i2c, uint8_t val_year, unsigned int addr_slave);
int get_year_M41T64(unsigned int fd_i2c, unsigned int addr_slave);
#endif /* INCLUDE_M41T64_CLOCK_H_ */
