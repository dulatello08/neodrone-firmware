# NeoDrone Firmware (ATSAMD21)

Simple bare-metal firmware for ATSAMD21G1BA. Firmware will not be that complicated as we don't have to handle commutation. This project is not going to use RTOS, either. The basic idea is to get input from RC module (UART) and input from IMU (SPI), and output 4 PWM signals. total of 5 outputs, 5th output pin being emergency stop. The basic idea gets very complicated very fast when we bring in control theory, this code will handle PID control and etc.

## Structure
- `src/`: Source code (main.c, startup files)
- `inc/`: Include files
- `linker/`: Linker script
- `Makefile`: Build configuration

## Dependencies
- ARM GCC Toolchain (`arm-none-eabi-gcc`)
- OpenOCD (for flashing)

## Building
Run `make` to build the firmware.

## Flashing
Run `make flash` to program via SWD (todo: configure `openocd.cfg` first).
