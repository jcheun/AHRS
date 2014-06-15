#include <errno.h>
#include <libgen.h> 
#include <stdio.h>
#include <unistd.h>


#include "debugf.h"
#include "linuxHAL.h"

int main(int argc, char** argv) {
   Exec_Name = basename(argv[0]);
   if (getopt(argc, argv, "@:") == '@') set_debugflags(optarg);
   printf("Hello World! %s\n", Exec_Name);
   I2C_init(1);
   unsigned char data[1];
   data[0] = 0;
   //I2C_read(0x68, 0x6B, 1, data);
   //printf("%d\n", data[0]);
   data[0] = 0x04;
   I2C_write(0x68, 0x6B, 1, data);


   return 0;
}
