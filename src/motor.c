#include "motor.h"
#include "samd21.h"

// 5th pin is Emergency Stop

void motor_init(void) {
  // TODO: Init PWM/TCC instances
  // TODO: Init GPIO pins
  // TODO: Init Emergency Stop GPIO
}

void motor_set_output(float m1, float m2, float m3, float m4) {
  // TODO: Update TCC CC registers
}

void motor_emergency_stop(void) {
  // TODO: Pull Stop Pin HIGH/LOW
  // TODO: Disable PWM
}

void motor_enable(void) {
  // TODO: Enable PWM output
}
