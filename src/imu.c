#include "imu.h"
#include "samd21.h"

// Placeholder driver for IMU

void imu_init(void) {
  // TODO: Init SPI SERCOM
  // TODO: Init CS pin
  // TODO: Send config to IMU registers
}

uint8_t imu_read(imu_data_t *data) {
  // TODO: Read SPI registers

  // Mock data
  data->accel_x = 0;
  data->accel_y = 0;
  data->accel_z = 9.8f;
  return 1;
}
