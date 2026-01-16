#ifndef ICM20602_REGS_H
#define ICM20602_REGS_H

// Device ID
#define ICM_WHO_AM_I 0x75
#define ICM_WHO_AM_I_VAL 0x12

// Power Management
#define ICM_PWR_MGMT_1 0x6B
#define ICM_PWR_MGMT_1_RESET 0x80
#define ICM_PWR_MGMT_1_CLKSEL 0x01 // Auto select clock

// Configuration
#define ICM_CONFIG 0x1A
#define ICM_GYRO_CONFIG 0x1B
#define ICM_ACCEL_CONFIG 0x1C
#define ICM_ACCEL_CONFIG2 0x1D
#define ICM_SMPLRT_DIV 0x19

// Data Registers
#define ICM_ACCEL_XOUT_H 0x3B
#define ICM_ACCEL_XOUT_L 0x3C
#define ICM_ACCEL_YOUT_H 0x3D
#define ICM_ACCEL_YOUT_L 0x3E
#define ICM_ACCEL_ZOUT_H 0x3F
#define ICM_ACCEL_ZOUT_L 0x40
#define ICM_TEMP_OUT_H 0x41
#define ICM_TEMP_OUT_L 0x42
#define ICM_GYRO_XOUT_H 0x43
#define ICM_GYRO_XOUT_L 0x44
#define ICM_GYRO_YOUT_H 0x45
#define ICM_GYRO_YOUT_L 0x46
#define ICM_GYRO_ZOUT_H 0x47
#define ICM_GYRO_ZOUT_L 0x48

// User Control (SPI/I2C)
#define ICM_USER_CTRL 0x6A
#define ICM_USER_CTRL_I2C_IF_DIS                                               \
  0x10 // Disable I2C to avoid bus contention if SPI used

#endif // ICM20602_REGS_H
