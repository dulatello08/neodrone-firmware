#include <stdint.h>

extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;

// Forward declaration of the default fault handlers.
void Default_Handler(void);
void Reset_Handler(void);

// Basic Interrupt Handlers for Cortex-M0+
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

// External Main
extern int main(void);

// Vector Table
__attribute__((section(".vectors"))) const void *exception_table[] = {
    (void *)&_estack,          // Initial Stack Pointer
    (void *)Reset_Handler,     // Reset Handler
    (void *)NMI_Handler,       // NMI Handler
    (void *)HardFault_Handler, // Hard Fault Handler
    0,
    0,
    0,
    0, // Reserved
    0,
    0,
    0,                   // Reserved
    (void *)SVC_Handler, // SVCall Handler
    0,
    0,                       // Reserved
    (void *)PendSV_Handler,  // PendSV Handler
    (void *)SysTick_Handler, // SysTick Handler
};

void Reset_Handler(void) {
  uint32_t *pSrc, *pDest;

  // Initialize data section
  pSrc = &_etext;
  pDest = &_srelocate;

  if (pSrc != pDest) {
    while (pDest < &_erelocate) {
      *pDest++ = *pSrc++;
    }
  }

  // Initialize bss section to zero
  pDest = &_szero;
  while (pDest < &_ezero) {
    *pDest++ = 0;
  }

  // Call main
  main();

  // Infinite loop if main returns
  while (1)
    ;
}

void Default_Handler(void) {
  while (1)
    ;
}
