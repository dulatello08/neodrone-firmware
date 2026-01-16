#include "imu.h"
#include "icm20602_regs.h"
#include "samd21.h"
#include <stdint.h>
#include <string.h>

// Hardware Configuration (Modify as needed for specific board)
#define IMU_SERCOM SERCOM4
#define IMU_SERCOM_PMUX PORT_PMUX_PMUXE_D_Val
#define IMU_SERCOM_GCLK_ID SERCOM4_GCLK_ID_CORE
#define IMU_SERCOM_APBCMASK PM_APBCMASK_SERCOM4

// Pins: DI=PA12, DO=PB10, SCK=PB11, CS=PA13, INT=PA14
#define PIN_DI 12  // PA12 (Group 0)
#define PIN_DO 10  // PB10 (Group 1)
#define PIN_SCK 11 // PB11 (Group 1)
#define PIN_CS 13  // PA13 (Group 0)
#define PIN_INT 14 // PA14 (Group 0)

// SPI Helper Functions
static void spi_init(void) {
  // 1. Enable Power to SERCOM
  PM->APBCMASK.reg |= IMU_SERCOM_APBCMASK;

  // 2. Configure GCLK (Use GCLK0 ~48MHz usually)
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(IMU_SERCOM_GCLK_ID) |
                      GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN;
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;

  // 3. Configure Port Pins
  // DI (PA12, PAD0)
  PORT->Group[0].PINCFG[PIN_DI].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[PIN_DI >> 1].reg |=
      (PIN_DI & 1) ? PORT_PMUX_PMUXO(IMU_SERCOM_PMUX)
                   : PORT_PMUX_PMUXE(IMU_SERCOM_PMUX);

  // DO (PB10, PAD2)
  PORT->Group[1].PINCFG[PIN_DO].bit.PMUXEN = 1;
  PORT->Group[1].PMUX[PIN_DO >> 1].reg |=
      (PIN_DO & 1) ? PORT_PMUX_PMUXO(IMU_SERCOM_PMUX)
                   : PORT_PMUX_PMUXE(IMU_SERCOM_PMUX);

  // SCK (PB11, PAD3)
  PORT->Group[1].PINCFG[PIN_SCK].bit.PMUXEN = 1;
  PORT->Group[1].PMUX[PIN_SCK >> 1].reg |=
      (PIN_SCK & 1) ? PORT_PMUX_PMUXO(IMU_SERCOM_PMUX)
                    : PORT_PMUX_PMUXE(IMU_SERCOM_PMUX);

  // CS (PA13), GPIO Out
  PORT->Group[0].DIRSET.reg = (1 << PIN_CS);
  PORT->Group[0].OUTSET.reg = (1 << PIN_CS); // Set High

  // INT (PA14), GPIO In
  PORT->Group[0].DIRCLR.reg = (1 << PIN_INT);
  PORT->Group[0].PINCFG[PIN_INT].bit.INEN = 1;

  // 4. Configure SERCOM SPI
  IMU_SERCOM->SPI.CTRLA.bit.SWRST = 1;
  while (IMU_SERCOM->SPI.SYNCBUSY.bit.SWRST)
    ;

  IMU_SERCOM->SPI.CTRLA.reg =
      SERCOM_SPI_CTRLA_MODE_SPI_MASTER |
      SERCOM_SPI_CTRLA_DIPO(0) | // MISO on PAD0 (PA12)
      SERCOM_SPI_CTRLA_DOPO(1);  // MOSI on PAD2 (PB10), SCK on PAD3 (PB11)

  IMU_SERCOM->SPI.CTRLB.reg = SERCOM_SPI_CTRLB_RXEN; // Enable Receiver

  IMU_SERCOM->SPI.BAUD.reg = 1; // High speed

  IMU_SERCOM->SPI.CTRLA.bit.ENABLE = 1;
  while (IMU_SERCOM->SPI.SYNCBUSY.bit.ENABLE)
    ;
}

static uint8_t spi_transfer(uint8_t data) {
  while (!IMU_SERCOM->SPI.INTFLAG.bit.DRE)
    ; // Wait for Data Register Empty
  IMU_SERCOM->SPI.DATA.reg = data;

  while (!IMU_SERCOM->SPI.INTFLAG.bit.RXC)
    ; // Wait for Receive Complete
  return (uint8_t)IMU_SERCOM->SPI.DATA.reg;
}

static void imu_cs_low(void) { PORT->Group[0].OUTCLR.reg = (1 << PIN_CS); }

static void imu_cs_high(void) { PORT->Group[0].OUTSET.reg = (1 << PIN_CS); }

static void imu_write_reg(uint8_t reg, uint8_t value) {
  imu_cs_low();
  spi_transfer(reg); // Write, bit 7 is 0
  spi_transfer(value);
  imu_cs_high();
}

static uint8_t imu_read_reg(uint8_t reg) {
  uint8_t value;
  imu_cs_low();
  spi_transfer(reg | 0x80); // Read, set bit 7
  value = spi_transfer(0x00);
  imu_cs_high();
  return value;
}

void imu_init(void) {
  spi_init();

  // Reset ICM20602
  imu_write_reg(ICM_PWR_MGMT_1, ICM_PWR_MGMT_1_RESET);
  for (volatile int i = 0; i < 100000; i++)
    ; // Short delay

  // Wake up and set clock source
  imu_write_reg(ICM_PWR_MGMT_1, ICM_PWR_MGMT_1_CLKSEL);

  // Disable I2C Interface
  imu_write_reg(ICM_USER_CTRL, ICM_USER_CTRL_I2C_IF_DIS);

  // Config Accel / Gyro ranges
  imu_write_reg(ICM_GYRO_CONFIG, 0x18);  // 2000 dps
  imu_write_reg(ICM_ACCEL_CONFIG, 0x18); // 16g
}

uint8_t imu_read(imu_data_t *data) {
  uint8_t raw[14];

  // Burst read starting from ACCEL_XOUT_H
  imu_cs_low();
  spi_transfer(ICM_ACCEL_XOUT_H | 0x80);
  for (int i = 0; i < 14; i++) {
    raw[i] = spi_transfer(0x00);
  }
  imu_cs_high();

  // Convert to meaningful units (simplified)
  int16_t ax = (raw[0] << 8) | raw[1];
  int16_t ay = (raw[2] << 8) | raw[3];
  int16_t az = (raw[4] << 8) | raw[5];
  int16_t t = (raw[6] << 8) | raw[7];
  int16_t gx = (raw[8] << 8) | raw[9];
  int16_t gy = (raw[10] << 8) | raw[11];
  int16_t gz = (raw[12] << 8) | raw[13];

  // Placeholder scaling (approx needed for correct units)
  data->accel_x = ax / 2048.0f;
  data->accel_y = ay / 2048.0f;
  data->accel_z = az / 2048.0f;

  data->gyro_x = gx / 16.4f;
  data->gyro_y = gy / 16.4f;
  data->gyro_z = gz / 16.4f;

  data->temp = (t / 326.8f) + 25.0f;

  return 1;
}
