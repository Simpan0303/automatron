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



// --------- BULLET LOGIC - BELOW WILL BE MOVED TO ANOTHER FILE ------------


int bullet_fire_delay = 0;  // Declare bullet_fire_delay as a global variable
int should_fire_bullet = 0;  // Declare should_fire_bullet as a global variable



Bullet bullets[MAX_BULLETS];

void init_bullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}

int x_speed_bullet = 0;
int y_speed_bullet = 0;
void spawn_bullet(int x, int y, int x_speed_bullet, int y_speed_bullet) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = x;
            bullets[i].y = y_mainCharacter;
            bullets[i].active = 1;
            bullets[i].x_speed = x_speed_bullet;
            bullets[i].y_speed = y_speed_bullet;
            break;
        }
    }
}

// Update the bullet's position
void update_bullet(Bullet* bullet) {
    // Previous position
    bullet->prev_x = bullet->x;
    bullet->prev_y = bullet->y;

    // New position
    bullet->x += bullet->x_speed * BULLET_SPEED;
    bullet->y += bullet->y_speed * BULLET_SPEED;

    // Deactivate the bullet if it goes off screen
    if (bullet->x < 0 || bullet->x > SCREEN_WIDTH - 2 || bullet->y < 0 || bullet->y > SCREEN_HEIGHT - 1) {
        bullet->active = 0;
    }
}

void border_collision_bullet() {
    for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].x <= 0 || bullets[i].x >= SCREEN_WIDTH - 2 || bullets[i].y <= 0 || bullets[i].y >= SCREEN_HEIGHT - 2) {
                bullets[i].active = 0;
            }
    }
}


void render_bullets() {
    int main_character_size = 5;
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            if (bullets[i].x != bullets[i].prev_x || bullets[i].y != bullets[i].prev_y) {
                clear_image(bullets[i].prev_x, bullets[i].prev_y, 2, bullet); // Clear the bullet
            }

            // if mainCharacter fires left, bullet should spawn to the left of mainCharacter by 5 pixels

            // if mainCharacter fires right, bullet should spawn to the right of mainCharacter by 5 pixels

            // if mainCharacter fires up, bullet should spawn above mainCharacter by 5 pixels

            // if mainCharacter fires down, bullet should spawn below mainCharacter by 5 pixels

            display_image(bullets[i].x, bullets[i].y, 2, bullet); // Draw the bullet
            // Update the previous position variables
            bullets[i].prev_x = bullets[i].x;
            bullets[i].prev_y = bullets[i].y;
        } else if ((bullets[i].active == 0) && (bullets[i].x != x_mainCharacter || bullets[i].y != y_mainCharacter)) {
            clear_image(bullets[i].x, bullets[i].y, 2, bullet); // Clear the bullet
        }
    }
}

// ---------  ABOVE WILL BE MOVED TO ANOTHER FILE ------------

// --------- ENEMY LOGIC BELOW ------------

// Enemy init
Enemy enemies[MAX_ENEMIES];
// MAX_ENEMIES might be replaced by int num_enemies; in the future

void init_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].active = 0;
    }
}

// spawn enemy at fixed position
// move enemy towards mainCharacter x and y position
// if enemy collides with mainCharacter, game over
// if enemy collides with bullet, enemy dies'

int x_speed_enemy = 0;
int y_speed_enemy = 0;
void spawn_enemy(int x, int y) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].active) {
            enemies[i].x = x;
            enemies[i].y = y;
            enemies[i].active = 1;
            enemies[i].x_speed = x_speed_enemy;
            enemies[i].y_speed = y_speed_enemy;
            break;
        }
    }
}


