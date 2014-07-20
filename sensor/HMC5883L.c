/***********************************************************************
 *  File Name     : HMC5883L.c
 *  Author        : Jesse Cheun
 *  Date          : 06/20/2014
 *  Description   : Honneywell HMC5883L Sensor Driver Library
 **********************************************************************/
#include <stdio.h>
#include <stdint.h>

#include "debugf.h"
#include "HMC5883L.h"
#include "linuxHAL.h"

int HMC_Init(void) {
   uint8_t tmp[3];
   if (I2C_read(HMC5883L_HW_ADDR, HMC_REG_ID, 3, tmp)) {
      ERROR("Fail to read device ID\n");
      return -1;
   }

   if (tmp[0] != 'H' || tmp[1] != '4' || tmp[2] != '3') {
      ERROR("Fail to match device ID\n");
      return -1;
   }
   
   tmp[0] = BIT_SMP_AVG_8 | BIT_SMP_RATE_15;
   if (I2C_write(HMC5883L_HW_ADDR, HMC_REG_CONFIG_A, 1, tmp)) {
      ERROR("Fail to configure device\n");
      return -1;
   }

   tmp[0] = BIT_GAIN_1090;
   if (I2C_write(HMC5883L_HW_ADDR, HMC_REG_CONFIG_B, 1, tmp)) {
      ERROR("Fail to configure gain\n");
      return -1;
   }
   
   tmp[0] = BIT_MODE_CONT;
   if (I2C_write(HMC5883L_HW_ADDR, HMC_REG_MODE, 1, tmp)) {
      ERROR("Fail to configure mode\n");
      return -1;
   }
   return 0;
}

int HMC_GetMag(int16_t *data) {
   uint8_t tmp[6];
   
   if (I2C_read(HMC5883L_HW_ADDR, HMC_REG_STATUS, 1, tmp)) 
      return -1;

   if ((tmp[0] & 0x01) != 1) return -1;

   if (I2C_read(HMC5883L_HW_ADDR, HMC_REG_MAG_OUT, 6, tmp)) {
      ERROR("Fail to read magnetometer measurements\n");
      return -1;
   }

   data[0] = (tmp[0] << 8) | tmp[1];
   data[2] = (tmp[2] << 8) | tmp[3];
   data[1] = (tmp[4] << 8) | tmp[5];
   
   return 0;
}

int HMC_SelfTest(void) {
   uint8_t tmp;
   int16_t mag[3];

   tmp = 0x71; //BIT_SMP_AVG_8 | BIT_SMP_RATE_RES;
   if (I2C_write(HMC5883L_HW_ADDR, HMC_REG_CONFIG_A, 1, &tmp)) {
      ERROR("Fail to configure device\n");
      return -1;
   }

   tmp = 0xA0;//BIT_GAIN_390;
   if (I2C_write(HMC5883L_HW_ADDR, HMC_REG_CONFIG_B, 1, &tmp)) {
      ERROR("Fail to configure gain\n");
      return -1;
   }
   
   tmp = 0; //BIT_MODE_CONT;
   if (I2C_write(HMC5883L_HW_ADDR, HMC_REG_MODE, 1, &tmp)) {
      ERROR("Fail to configure mode\n");
      return -1;
   }

   delay_ms(6);

   while (HMC_GetMag(mag) != 0);
   printf("Mag: %06d %06d %06d\n",  mag[0], mag[1], mag[2]);
   return 0;
}




