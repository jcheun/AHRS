/***********************************************************************
 *  File Name     : linuxHAL.h
 *  Author        : Jesse Cheun
 *  Date          : 06/14/2014
 *  Description   : Simple Linux I2C Communcation and System Delay 
 **********************************************************************/

#ifndef __LINUXHAL_H__
#define __LINUXHAL_H__

/*-- Includes --------------------------------------------------------*/
#include <stdint.h>

/*-- Functions -------------------------------------------------------*/
void I2C_init(int bus);
int I2C_read(uint8_t sAddr, uint8_t rAddr, uint8_t length, 
             uint8_t *data);
int I2C_write(uint8_t sAddr, uint8_t rAddr, uint8_t length, 
              uint8_t const *data);
int delay_ms(uint64_t ms);

#endif
