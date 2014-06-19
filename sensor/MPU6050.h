/***********************************************************************
 *  File Name     : MPU6050.h
 *  Author        : Jesse Cheun
 *  Date          : 06/15/2014
 *  Description   : Invensense MPU6050 Senser Driver Library
 **********************************************************************/

#ifndef __MPU6050_H__
#define __MPU6050_H__

/*-- Includes --------------------------------------------------------*/

/*-- Registers -------------------------------------------------------*/
/* MPU6050 Hardware Address */
#define MPU6050_HW_ADDR    (0x68)

/* Sample Rate Divider */
#define REG_SMPRT_DIV      (0x19)

/* Configuration */
#define REG_CONFIG         (0x1A)
#define BIT_DLPF_CFG_256   (0x00) 
#define BIT_DLPF_CFG_188   (0x01)
#define BIT_DLPF_CFG_98    (0x02)
#define BIT_DLPF_CFG_42    (0x03)
#define BIT_DLPF_CFG_20    (0x04)
#define BIT_DLPF_CFG_10    (0x05)
#define BIT_DLPF_CFG_5     (0x06)
#define BIT_DLPF_CFG_RES   (0x07)

/* Gyroscope Configuration */
#define REG_GYRO_CONFIG    (0x1B)
#define BIT_GFS_SEL_250    (0x00)
#define BIT_GFS_SEL_500    (0x08)
#define BIT_GFS_SEL_1000   (0x10)
#define BIT_GFS_SEL_2000   (0x18)

/* Accelerometer Configuration */
#define REG_ACCEL_CONFIG   (0x1C)
#define BIT_AFS_SEL_2G     (0x00)
#define BIT_AFS_SEL_4G     (0x08)
#define BIT_AFS_SEL_8G     (0x10)
#define BIT_AFS_SEL_16G    (0x18)

/* FIFO Enable */
#define REG_FIFO_EN        (0x23)
#define BIT_XG_FIFO_EN     (0x40)
#define BIT_YG_FIFO_EN     (0x20)
#define BIT_ZG_FIFO_EN     (0x10)
#define BIT_ACCEL_FIFO_EN  (0x08)

/* Accelerometer Measurements */
#define REG_ACCEL_OUT      (0x3B)

/* Gyroscope Measurements */
#define REG_GYRO_OUT       (0x43)

/* User Control */
#define REG_USER_CTRL      (0x6A)
#define BIT_FIFO_EN        (0x40)

/* Power Management 1 */
#define REG_PWR_MGMT_1     (0x6B)
#define BIT_DEVICE_RESET   (0x80)
#define BIT_SLEEP          (0x40)
#define BIT_CLKSEL_X       (0x01)

/* Power Management 2 */
#define REG_PWR_MGMT_2     (0x6C)

/* FIFO Count Registers */
#define REG_FIFO_COUNT     (0x72)

/* FIFO Read Write */
#define REG_FIFO_R_W       (0x74)

/* WHO_AM_I */
#define REG_WHO_AM_I       (0x75)

int MPU_Init(uint8_t gyroFsr, uint8_t accelFsr,
             uint16_t sampleRate, uint8_t lpf);

int MPU_GetAccel(int16_t *data);
int MPU_GetGyro(int16_t *data);
int MPU_GetSensors(int16_t *accel, int16_t *gyro);
int MPU_SetSampleRate(uint16_t rate, uint8_t lpf);

#endif
