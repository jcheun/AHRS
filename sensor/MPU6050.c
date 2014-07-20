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
int get_biases ();
/***********************************************************************
 * Function Name  : MPU_Init
 * Description    : Initialize MPU6050
 * Parameter[In]  : gyroFsr   -> Gyroscope Full Scale Range
 *                  accelFsr  -> Accelerometer Full Scale Range
 *                  sampleRate-> Sample Rate
 *                  lpf       -> Low Pass Filter
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
      ERROR("Fail to set Low Pass Filter\n");
      return -1;
   }

   /* Set Saple Rate */
   if (MPU_SetSampleRate(sampleRate, lpf))
      return -1;

   /* BYPASS Enable */
   if (MPU_SetBypass())
      return -1;
   
   /* Enable FIFO */
   if (MPU_ResetFIFO())
      return -1; 

   DEBUGF('m', "MPU6050 Succesfully Init\n");  
   return 0;
}

/*
int MPU_GetIntStatus(int16_t *data) {
   uint8_t tmp[2];
   if (I2C_read(MPU6050_HW_ADDR, 0x39, 2, tmp)) {
      return -1;
   }
   
   data[0] = (tmp[0] << 8) | tmp[1];
   return 0;
}*/

int get_biases () {
   uint8_t data;
   int hw_test = 0;

   data = BIT_DLPF_CFG_188;
   if (I2C_write (MPU6050_HW_ADDR, REG_CONFIG, 1, &data)) {  
      ERROR ("Fail to set Low Pass Filter\n");
      return -1;
   }
   
   data = 0x00;
   if (I2C_write(MPU6050_HW_ADDR, REG_SMPRT_DIV, 1, &data)) {
      ERROR("Fail to set device Sample Rate\n");
      return -1;
   }

   /* Set Gyroscope Full Scale Range */
   data = 0x00 | (hw_test) ? 0xE0 : 0x00;
   if (I2C_write(MPU6050_HW_ADDR, REG_GYRO_CONFIG, 1, &data)) {
      ERROR("Fail to set Gyroscope Full Scale Range\n");
      return -1;
   }

   /* Set Accelerometer Full Scale Range */
   data = BIT_AFS_SEL_16G | (hw_test) ? 0xE0 : 0x00;;
   if (I2C_write(MPU6050_HW_ADDR, REG_ACCEL_CONFIG, 1, &data)) {
      ERROR("Fail to set Accelerometer Full Scale Range\n");
      return -1;
   }

   if (hw_test) delay_ms (200);

   return 0;
}

//int MPU_SelfTest (int16_t *accel, int16_t *gyro) {
//   long accel_test[3], gyro_test[3];
//   uint8_t a_result, g_result;
//   
//
//}

/***********************************************************************
 * Function Name  : MPU_ResetFIFO
 * Description    : Reset FIFO buffer
 * Parameter[In]  : NONE
 * Parameter[Out] : NONE
 * Return         : 0 if Success
 **********************************************************************/
int MPU_ResetFIFO(void) {
   uint8_t tmp;
   
   tmp = 0;
   if (I2C_write(MPU6050_HW_ADDR, REG_INT_ENABLE, 1, &tmp)) 
      return -1;

   if (I2C_write(MPU6050_HW_ADDR, REG_FIFO_EN, 1, &tmp)) 
      return -1;

   if (I2C_write(MPU6050_HW_ADDR, REG_USER_CTRL, 1, &tmp)) 
      return -1;

   tmp = BIT_FIFO_RESET;
   if (I2C_write(MPU6050_HW_ADDR, REG_USER_CTRL, 1, &tmp)) {
      ERROR("Fail to reset FIFO\n");
      return -1;
   }

   tmp = BIT_FIFO_EN;
   if (I2C_write(MPU6050_HW_ADDR, REG_USER_CTRL, 1, &tmp)) {
      ERROR("Fail to enable FIFO\n");
      return -1;
   }
   
   delay_ms(50);
  
   tmp = BIT_DATA_RDY_EN;
   if (I2C_write(MPU6050_HW_ADDR, REG_INT_ENABLE, 1, &tmp)) {
      ERROR("Fail to enable data ready interupt\n");
      return -1;
   }

   tmp = BIT_XG_FIFO_EN | BIT_YG_FIFO_EN | BIT_ZG_FIFO_EN |
         BIT_ACCEL_FIFO_EN;
   if (I2C_write(MPU6050_HW_ADDR, REG_FIFO_EN, 1, &tmp)) {
      ERROR("Fail to set FIFO measurments");
      return -1;
   }

   DEBUGF('m', "FIFO reset\n");
   return 0;
}

