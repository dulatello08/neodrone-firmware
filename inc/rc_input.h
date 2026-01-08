#ifndef RC_INPUT_H
#define RC_INPUT_H

#include <stdint.h>

typedef struct {
  float throttle;     // 0.0 to 1.0
  float roll;         // -1.0 to 1.0 (or angle)
  float pitch;        // -1.0 to 1.0
  float yaw;          // -1.0 to 1.0
  uint8_t arm_switch; // 1 = Armed, 0 = Disarmed
  uint8_t mode_switch;
} rc_data_t;

void rc_input_init(void);
void rc_input_update(void); // Call in main loop or interrupt
uint8_t rc_input_get_data(rc_data_t *data);

#endif // RC_INPUT_H
