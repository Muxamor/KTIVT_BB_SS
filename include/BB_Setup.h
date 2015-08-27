/*
 * BB_Setup.h
 *
 *  Created on: 25 авг. 2015 г.
 *      Author: Ivan
 */

#ifndef INCLUDE_BB_SETUP_H_
#define INCLUDE_BB_SETUP_H_

/****************************************************
*                 Constants                         *
****************************************************/

#define GPIO_SPI_CS_Ch1 115
#define GPIO_SPI_CS_Ch2 117
#define GPIO_SPI_CS_Ch3 48
#define GPIO_SPI_INT_Ch1 7
#define GPIO_SPI_INT_Ch2 26
#define GPIO_SPI_INT_Ch3 27
#define GPIO_SPI_CS_Disp 86
#define GPIO_Sync_Ch1_Ch2_Ch3 89
#define GPIO_SPI_Reset_Ch1 65
#define GPIO_SPI_Reset_Ch2 70
#define GPIO_SPI_Reset_Ch3 71
#define GPIO_SPI_CS_Col 49

	int fd_GPIO_SPI_CS_Ch1;
	int fd_GPIO_SPI_CS_Ch2;
	int fd_GPIO_SPI_CS_Ch3;
	int fd_GPIO_SPI_CS_Disp;
	int fd_GPIO_SPI_CS_Col;
	int fd_GPIO_Sync_Ch1_Ch2_Ch3;
	int fd_GPIO_SPI_INT_Ch1;
	int fd_GPIO_SPI_INT_Ch2;
	int fd_GPIO_SPI_INT_Ch3;
	int fd_GPIO_SPI_Reset_Ch1;
	int fd_GPIO_SPI_Reset_Ch2;
	int fd_GPIO_SPI_Reset_Ch3;


/***************************************************
*                Setup BB function                 *
***************************************************/
int Default_Setup_GPIO_BB (void);



#endif /* INCLUDE_BB_SETUP_H_ */
