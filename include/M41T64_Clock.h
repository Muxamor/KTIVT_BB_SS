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

int Defult_setup_M41T64(uint8_t i2c_port, int addr_slave );
int M41T64_set_SQW_clock (uint8_t i2c_port, uint16_t clock_SQW, int addr_slave );

#endif /* INCLUDE_M41T64_CLOCK_H_ */
