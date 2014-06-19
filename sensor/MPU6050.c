/***********************************************************************
 *  File Name     : MPU6050.h
 *  Author        : Jesse Cheun
 *  Date          : 06/15/2014
 *  Description   : Invensense MPU6050 Senser Driver Library
 **********************************************************************/

/*-- Includes --------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>

#include "debugf.h"
#include "linuxHAL.h"
#include "MPU6050.h"

/***********************************************************************
 * Function Name  : MPU_Init
 * Description    : Initialize MPU6050
 * Parameter[In]  : gyroFsr   -> Gyroscope Full Scale Range
 *                  accelFsr  -> Accelerometer Full Scale Range
 *                  sampleRate-> Sample Rate
 *                  lpg       -> Low Pass Filter
 * Parameter[Out] : NONE
 * Return         : 0 if Success
 **********************************************************************/
int MPU_Init(uint8_t gyroFsr, uint8_t accelFsr, 
             uint16_t sampleRate, uint8_t lpf) {
   
   uint8_t data;
   
   /* Check device ID */
   if (I2C_read(MPU6050_HW_ADDR, REG_WHO_AM_I, 1, &data)) {
      ERROR("Fail to read device ID\n");
      return -1;
   } 
   
   if (data != 0x68) {
      ERROR("Fail to idenify device ID\n");
      return -1;
   }

   /* Reset device */
   data = BIT_DEVICE_RESET;
   if (I2C_write(MPU6050_HW_ADDR, REG_PWR_MGMT_1, 1, &data)) {  
      ERROR("Fail to reset MPU6050\n");
      return -1;
   }
  
   delay_ms(100);

   /* Wake up chip and set clock source */
   data = BIT_CLKSEL_X;
   if (I2C_write(MPU6050_HW_ADDR, REG_PWR_MGMT_1, 1, &data)) {
      ERROR("Fail to wake MPU6050 and set clock source\n");
      return -1;
   }
 
   /* Set Gyroscope Full Scale Range */
   data = gyroFsr;
   if (I2C_write(MPU6050_HW_ADDR, REG_GYRO_CONFIG, 1, &data)) {
      ERROR("Fail to set Gyroscope Full Scale Range\n");
      return -1;
   }

   /* Set Accelerometer Full Scale Range */
   data = accelFsr;
   if (I2C_write(MPU6050_HW_ADDR, REG_ACCEL_CONFIG, 1, &data)) {
      ERROR("Fail to set Accelerometer Full Scale Range\n");
      return -1;
   }

   /* Set Low Pass Filter */
   data = lpf;
   if (I2C_write(MPU6050_HW_ADDR, REG_CONFIG, 1, &data)) {
      ERROR("Fail to set Low Pass Filter");
      return -1;
   }

   /* Set Saple Rate */
   if (MPU_SetSampleRate(sampleRate, lpf))
      return -1;
   
   /* Enable FIFO */

   /* BYPASS Enaable*/

   DEBUGF('m', "MPU6050 Succesfully Init\n");  
   return 0;
}

/***********************************************************************
 * Function Name  : MPU_SetSampleRate
 * Description    : Set device sample rate
 * Parameter[In]  : rate    -> Sample Rate
 *                  lpf     -> Low Pass Filter
 * Parameter[Out] : NONE
 * Return         : 0 if Success
 **********************************************************************/

int MPU_SetSampleRate(uint16_t rate, uint8_t lpf) {
   int outRate = 1000;
   uint8_t sampleRateDiv = 0;

   if (lpf == BIT_DLPF_CFG_256 ||
       lpf == BIT_DLPF_CFG_RES)
      outRate = 8000;

   sampleRateDiv = outRate / rate - 1;
   
   if (I2C_write(MPU6050_HW_ADDR, REG_SMPRT_DIV, 1, &sampleRateDiv)) {
      ERROR("Fail to set device Sample Rate");
      return -1;
   }

   return 0;
}

/***********************************************************************
 * Function Name  : MPU_GetGyro
 * Description    : Read raw gyroscope measurement from device
 * Parameter[In]  : NONE
 * Parameter[Out] : data   -> Raw gyroscope measurements
 * Return         : 0 if Success
 **********************************************************************/
int MPU_GetGyro(int16_t *data) {
   uint8_t tmp[6];
   
   if (I2C_read(MPU6050_HW_ADDR, REG_GYRO_OUT, 6, tmp)) {
      ERROR("Fail to read gyroscope measurements\n");
      return -1;
   }
   
   data[0] = (tmp[0] << 8) | tmp[1];
   data[1] = (tmp[2] << 8) | tmp[3];
   data[2] = (tmp[4] << 8) | tmp[5];
   
   return 0;
}

/***********************************************************************
 * Function Name  : MPU_GetAccel
 * Description    : Read raw acceleromenter measurements from device
 * Parameter[In]  : NONE
 * Parameter[Out] : data   -> Raw accelerometer measurments
 * Return         : NONE
 **********************************************************************/
int MPU_GetAccel(int16_t *data) {
   uint8_t tmp[6];
   
   if (I2C_read(MPU6050_HW_ADDR, REG_ACCEL_OUT, 6, tmp)) {
      ERROR("Fail to read accelerometer measurements\n");
      return -1;
   }
   
   data[0] = (tmp[0] << 8) | tmp[1];
   data[1] = (tmp[2] << 8) | tmp[3];
   data[2] = (tmp[4] << 8) | tmp[5];
   
   return 0;
}

/***********************************************************************
 * Function Name  : MPU_GetSensors
 * Description    : Read raw measurements from device
 * Parameter[In]  : NONE
 * Parameter[Out] : accel  -> Raw accelerometer measurments
 *                  gyro   -> Raw gyroscope measurements   
 * Return         : NONE
 **********************************************************************/
int MPU_GetSensors(int16_t *accel, int16_t *gyro) {
   uint8_t tmp[14];
   
   if (I2C_read(MPU6050_HW_ADDR, REG_ACCEL_OUT, 14, tmp)) {
      ERROR("Fail to read sensor measurements\n");
      return -1;
   }
   
   accel[0] = (tmp[0] << 8) | tmp[1];
   accel[1] = (tmp[2] << 8) | tmp[3];
   accel[2] = (tmp[4] << 8) | tmp[5];
    
   gyro[0] = (tmp[8]  << 8) | tmp[9];
   gyro[1] = (tmp[10] << 8) | tmp[11];
   gyro[2] = (tmp[12] << 8) | tmp[13];
   
   return 0;
}






