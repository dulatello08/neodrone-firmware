#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

void motor_init(void);
void motor_set_output(float m1, float m2, float m3,
                      float m4); // 0.0 to 1.0 (or PWM duty)
void motor_emergency_stop(void);
void motor_enable(void);

#endif // MOTOR_H
