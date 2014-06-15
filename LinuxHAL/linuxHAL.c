/***********************************************************************
 *  File Name     : linuxHAL.c
 *  Author        : Jesse Cheun
 *  Date          : 06/14/2014
 *  Description   : Simple Linux I2C Communcation and System Delay 
 **********************************************************************/

/*-- Includes --------------------------------------------------------*/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#include "debugf.h"
#include "linuxHAL.h"

/*-- Function Prototypes ---------------------------------------------*/
int  I2C_setSlave(int sAddr);
int  I2C_open(void);
void I2C_close(void);

/*-- Variables -------------------------------------------------------*/
int  I2C_file = -1;
char I2C_filename[11];

/***********************************************************************
 * Function Name  : I2C_init
 * Description    : Initialize I2C file path
 * Parameter[In]  : bus    -> Bus port number to set.
 * Parameter[Out] : NONE
 * Return         : NONE
 **********************************************************************/
void I2C_init(int bus) {
   I2C_close();
   sprintf(I2C_filename, "/dev/i2c-%d", bus);
   DEBUGF('i', "I2C bus set %s\n", I2C_filename);
}

/***********************************************************************
 * Function Name  : I2C_open
 * Description    : Open I2C port
 * Parameter[In]  : NONE
 * Parameter[Out] : NONE
 * Return         : 0 if Success 
 **********************************************************************/
int I2C_open(void) {
   if(I2C_file >= 0) return 0;
   I2C_file = open(I2C_filename, O_RDWR);
   if (I2C_file < 0) {
      fprintf(stderr, "Error: Could not open file %s: %s\n",
              I2C_filename, strerror(errno));
      return -1;
   }

   DEBUGF('i', "I2C bus %s opened\n", I2C_filename);
   return 0;
}

/***********************************************************************
 * Function Name  : I2C_close
 * Description    : Close I2C port
 * Parameter[In]  : NONE
 * Parameter[Out] : NONE
 * Return         : NONE
 **********************************************************************/
void I2C_close(void) {
   if (I2C_file >= 0) {
      close(I2C_file);
      I2C_file = -1;
   }
   DEBUGF('i', "I2C bus %s closed\n", I2C_filename);
}

/***********************************************************************
 * Function Name  : I2C_setSlave
 * Description    : Set I2C Slave Address
 * Parameter[In]  : sAddr  -> I2C Slave Address 
 * Parameter[Out] : NONE
 * Return         : 0 if Success
 **********************************************************************/
int I2C_setSlave(int sAddr) {
   if (ioctl(I2C_file, I2C_SLAVE, sAddr) < 0) {
      fprintf(stderr, "Error: Could not set slave address 0x%02x: %s\n",
              sAddr, strerror(errno)); 
      return -1;
   }

   DEBUGF('i', "Slave Address 0x%02x set\n", sAddr);
   return 0;
}

/***********************************************************************
 * Function Name  : I2C_read
 * Description    : Read bytes from device
 * Parameter[In]  : sAddr  -> I2C Slave Address
 *                  rAddr  -> Device Register Address
 *                  length -> Number of bytes to read
 * Parameter[Out] : data   -> store bytes read from device
 * Return         : 0 if Success
 **********************************************************************/
int I2C_read(uint8_t sAddr, uint8_t rAddr,
             uint8_t length, uint8_t *data) {
   if (I2C_write(sAddr, rAddr, 0, NULL))
      return -1;

   int bytes;
   for (int tries = 1; tries <= 3 && bytes != length; ++tries) {
      bytes = read(I2C_file, data, length);
      if (bytes < 0) {
         fprintf(stderr, "Error: Fail to read data from device: %s\n",
                 strerror(errno));
         return -1;
      }
      
      if (bytes != length) {
         fprintf(stderr, "Error: %d - Read %d bytes of %d bytes: %s\n",
                 tries, bytes, length, strerror(errno));
      }
   }

   if (bytes != length) {
      fprintf(stderr, "Error: Unable to read %d bytes from device\n",
              length);
      return -1;
   }

   DEBUGF('i', "Read %d bytes from device 0x%02x\n", length, sAddr);
   return 0;
}

/***********************************************************************
 * Function Name  : I2C_write
 * Description    : Write bytes to device
 * Parameter[In]  : sAddr  -> I2C Slave Address 
 *                  rAddr  -> Device Register Address
 *                  length -> Number of bytes to write
 *                  data   -> bytes to write to device
 * Parameter[Out] : NONE
 * Return         : 0 if Success
 **********************************************************************/
int I2C_write(uint8_t sAddr, uint8_t rAddr, 
              uint8_t length, uint8_t const *data) {
   int written;

   if (I2C_open()) return -1;
   if (I2C_setSlave(sAddr)) return -1;
 
   if (length == 0) {
      if (write(I2C_file, &rAddr, 1) != 1) {
         fprintf(stderr, "Error: Fail to write register address 0x%02x\
                 : %s\n", rAddr,strerror(errno));
         return -1;
      }
   } else {
      char tx_data[length + 1];
      tx_data[0] = rAddr;
      for(int i = 0; i < length; ++i) 
         tx_data[i+1] = data[i];
      
      written = write(I2C_file, tx_data, length + 1);
      if (written < 0) {
         fprintf(stderr, "Error: Fail to write data to device :%s\n",
                 strerror(errno));
         return -1;
      }
      if (written < length) {
         fprintf(stderr, "Error: Written %d bytes of %d bytes: %s\n",
                 written, length, strerror(errno));
         return -1;
      }
   }
   
   DEBUGF('i', "Written %d bytes to device 0x%02x\n", length, sAddr);
   return 0;
}

/***********************************************************************
 * Function Name  : delay_ms
 * Description    : Delay system for a period of time
 * Parameter[In]  : ms    -> Milliseconds 
 * Parameter[Out] : NONE
 * Return         : 0 if Success
 **********************************************************************/
int delay_ms(uint64_t ms) {
   struct timespec time;
   time.tv_sec  =  ms / 1000;
   time.tv_nsec = (ms % 1000) * 1000000L;
   return nanosleep(&time, NULL);
}

