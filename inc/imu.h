#ifndef IMU_H
#define IMU_H

#include <stdint.h>

typedef struct {
  float accel_x; // m/s^2
  float accel_y;
  float accel_z;
  float gyro_x; // rad/s
  float gyro_y;
  float gyro_z;
  float temp;
} imu_data_t;

void imu_init(void);
uint8_t imu_read(imu_data_t *data);

#endif // IMU_H