// update enemy
void update_enemy(Enemy* enemy) {
    // Previous position
    enemy->prev_x = enemy->x;
    enemy->prev_y = enemy->y;

    // New position
    enemy->x += enemy->x_speed;
    enemy->y += enemy->y_speed;

    // Deactivate the enemy if it goes off screen - pretty much redundant
    if (enemy->x < 0 || enemy->x > SCREEN_WIDTH - 2 || enemy->y < 0 || enemy->y > SCREEN_HEIGHT - 1) {
        enemy->active = 0;
    }

    // Deactivate the enemy if it collides with the mainCharacter
    if (enemy->x == x_mainCharacter && enemy->y == y_mainCharacter) {
        enemy->active = 0;
        // Game over
    }

    // Deactivate the enemy if it collides with a bullet
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active && bullets[i].x == enemy->x && bullets[i].y == enemy->y) {
            enemy->active = 0;
        }
    }

    // Update all active enemies movement
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // Move the enemy towards the mainCharacter
            if (x_mainCharacter > enemies[i].x) {
                enemies[i].x++;
            } else if (x_mainCharacter < enemies[i].x) {
                enemies[i].x--;
            }
            if (y_mainCharacter > enemies[i].y) {
                enemies[i].y++;
            } else if (y_mainCharacter < enemies[i].y) {
                enemies[i].y--;
            }
        }
    }
}

void render_enemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].active) {
            // Clear the image at the previous position if the character has moved
            if (enemies[i].x != enemies[i].prev_x || enemies[i].y != enemies[i].prev_y) {
                clear_image(enemies[i].prev_x, enemies[i].prev_y, 3, enemy); // Clear the enemy
            }
            display_image(enemies[i].x, enemies[i].y, 3, enemy); // Draw the enemy
            // Update the previous position variables
            enemies[i].prev_x = enemies[i].x;
            enemies[i].prev_y = enemies[i].y;
        } else if (enemies[i].active == 0) {
            clear_image(enemies[i].x, enemies[i].y, 3, enemy); // Clear the enemy
        }
    }
}



// --------- ENEMY LOGIC ABOVE ------------

// --------- MAIN CHARACTER LOGIC BELOW ------------
int mainCharacterState = STATE_DRAW;
// update mainCharacter
int prev_x_mainCharacter = 0;
int prev_y_mainCharacter = 0;
void update_mainCharacter() {
    knapptryck();
    if (mainCharacterState == STATE_DRAW) {
        if (x_mainCharacter != prev_x_mainCharacter || y_mainCharacter != prev_y_mainCharacter) {
            clear_image(prev_x_mainCharacter, prev_y_mainCharacter, 5, filled_square); // Clear the image at the previous position
        }
        

        // Draw the main character at the new position
        display_image(x_mainCharacter, y_mainCharacter, 5, filled_square);

        // Update the previous position variables
        prev_x_mainCharacter = x_mainCharacter;
        prev_y_mainCharacter = y_mainCharacter;
    } else if (mainCharacterState == STATE_CLEAR) {
        // Clear the image at the previous position
        clear_image(prev_x_mainCharacter, prev_y_mainCharacter, 5, filled_square);
    }
}


#define BULLET_FIRE_DELAY_MAX 5  // Decrease this value to make bullets spawn faster
void update_bullet(Bullet* bullet);
void game_loop(void) {
    // Game speed
    if (IFS(0) & 0x100) {
        IFSCLR(0) = 0x100;
        // timeoutcount++;
        timeoutcount = 0;

        if (timeoutcount >= 0) {
            timeoutcount = 0;
            //clear_display();
            border_collision_bullet();


            // inputs
            spaktryck(&x_speed_bullet, &y_speed_bullet);
            
            update_mainCharacter(); // Move the main character

            // Update bullets
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (bullets[i].active) {
                    update_bullet(&bullets[i]);
                }
            }

            // spawn bullet at mainCharacter position
            if (bullet_fire_delay >= BULLET_FIRE_DELAY_MAX && should_fire_bullet) {
                // Calculate the middle of the square
                int middle_x = x_mainCharacter;
                int middle_y = y_mainCharacter;

                spawn_bullet(middle_x, middle_y, x_speed_bullet, y_speed_bullet);
                bullet_fire_delay = 0;  // Reset the delay counter after firing a bullet
            }
            
            // display bullets
            render_bullets();
            

            // collisions
            border_collision();

            spawn_enemy(10, 10); // Draw the enemy
            // Update enemies
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemies[i].active) {
                    update_enemy(&enemies[i]); // Move the enemy
                }
            }
            render_enemies();

            bullet_fire_delay++;  // Increment the delay counter at the end of the game loop
        }
    }
}



