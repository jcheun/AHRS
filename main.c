#include <errno.h>
#include <libgen.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#include "debugf.h"
#include "linuxHAL.h"
#include "MPU6050.h"

int main(int argc, char** argv) {
   time_t start,stop;
   Exec_Name = basename(argv[0]);
   if (getopt(argc, argv, "@:") == '@') set_debugflags(optarg);
   
   I2C_init(1);
   MPU_Init(BIT_GFS_SEL_1000, BIT_AFS_SEL_4G);
   int16_t accel[3];
   int16_t gyro[3];
   //int i = 0;
   time(&start);
   //while (i++ < 1000) {
      //MPU_GetSensors(accel, gyro);
      //MPU_GetAccel(accel);
      //MPU_GetGyro(gyro);
      //printf("Accel: %d\t%d\t%d\t", accel[0], accel[1], accel[2]);
      //printf("Gyro: %d\t%d\t%d\n",  gyro[0], gyro[1], gyro[2]);
  // }
   delay_ms(1500);
   time(&stop);
   printf("Finish in about %f seconds.\n", difftime(stop,start));
   return 0;
}
