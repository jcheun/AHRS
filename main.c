#include <errno.h>
#include <libgen.h> 
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>

#include "debugf.h"
#include "linuxHAL.h"
#include "MPU6050.h"

int main(int argc, char** argv) {
   struct timeval start, stop;
   int16_t accel[3], gyro[3];
   
   Exec_Name = basename(argv[0]);
   if (getopt(argc, argv, "@:") == '@') set_debugflags(optarg);
   
   I2C_init(1);
   
   MPU_Init(BIT_GFS_SEL_1000, BIT_AFS_SEL_4G, 1000, BIT_DLPF_CFG_256);
   int i = 0; 
   gettimeofday(&start, NULL);

   while (i++ < 1000) {
      MPU_GetSensors(accel, gyro);
      printf("Accel: %d\t%d\t%d\t", accel[0], accel[1], accel[2]);
      printf("Gyro: %d\t%d\t%d\n",  gyro[0], gyro[1], gyro[2]);
   }

   gettimeofday(&stop, NULL);
   
   double s = (double)start.tv_sec + (double)start.tv_usec * 1e-6;
   double e = (double)stop.tv_sec + (double)stop.tv_usec * 1e-6;
   printf("Finish in about %f seconds.\n", e - s);
   return 0;
}
