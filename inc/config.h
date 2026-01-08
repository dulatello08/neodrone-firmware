#ifndef CONFIG_H
#define CONFIG_H

// System
#define SYSTICK_FREQ_HZ 1000

// RC Input (UART)
// Define SERCOM and Pins here later
#define RC_UART_BAUDRATE 115200

// IMU (SPI)
// Define SERCOM and Pins here later

// Motor Outputs (PWM)
// Front Left, Front Right, Rear Right, Rear Left
#define MOTOR_COUNT 4
// Pin definitions to be added once hardware is finalized

// Emergency Stop
// Pin definition to be added

// Control Loop
#define LOOP_RATE_HZ 500
#define LOOP_TIME_US (1000000 / LOOP_RATE_HZ)

#endif // CONFIG_H
