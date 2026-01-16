#include "config.h"
#include "imu.h"
#include "motor.h"
#include "pid.h"
#include "rc_input.h"
#include "samd21.h"
#include <stdint.h>

// System ticks
volatile uint32_t uwTick;

void SysTick_Handler(void) { uwTick++; }

void delay_ms(uint32_t ms) {
  uint32_t tickstart = uwTick;
  while ((uwTick - tickstart) < ms) {
    // Wait
  }
}

// PID Controllers
pid_controller_t pid_roll;
pid_controller_t pid_pitch;
pid_controller_t pid_yaw;

int main(void) {
  // 1. System Initialization
  // Init clocks, buses, etc. (Assumed done or in init functions)

  // 2. Driver Initialization
  rc_input_init();
  imu_init();
  motor_init();

  // 3. PID Initialization
  // Tunings need real values
  pid_init(&pid_roll, 1.0f, 0.0f, 0.0f, 10.0f, 1.0f);
  pid_init(&pid_pitch, 1.0f, 0.0f, 0.0f, 10.0f, 1.0f);
  pid_init(&pid_yaw, 2.0f, 0.0f, 0.0f, 10.0f, 1.0f);

  // 4. Main Config
  rc_data_t rc_in;
  imu_data_t imu_in;

  uint32_t last_loop_time = 0;
  const uint32_t loop_interval_ms = 1000 / LOOP_RATE_HZ;

  while (1) {
    uint32_t current_time = uwTick;

    if (current_time - last_loop_time >= loop_interval_ms) {
      float dt = (float)(current_time - last_loop_time) / 1000.0f;
      last_loop_time = current_time;

      // --- 1. Read Inputs ---
      rc_input_update();
      rc_input_get_data(&rc_in);

      imu_read(&imu_in);

      // --- 2. Safety Check ---
      if (rc_in.arm_switch == 0) {
        motor_set_output(0, 0, 0, 0);
        pid_reset(&pid_roll);
        pid_reset(&pid_pitch);
        pid_reset(&pid_yaw);
        continue;
      }

      // --- 3. Control Logic (PID) ---
      // Calculate Errors (Setpoint vs Actual)
      // Roll
      float roll_out = pid_update(&pid_roll, rc_in.roll * 45.0f,
                                  0.0f /* Estimate roll from IMU */, dt);
      // Pitch
      float pitch_out = pid_update(&pid_pitch, rc_in.pitch * 45.0f,
                                   0.0f /* Estimate pitch from IMU */, dt);
      // Yaw
      float yaw_out = pid_update(&pid_yaw, rc_in.yaw * 100.0f, imu_in.gyro_z,
                                 dt); // Rate mode used often for yaw

      // --- 4. Mixing ---
      // Quad X configuration
      // M1 (FL): Throttle + Pitch + Roll - Yaw
      // M2 (FR): Throttle + Pitch - Roll + Yaw
      // M3 (RR): Throttle - Pitch - Roll - Yaw
      // M4 (RL): Throttle - Pitch + Roll + Yaw

      float m1 = rc_in.throttle + pitch_out + roll_out - yaw_out;
      float m2 = rc_in.throttle + pitch_out - roll_out + yaw_out;
      float m3 = rc_in.throttle - pitch_out - roll_out - yaw_out;
      float m4 = rc_in.throttle - pitch_out + roll_out + yaw_out;

      // Clamp and Output
      // clamp(val, 0.0, 1.0) logic needed here

      motor_set_output(m1, m2, m3, m4);
    }
  }
  return 0;
}
