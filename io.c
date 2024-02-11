#include <stdint.h>
#include <pic32mx.h>
#include "functiondefinitions.h"  /* Declarations for functions */

volatile uint32_t *portDAddress = (volatile uint32_t *) &PORTD;




// Function to get the status of the switches
int getsw(void) {
    return ((PORTD >> 8) & 0xF); // Shift right 8 bits and mask with 0xF to get the 4 least significant bits
}

// Function to get the status of the buttons

// btn2-4 are mapped to RD5-7
unsigned int getbtns(void) {
    return ((PORTD >> 5) & 0x7); // Shift right 5 bits and mask with 0x7 to get the 3 least significant bits
}

// btn1 is mapped to RF1
unsigned int getbtn1(void) {
    return (((PORTF >> 1) & 0x1)<<3); // if the button is pressed, return 0x200, else return 0
}
