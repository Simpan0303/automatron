#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

volatile uint32_t *portDAddress = (volatile uint32_t *) &PORTD;




// Function to get the status of the switches
int getsw(void) {
    return ((PORTD >> 8) & 0xF); // Shift right 8 bits and mask with 0xF to get the 4 least significant bits
}

// Function to get the status of the buttons
int getbtns(void) {
    return ((PORTD >> 5) & 0x7); // Shift right 5 bits and mask with 0x7 to get the 3 least significant bits
}
