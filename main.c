#include <errno.h>
#include <libgen.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>

#include "debugf.h"
#include "linuxHAL.h"
#include "HMC5883L.h"
#include "MPU6050.h"
#include "MahonyAHRS.h"

#define MPU6050_DEG_PER_LSB_2000 (float)((2 * 2000.0) / 65536.0)
#define MPU6050_G_PER_LSB_8      (float)((2 * 8) / 65536.0)

int main (int argc, char** argv) {
   struct timeval start, stop;
   int16_t accel[3], gyro[3], mag[3];
   long  avg_accel[3], avg_gyro[3];

   Exec_Name = basename (argv[0]);
   if (getopt(argc, argv, "@:") == '@') set_debugflags(optarg);
   I2C_init (1);
   
   MPU_Init (BIT_GFS_SEL_250, BIT_AFS_SEL_16G, 1000, BIT_DLPF_CFG_188);
   //HMC_Init ();
   delay_ms (1000);   
    
   gettimeofday (&start, NULL);
   
   uint8_t tmp[6];
   long gx = 56;
   long gy = -31;
   long gz = 16;

   tmp[0] = (gx >> 8) & 0xff; 
   tmp[1] = (gx) & 0xff;
   
   tmp[2] = (gy >> 8) & 0xff; 
   tmp[3] = (gy) & 0xff;

   tmp[4] = (gz >> 8) & 0xff; 
   tmp[5] = (gz) & 0xff;

   if (I2C_write(MPU6050_HW_ADDR, 0x13, 2, &tmp[0])) {
      ERROR("Fail to set Gyroscope X bias\n");
      return -1;
   }

   if (I2C_write(MPU6050_HW_ADDR, 0x15, 2, &tmp[2])) {
      ERROR("Fail to set Gyroscope Y bias\n");
      return -1;
   }

   if (I2C_write(MPU6050_HW_ADDR, 0x17, 2, &tmp[4])) {
      ERROR("Fail to set Gyroscope Z bias\n");
      return -1;
   }

for (int i  = 0; i < 1; ++i) {
   printf ("--------------------------------------------------------------------------\n");
   uint8_t data;   
   /* Set Gyroscope Full Scale Range */
   data = BIT_GFS_SEL_250;
   if (I2C_write(MPU6050_HW_ADDR, REG_GYRO_CONFIG, 1, &data)) {
      ERROR("Fail to set Gyroscope Full Scale Range\n");
      return -1;
   }

   /* Set Accelerometer Full Scale Range */
   data = BIT_AFS_SEL_2G;
   if (I2C_write(MPU6050_HW_ADDR, REG_ACCEL_CONFIG, 1, &data)) {
      ERROR("Fail to set Accelerometer Full Scale Range\n");
      return -1;
   }

   delay_ms (500);
   avg_accel[0] = 0;
   avg_accel[1] = 0;
   avg_accel[2] = 0;
   avg_gyro[0] = 0;
   avg_gyro[1] = 0;
   avg_gyro[2] = 0;
 
   for (int i = 0; i < 100; ++i) {
      MPU_GetSensors(accel, gyro);
      printf("--- Accel: %07d %07d %07d  ", accel[0] , accel[1], accel[2]);
      printf("Gyro: %03d %03d %03d\n", gyro[0] , gyro[1], gyro[2]);
      avg_accel[0] += accel[0];
      avg_accel[1] += accel[1];
      avg_accel[2] += accel[2];
      avg_gyro[0] += gyro[0];
      avg_gyro[1] += gyro[1];
      avg_gyro[2] += gyro[2];
      delay_ms (50);
   }
   printf("tot Accel: %07ld %07ld %07ld  ", avg_accel[0], avg_accel[1], avg_accel[2]);
   printf("Gyro: %03ld %03ld %03ld\n", avg_gyro[0], avg_gyro[1], avg_gyro[2]);
   printf("AVG Accel: %07ld %07ld %07ld  ", avg_accel[0]/100, avg_accel[1]/100, avg_accel[2]/100);
   printf("Gyro: %03ld %03ld %03ld\n", avg_gyro[0]/100, avg_gyro[1]/100, avg_gyro[2]/100);
   
   printf ("\n\n");
   data = BIT_GFS_SEL_500;
   if (I2C_write(MPU6050_HW_ADDR, REG_GYRO_CONFIG, 1, &data)) {
      ERROR("Fail to set Gyroscope Full Scale Range\n");
      return -1;
   }

   /* Set Accelerometer Full Scale Range */
   data = BIT_AFS_SEL_4G;
   if (I2C_write(MPU6050_HW_ADDR, REG_ACCEL_CONFIG, 1, &data)) {
      ERROR("Fail to set Accelerometer Full Scale Range\n");
      return -1;
   }

   if (1) delay_ms (500);
   avg_accel[0] = 0;
   avg_accel[1] = 0;
   avg_accel[2] = 0;
   avg_gyro[0] = 0;
   avg_gyro[1] = 0;
   avg_gyro[2] = 0;
 
   for (int i = 0; i < 100; ++i) {
      MPU_GetSensors(accel, gyro);
      //printf("--- Accel: %07d %07d %07d  ", accel[0] , accel[1], accel[2]);
      //printf("Gyro: %03d %03d %03d\n", gyro[0] , gyro[1], gyro[2]);
       avg_accel[0] += accel[0];
      avg_accel[1] += accel[1];
      avg_accel[2] += accel[2];
      avg_gyro[0] += gyro[0];
      avg_gyro[1] += gyro[1];
      avg_gyro[2] += gyro[2];
      delay_ms (50);
   }
    printf("tot Accel: %07ld %07ld %07ld  ", avg_accel[0], avg_accel[1], avg_accel[2]);
   printf("Gyro: %03ld %03ld %03ld\n", avg_gyro[0], avg_gyro[1], avg_gyro[2]);
   printf("AVG Accel: %07ld %07ld %07ld  ", avg_accel[0]/100, avg_accel[1]/100, avg_accel[2]/100);
   printf("Gyro: %03ld %03ld %03ld\n", avg_gyro[0]/100, avg_gyro[1]/100, avg_gyro[2]/100);
   
   printf ("\n\n");
   data = BIT_GFS_SEL_1000;
   if (I2C_write(MPU6050_HW_ADDR, REG_GYRO_CONFIG, 1, &data)) {
      ERROR("Fail to set Gyroscope Full Scale Range\n");
      return -1;
   }

   /* Set Accelerometer Full Scale Range */
   data = BIT_AFS_SEL_8G;
   if (I2C_write(MPU6050_HW_ADDR, REG_ACCEL_CONFIG, 1, &data)) {
      ERROR("Fail to set Accelerometer Full Scale Range\n");
      return -1;
   }

   if (1) delay_ms (500);
   avg_accel[0] = 0;
   avg_accel[1] = 0;
   avg_accel[2] = 0;
   avg_gyro[0] = 0;
   avg_gyro[1] = 0;
   avg_gyro[2] = 0;
 
   for (int i = 0; i < 100; ++i) {
      MPU_GetSensors(accel, gyro);
      //printf("--- Accel: %07d %07d %07d  ", accel[0] , accel[1], accel[2]);
     // printf("Gyro: %03d %03d %03d\n", gyro[0] , gyro[1], gyro[2]);
       avg_accel[0] += accel[0];
      avg_accel[1] += accel[1];
      avg_accel[2] += accel[2];
      avg_gyro[0] += gyro[0];
      avg_gyro[1] += gyro[1];
      avg_gyro[2] += gyro[2];
      delay_ms (50);
   }
   printf("tot Accel: %07ld %07ld %07ld  ", avg_accel[0], avg_accel[1], avg_accel[2]);
   printf("Gyro: %03ld %03ld %03ld\n", avg_gyro[0], avg_gyro[1], avg_gyro[2]);
   printf("AVG Accel: %07ld %07ld %07ld  ", avg_accel[0]/100, avg_accel[1]/100, avg_accel[2]/100);
   printf("Gyro: %03ld %03ld %03ld\n", avg_gyro[0]/100, avg_gyro[1]/100, avg_gyro[2]/100);
 

   printf ("\n\n");
   data = BIT_GFS_SEL_2000;
   if (I2C_write(MPU6050_HW_ADDR, REG_GYRO_CONFIG, 1, &data)) {
      ERROR("Fail to set Gyroscope Full Scale Range\n");
      return -1;
   }

   /* Set Accelerometer Full Scale Range */
   data = BIT_AFS_SEL_16G;
   if (I2C_write(MPU6050_HW_ADDR, REG_ACCEL_CONFIG, 1, &data)) {
      ERROR("Fail to set Accelerometer Full Scale Range\n");
      return -1;
   }

   if (1) delay_ms (500);
   avg_accel[0] = 0;
   avg_accel[1] = 0;
   avg_accel[2] = 0;
   avg_gyro[0] = 0;
   avg_gyro[1] = 0;
   avg_gyro[2] = 0;
 
   for (int i = 0; i < 100; ++i) {
      MPU_GetSensors(accel, gyro);
      printf("--- Accel: %07d %07d %07d  ", accel[0] , accel[1], accel[2]);
      printf("Gyro: %03d %03d %03d\n", gyro[0] , gyro[1], gyro[2]);
       avg_accel[0] += accel[0];
      avg_accel[1] += accel[1];
      avg_accel[2] += accel[2];
      avg_gyro[0] += gyro[0];
      avg_gyro[1] += gyro[1];
      avg_gyro[2] += gyro[2];
      delay_ms (50);
   }
   printf("tot Accel: %07ld %07ld %07ld  ", avg_accel[0], avg_accel[1], avg_accel[2]);
   printf("Gyro: %03ld %03ld %03ld\n", avg_gyro[0], avg_gyro[1], avg_gyro[2]);
   printf("AVG Accel: %07ld %07ld %07ld  ", avg_accel[0]/100, avg_accel[1]/100, avg_accel[2]/100);
   printf("Gyro: %03ld %03ld %03ld\n", avg_gyro[0]/100, avg_gyro[1]/100, avg_gyro[2]/100);
 
}


   gettimeofday(&stop, NULL);
   
   double s = (double)start.tv_sec + (double)start.tv_usec * 1e-6;
   double e = (double)stop.tv_sec + (double)stop.tv_usec * 1e-6;
   printf("Finish in about %f seconds.\n", e - s);
   return 0;
}

 /*   while (i < 100) {
      //if(MPU_GetIntStatus(&status) == 0) {
      //   printf("Int status : %d\n", status);
      // }
      //if (MPU_GetFIFO(accel, gyro) == 0) {
      //  gyro[0] = 0;
      //  gyro[1] = 0; 
      //  gyro[2] = 0;
      //   MahonyAHRSupdateIMU ((float)gyro[0] * MPU6050_DEG_PER_LSB_2000 * M_PI / 180, (float)gyro[1] * MPU6050_DEG_PER_LSB_2000 * M_PI / 180, (float)gyro[2] * MPU6050_DEG_PER_LSB_2000 * M_PI / 180, 
      //                        (float)accel[0] * MPU6050_G_PER_LSB_8, (float)accel[1] * MPU6050_G_PER_LSB_8, (float)accel[2] * MPU6050_G_PER_LSB_8);
      //   printf("quat: %f\t%f\t%f\t%f\n", q0, q1, q2, q3);
      //   //++i; 
      //}
    //}

      //if (HMC_GetMag(mag) !=0) {
      //   heading = atan2(mag[1] * 0.92, mag[0] * 0.92);
      //   if (heading < 0) {
      //      heading += 2 * M_PI;
      //   }
      //   heading *= (180 / M_PI);
      //   printf("Mag: %06d %06d %06d %f\n",  mag[0], mag[1], mag[2], heading);
         //i++;
      //}
         MPU_GetSensors(accel, gyro);
         //if (accel[0] > max_accel[0]) max_accel[0] = accel[0];
         //if (accel[1] > max_accel[1]) max_accel[1] = accel[1];
         //if (accel[2] > max_accel[2]) max_accel[2] = accel[2];
         //if (accel[0] < min_accel[0]) min_accel[0] = accel[0];
         //if (accel[1] < min_accel[1]) min_accel[1] = accel[1];
         //if (accel[2] < min_accel[2]) min_accel[2] = accel[2];
         //printf("Gyro: %07d %07d %07d %07d %07d %07d\n", max_accel[0] , max_accel[1], max_accel[2], min_accel[0], min_accel[1], min_accel[2]);

         //printf("Accel: %05.2f\t%05.2f\t%05.2f\t", (float)accel[0] * MPU6050_G_PER_LSB_8, (float)accel[1] * MPU6050_G_PER_LSB_8, (float)accel[2] * MPU6050_G_PER_LSB_8);
         //printf("Gyro: %06.1f\t%06.1f\t%06.1f\n",  (float)gyro[0] * MPU6050_DEG_PER_LSB_2000 * M_PI / 180, 
         //                                          (float)gyro[1] * MPU6050_DEG_PER_LSB_2000 * M_PI / 180, 
         //                                          (float)gyro[2] * MPU6050_DEG_PER_LSB_2000 * M_PI / 180);

        //gyro[0] += 32;
        //gyro[1] -= 16; 
        //gyro[2] += 10;
        printf("Accel: %07d %07d %07d  ", accel[0] , accel[1], accel[2]);
        printf("Gyro: %03d %03d %03d\n", gyro[0] , gyro[1], gyro[2]);
      ++ i;
           accelt[0] += accel[0];
        accelt[1] += accel[1];
        accelt[2] += accel[2];

//}
        
         delay_ms (100);
   } */
