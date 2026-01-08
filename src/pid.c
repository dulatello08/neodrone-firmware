#include "pid.h"

void pid_init(pid_controller_t *pid, float kp, float ki, float kd,
              float i_limit, float out_limit) {
  pid->Kp = kp;
  pid->Ki = ki;
  pid->Kd = kd;
  pid->i_limit = i_limit;
  pid->out_limit = out_limit;
  pid_reset(pid);
}

void pid_reset(pid_controller_t *pid) {
  pid->prev_error = 0.0f;
  pid->integral = 0.0f;
}

float pid_update(pid_controller_t *pid, float setpoint, float measurement,
                 float dt) {
  float error = setpoint - measurement;

  // Proportional
  float p_term = pid->Kp * error;

  // Integral
  pid->integral += error * dt;
  // Anti-windup
  if (pid->integral > pid->i_limit) {
    pid->integral = pid->i_limit;
  } else if (pid->integral < -pid->i_limit) {
    pid->integral = -pid->i_limit;
  }
  float i_term = pid->Ki * pid->integral;

  // Derivative
  float derivative = (error - pid->prev_error) / dt;
  float d_term = pid->Kd * derivative;

  // Update state
  pid->prev_error = error;

  // Calculate total output
  float output = p_term + i_term + d_term;

  // Output limiting
  if (output > pid->out_limit) {
    output = pid->out_limit;
  } else if (output < -pid->out_limit) {
    output = -pid->out_limit;
  }

  return output;
}
