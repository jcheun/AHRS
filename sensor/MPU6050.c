#include <stdio.h>
#include <stdint.h>

#include "MPU6050.h"
#include "debugf.h"
#include "linuxHAL.h"

int MPU_Init(uint8_t gyroFsr, uint8_t accelFsr) {
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
   //data = lpf;
   //if (I2C_write(MPU_HW_ADDR, REG_PWR_MGMT_1, 1, &data))
     // return -1;
 
   /* Set Sample Rate */
   //data = sampleRate;
   //if (I2C_write(MPU_HW_ADDR, REG_PWR_MGMT_1, 1, &data))
      //return -1;
 
   /* Enable FIFO */


   DEBUGF('m', "MPU6050 Succesfully Init\n");  
   return 0;
}

int MPU_GetGyro(short *data) {
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






