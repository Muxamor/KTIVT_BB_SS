/*
 * main.h
 *
 *
 * Version: 0.0
 * Date:	26.7.2015
 *
 * Develop: Ivan Neskorodev
 * Email: ivan.neskorodev@gmail.com
 *
 */

#ifndef _MAIN_H
#define _MAIN_H

/****************************************************************
 * Includes
 ****************************************************************/
#define  DEBUG_MODE // ON debug mode
//#undef DEBUG_MODE // OFF debug mode


#define REVERSE_LE_BE_u16(A) \
	((((uint16_t)(A) & 0xff00) >> 8) | \
    (((uint16_t)(A) & 0x00ff) << 8))


#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))


#endif /* _MAIN_H */
