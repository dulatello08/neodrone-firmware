#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef struct {
  float Kp;
  float Ki;
  float Kd;
  float i_limit;   // Integral windup limit
  float out_limit; // Output limit

  float prev_error;
  float integral;
} pid_controller_t;

void pid_init(pid_controller_t *pid, float kp, float ki, float kd,
              float i_limit, float out_limit);
void pid_reset(pid_controller_t *pid);
float pid_update(pid_controller_t *pid, float setpoint, float measurement,
                 float dt);

#endif // PID_H
