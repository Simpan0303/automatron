/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */




#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "functiondefinitions.h"  /* Declarations for functions */

#define INTCON_MVEC_MASK (1 << 12)

int mytime = 0x5957;

int prime = 1234567;

volatile int *trise;
volatile int *porte;

extern const uint8_t filled_square[][5];
int timeoutcount = 0; // Declare timeoutcount as a global variable

char textstring[] = "text, more text, and even more text!";

void user_isr( void ) 
{
  // Check if Timer 2 interrupt flag is set
  if (IFS(0) & 0x100) {
    // Clear Timer 2 interrupt flag
    IFSCLR(0) = 0x100;

    //time2string, display_string, display_update and tick
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
  }
}



/* Lab-specific initialization goes here */
void labinit( void )
{
  TRISD |= 0x0FE0;

  volatile int *trise = (volatile int*) 0xbf886100; // Define a volatile pointer to TRIS register of Port E
  *trise &= 0xFF00; // Set bits 7 through 0 as outputs

  porte = (volatile int*) 0xbf886110; // Define a volatile pointer to PORTE register
  *porte = 0; // Initialize the value to 0

  T2CON = 0x0; // Stop the timer and clear the control register
  TMR2 = 0x0; // Clear the timer register
  PR2 = 31250; // Load the period register with the value for 100ms
  T2CONSET = 0x70; // 1:256 Set prescaler (choose value based on calculation)
  IFSCLR(0) = 0x100; // Clear Timer 2 interrupt flag
  IECSET(0) = 0x100; // Enable Timer 2 interrupt
  T2CONSET = 0x8000; // Start Timer 2
  
  INTCONSET = INTCON_MVEC_MASK; // Enable global interrupts

  volatile int *ipc2 = (volatile int*) 0xbf8810b0; // Define a volatile pointer to IPC(2) register
  *ipc2 = (*ipc2 & ~0x1F) | (4 << 2) | 2; // Set priority level to 4 and subpriority level to 2
  /*
  INTEnableSystemMultiVectoredInt() is equivalent to the following:
  INTCONSET = _INTCON_MVEC_MASK;
  According to the PIC32MX documentation, 
  INTCON_MVEC_MASK is a bit mask for the MVEC bit in the INTCON register, 
  which is bit 12. So, you can define INTCON_MVEC_MASK as follows:
  #define INTCON_MVEC_MASK (1 << 12)
  */
}

void game_loop( void ) {
  // Game speed
	if (IFS(0) & 0x100) {
		IFSCLR(0) = 0x100;
		timeoutcount++;

		if (timeoutcount >= 1) //kske modifiera, ska inte vara nödvändigt
		{
			timeoutcount = 0;

      // inputs
      knapptryck();
      shoot_bullet(); // spaktryck();
      // If shoot_bullet() is called, spawn bullet at mainCharacter position
      bullet_init();

      display_image(bullet_x, bullet_y, 5, filled_square);

      // collisions
      border_collision();

			clear_display();
      
			display_image(x_mainCharacter, y_mainCharacter, 5, filled_square);
				
		}
			
	}

}



/*
NOTES:
INTCON_MVEC_MASK can be defined manually. 
According to the PIC32MX documentation, 
INTCON_MVEC_MASK is a bit mask for the MVEC bit in the INTCON register, 
which is bit 12. So, define INTCON_MVEC_MASK as follows:
#define INTCON_MVEC_MASK (1 << 12)

void labinit( void )
{
  // ... other code ...

  INTCONSET = INTCON_MVEC_MASK; // Enable global interrupts
}
*/
/*
--- Assignment 3 ---
• When the time-out event-flag is a "1", how does your code reset it to "0"?
- The time-out event-flag is reset to "0" by clearing the Timer 2 interrupt flag in the user_isr function. 
  This is done by writing a "1" to the IFS(0) register at bit position 8. 
  This is done by the line of code IFSCLR(0) = 0x100; in the user_isr function.


• What would happen if the time-out event-flag was not reset to "0" by your code? Why?
- If the time-out event-flag was not reset to "0" by your code, the interrupt service routine user_isr 
would be continuously called. This is because the interrupt controller would think that the interrupt 
condition (in this case, a timer timeout) is still present as long as the interrupt flag is set. 
This could lead to your program getting stuck in the interrupt service routine, preventing it from 
executing other tasks.


• From which part of the code is the function user_isr called? Why is it called from there?
- The function user_isr is an Interrupt Service Routine (ISR). It is not explicitly called from any part of the code you provided. Instead, it is invoked automatically by the hardware when a specific event, known as an interrupt, occurs.

In this case, the user_isr function is associated with the Timer 2 interrupt. 
When Timer 2 reaches the value specified in the PR2 register (31250 in this case), 
it generates an interrupt. The hardware then automatically invokes the user_isr function 
to handle this interrupt.


• Why are registers saved before the call to user_isr? Why are only some registers saved?
- The registers are saved before the call to user_isr to ensure that the state of the processor is preserved.
  Only some registers are saved because the user_isr function does not use all the registers.


• Which device-register (or registers), and which processor-register (or registers) must be
written to enable interrupts from the timer? Describe the functions of the relevant registers
- To enable interrupts from the timer, you must write to the IEC(0) register. 
  This register is used to enable specific interrupts. 
  In this case, you must write a "1" to bit position 8 of the IEC(0) register to enable the Timer 2 interrupt. 
  This is done by the line of code IECSET(0) = 0x100; in the labinit function.

  The INTCONSET register is used to enable global interrupts. 
  This register has a bit called MVEC (bit 12) that must be set to "1" to enable multi-vector mode. 
  This is done by the line of code INTCONSET = INTCON_MVEC_MASK; in the labinit function.

  The functions of the relevant registers are as follows:
  - IEC(0): This register is used to enable specific interrupts. 
    Writing a "1" to a specific bit position enables the corresponding interrupt.
  - INTCONSET: This register is used to enable global interrupts.
    Writing a "1" to a specific bit position enables the corresponding interrupt.
  - INTCON_MVEC_MASK: This is a bit mask for the MVEC bit in the INTCON register.
    It is used to enable multi-vector mode.



*/