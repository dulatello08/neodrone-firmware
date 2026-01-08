#include "rc_input.h"
#include "samd21.h"

// Placeholder for receiver data
static rc_data_t current_rc_data;

void rc_input_init(void) {
  // TODO: Initialize UART SERCOM
  // TODO: Configure pins (RX/TX)

  // Initialize default safe values
  current_rc_data.throttle = 0.0f;
  current_rc_data.roll = 0.0f;
  current_rc_data.pitch = 0.0f;
  current_rc_data.yaw = 0.0f;
  current_rc_data.arm_switch = 0;
}

void rc_input_update(void) {
  // TODO: Read checks from UART buffer
  // TODO: Parse protocol (CRSF)

  // Mock data for compilation
}

uint8_t rc_input_get_data(rc_data_t *data) {
  *data = current_rc_data;
  return 1; // Return 1 if data is fresh/valid
}
