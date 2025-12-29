#include <stdint.h>
#include "samd21.h" // Placeholder, user will need to provide CMSIS headers or similar

// Simple delay function
void delay(uint32_t count) {
    while (count--) {
        __asm("nop");
    }
}

int main(void) {
    // System Init placeholder
    // SystemInit();

    // Main loop
    while (1) {
        // Todo: Toggle LED or do something
        delay(100000);
    }

    return 0;
}
