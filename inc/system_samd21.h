#ifndef _SYSTEM_SAMD21_H_
#define _SYSTEM_SAMD21_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock) */

/**
 * \brief Setup the microcontroller system.
 *
 * Initialize the System and update the SystemCoreClock variable.
 */
void SystemInit(void);

/**
 * \brief Updates the SystemCoreClock with current core Clock retrieved from cpu
 * registers.
 */
void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* _SYSTEM_SAMD21_H_ */
