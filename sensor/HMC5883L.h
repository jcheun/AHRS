/***********************************************************************
 *  File Name     : HMC5883L.h
 *  Author        : Jesse Cheun
 *  Date          : 06/20/2014
 *  Description   : Honneywell HMC5883L Sensor Driver Library
 **********************************************************************/

#ifndef __HMC5883L_H__
#define __HMC5883L_H__

/*-- Includes --------------------------------------------------------*/

/*-- Registers -------------------------------------------------------*/
/* HMC5883L Hardware Address */
#define HMC5883L_HW_ADDR   (0x1E)

/* Configuration Register A */
#define HMC_REG_CONFIG_A   (0x00)
#define BIT_SMP_AVG_1      (0x00)
#define BIT_SMP_AVG_2      (0x20)
#define BIT_SMP_AVG_3      (0x40) 
#define BIT_SMP_AVG_8      (0x60)

#define BIT_SMP_RATE_0P75  (0x00)
#define BIT_SMP_RATE_1P5   (0x04)
#define BIT_SMP_RATE_3     (0x08)
#define BIT_SMP_RATE_7P5   (0x0C)
#define BIT_SMP_RATE_15    (0x10)
#define BIT_SMP_RATE_30    (0x14)
#define BIT_SMP_RATE_75    (0x18)
#define BIT_SMP_RATE_RES   (0x1C)


/* Configuration Register B */
#define HMC_REG_CONFIG_B   (0x01)
#define BIT_GAIN_1370      (0x00)
#define BIT_GAIN_1090      (0x20)
#define BIT_GAIN_820       (0x40)
#define BIT_GAIN_660       (0x60)
#define BIT_GAIN_440       (0x80)
#define BIT_GAIN_390       (0xA0)
#define BIT_GAIN_330       (0xC0)
#define BIT_GAIN_230       (0xE0)

/* Mode Register */
#define HMC_REG_MODE       (0x02)
#define BIT_MODE_CONT      (0x00)
#define BIT_MODE_SINGLE    (0x01) 
#define BIT_MODE_IDLE      (0x02) 

/* Magnetometer Measurements */
#define HMC_REG_MAG_OUT    (0x03)

/* Status Register */
#define HMC_REG_STATUS     (0x09)
#define BIT_RDY            (0x01)
#define BIT_LOCK           (0x02)

/* Identification Registers */
#define HMC_REG_ID         (0x0A)


int HMC_Init(void);
int HMC_SelfTest(void);
int HMC_GetMag(int16_t *data);
#endif