/***********************************************************************
 * Function Name  : MPU_SetBypass
 * Description    : Allow direct access to auxiliary I2C bus of the
 *                  device
 * Parameter[In]  : NONE
 * Parameter[Out] : NONE
 * Return         : 0 if Success
 **********************************************************************/
int MPU_SetBypass(void) {
   uint8_t tmp;

   if (I2C_read(MPU6050_HW_ADDR, REG_USER_CTRL, 1, &tmp)) {
      ERROR("Fail to read User Control Register\n");
      return -1;  
   }
            
   tmp &= ~BIT_I2C_MST_EN;
   if (I2C_write(MPU6050_HW_ADDR, REG_USER_CTRL, 1, &tmp)) {
      ERROR("Fail to disable I2C Master Mode\n");
      return -1;         
   }

   delay_ms(3);

   tmp = BIT_INT_LEVEL | BIT_I2C_BYPASS_EN;
   if (I2C_write(MPU6050_HW_ADDR, REG_INT_PIN_CFG, 1,&tmp)) {
      ERROR("Fail to disable I2C Master Mode\n");
      return -1;
   }
  
   DEBUGF('m', "MPU6050 Bypass Enable\n");
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
      ERROR("Fail to set device Sample Rate\n");
      return -1;
   }
   
   DEBUGF('m', "Sample Rate set at %dhz\n", rate);
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

/***********************************************************************
 * Function Name  : MPU_GetFIFO
 * Description    : Read raw measurements from FIFO buffer
 * Parameter[In]  : NONE
 * Parameter[Out] : accel  -> Raw accelerometer measurments
 *                  gyro   -> Raw gyroscope measurements   
 * Return         : 0 if Success
 **********************************************************************/
int MPU_GetFIFO(int16_t *accel, int16_t *gyro) {
   uint8_t tmp[12];

   if (I2C_read(MPU6050_HW_ADDR, REG_FIFO_COUNT, 2, tmp)) {
      ERROR("Fail to read FIFO count\n");
      return -1;
   }

   uint16_t count = (tmp[0] << 8) | tmp[1];
   if (count < 12) return 1;

   if (count > 512) {
      if (I2C_read(MPU6050_HW_ADDR, REG_INT_STATUS, 1, tmp)) {
         ERROR("Fail to read interrupt status\n");
         return -1;
      }
      if(tmp[0] & BIT_FIFO_OFLOW_INT) {
         ERROR("FIFO buffer overflow resetting buffer\n");
         MPU_ResetFIFO();
         return -1;
      }
   }

   if (I2C_read(MPU6050_HW_ADDR, REG_FIFO_R_W, 12, tmp)) {
      ERROR("Fail to read FIFO buffer\n");
      return -1;
   }

   accel[0] = (tmp[0] << 8) | tmp[1];
   accel[1] = (tmp[2] << 8) | tmp[3];
   accel[2] = (tmp[4] << 8) | tmp[5];
    
   gyro[0] = (tmp[6]  << 8) | tmp[7];
   gyro[1] = (tmp[8]  << 8) | tmp[9];
   gyro[2] = (tmp[10] << 8) | tmp[11];
 
   return 0;
}




