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
#define SCREEN_WIDTH 128

int mytime = 0x5957;

int prime = 1234567;

volatile int *trise;
volatile int *porte;

extern const uint8_t filled_square[][5];
extern const uint8_t bullet[][2];
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



#define MAX_BULLETS 10
#define BULLET_FIRE_DELAY_MAX 10  // Adjust this value to change the delay between bullets

int bullet_fire_delay = 0;  // Declare bullet_fire_delay as a global variable
int should_fire_bullet = 0;  // Declare should_fire_bullet as a global variable

typedef struct {
    int x, y;
    int active;
} Bullet;

Bullet bullets[MAX_BULLETS];

void init_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}

void spawn_bullet(int x, int y) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = x;
            bullets[i].y = y;
            bullets[i].active = 1;

            // Check if bullet is out of bounds
            if (bullets[i].x >= SCREEN_WIDTH) {
                bullets[i].active = 0;  // Deactivate bullet
            }

            break;
        }
    }
}

void update_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].x += 2 * bullet_direction;  // Update bullet's x position based on its direction
            // Check if bullet is out of bounds
            if (bullets[i].x >= SCREEN_WIDTH - 1 || bullets[i].x < 0) {
                bullets[i].active = 0;  // Deactivate bullet
            }
        }
    }
}

void game_loop(void) {
    // Game speed
    if (IFS(0) & 0x100) {
        IFSCLR(0) = 0x100;
        timeoutcount++;

        if (timeoutcount >= 1) {
            timeoutcount = 0;
            clear_display();

            // inputs
            knapptryck();
            spaktryck();

            // spawn bullet at mainCharacter position
            if (bullet_fire_delay >= BULLET_FIRE_DELAY_MAX && should_fire_bullet) {
                // Calculate the middle of the square
                int middle_x = x_mainCharacter + 5 / 2;
                int middle_y = y_mainCharacter + 5 / 2;

                spawn_bullet(middle_x, middle_y);
                bullet_fire_delay = 0;  // Reset the delay counter after firing a bullet
            }

            // update bullets
            update_bullets();

            // display bullets
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                  display_image(bullets[i].x, bullets[i].y, 2, bullet);
                }
            }

            // collisions
            border_collision();

            display_image(x_mainCharacter, y_mainCharacter, 5, filled_square);

            bullet_fire_delay++;  // Increment the delay counter at the end of the game loop
        }
    }
}



