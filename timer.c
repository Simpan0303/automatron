/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */




#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <stdlib.h>   /* Declarations of malloc and free */
#include <stdio.h>  /* Declarations of printf */
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

    // Check if Timer 3 interrupt flag is set
    if (IFS(0) & 0x1000) {
        // Clear Timer 3 interrupt flag
        IFSCLR(0) = 0x1000;

        // Add your Timer 3 interrupt handling code here
        // For example, you might want to update a different part of the display
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

    // Timer 3 twice as fast as Timer 2, for bullets
    T3CON = 0x0; // Stop the timer and clear the control register
    TMR3 = 0x0; // Clear the timer register
    PR3 = PR2 / 2; // Load the period register with the value for 100ms
    T3CONSET = 0x70; // 1:256 Set prescaler (choose value based on calculation)
    IFSCLR(0) = 0x1000; // Clear Timer 3 interrupt flag
    IECSET(0) = 0x1000; // Enable Timer 3 interrupt
    T3CONSET = 0x8000; // Start Timer 3

    
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

/*
----------------------------------------
MATH FUNCTIONS
By Isak Erikson Winbäck and Simon Svanberg
----------------------------------------
*/

// own random number function - chipKit does not have rand() function
// returns a random number for x coordinate between 1 and 120
// TMR2 to seed the random number generator
int random_int(int start, int end) {
    return start + ((TMR2) % end); 
}

// own abs function
int abs(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}


/*      
----------------------------------------
STRING AND INT CONVERSION FUNCTIONS
By: Simon Svanberg
----------------------------------------
*/

void my_strcpy(char* dest, const char* src) {
    while (*src) {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

void my_strcat(char* destination, const char* source) {
    while (*destination) {
        destination++;
    }
    while (*source) {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
}

int my_atoi(const char *str) {
    int res = 0; // Initialize result

    // Find the start of the number in the string - skip all characters that are not digits
    int i = 0;
    while (str[i] != '\0' && (str[i] < '0' || str[i] > '9')) {
        i++;
    }

    // Iterate through all characters of input string and
    // update result
    for (; str[i] != '\0'; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            res = res*10 + str[i] - '0';
        }
    }

    // return result.
    return res;
}



/*
----------------------------------------
INTEGER TO STRING FUNCTION
By: ExternCowWindows - https://stackoverflow.com/questions/11819837/converting-integer-to-string-in-c-without-sprintf
Modified by: Simon Svanberg
----------------------------------------
*/

void integer_to_string(char * string, int number, int min_length) {
    if(number == 0) { 
        for (int i = 0; i < min_length - 1; i++) {
            string[i] = '0';
        }
        string[min_length - 1] = '0'; 
        string[min_length] = '\0';
        return; 
    };
    int divide = 0;
    int modResult;
    int  length = 0;
    int isNegative = 0;
    int  copyOfNumber;
    int offset = 0;
    copyOfNumber = number;
    if( number < 0 ) {
        isNegative = 1;
        number = 0 - number;
        length++;
    }
    while(copyOfNumber != 0) { 
        length++;
        copyOfNumber /= 10;
    }

    for(divide = 0; divide < length; divide++) {
        modResult = number % 10;
        number    = number / 10;
        string[length - (divide + 1)] = modResult + '0';
    }
    if(isNegative) { 
        string[0] = '-';
    }
    while (length < min_length) {
        for (int i = length; i > 0; i--) {
            string[i] = string[i - 1]; // Shift characters to the right
        }
        string[0] = '0'; // Insert leading zero
        length++;
    }
    string[length] = '\0';
}



/*
----------------------------------------
BULLET LOGIC
By: Simon Svanberg
Modified by: Isak Erikson Winbäck
----------------------------------------
*/


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
            if (x_speed_bullet < 0) {
                bullets[i].x = x - 2;
            } else if (x_speed_bullet > 0) {
                bullets[i].x = x + 5;
            } else {
                bullets[i].x = x;
            }

            if (y_speed_bullet < 0) {
                bullets[i].y = y;
            } else if (y_speed_bullet > 0) {
                bullets[i].y = y;
            } else {
                bullets[i].y = y;
            }

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

#define BULLET_FIRE_DELAY_MAX 5  // Decrease this value to make bullets spawn faster
void update_bullet(Bullet* bullet);


/*
----------------------------------------
ENEMY LOGIC
By: Simon Svanberg

----------------------------------------
*/

// Enemy init
Enemy enemies[MAX_ENEMIES];

int enemies_amount = 1;

void init_enemies() {
    for (int i = 0; i < enemies_amount; i++) {
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
    for (int i = 0; i < enemies_amount; i++) {
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

// spawn enemy at random position around screen left or right side (x == 1 or x == 127) or top or bottom side (y == 1 or y == 28
void spawn_enemy_random() {
    // x is randomly chosen to be 1 or 120
    int random_side = random_int(1, 2);
    if (random_side == 1) {
        // y is randomly chosen to be between 1 and 28
        spawn_enemy(1, random_int(1, 28));
    } else if (random_side == 2) {
        // y is randomly chosen to be between 1 and 28
        spawn_enemy(120, random_int(1, 28));
    }
}


int temp_score = 0;
int selected_movement_type = 1;

void enemy_movement_selector(Enemy* enemy, int movement_type) {
    if (movement_type == 4) {
        movement_type = random_int(1, 3);
    }
    
    if (movement_type == 1) {
        if (enemy->x < x_mainCharacter) {
        enemy->x_speed = 1;
        } else if (enemy->x > x_mainCharacter) {
            enemy->x_speed = -1;
        } else {
            enemy->x_speed = 0;
        }

        if (enemy->y < y_mainCharacter) {
            enemy->y_speed = 1;
        } else if (enemy->y > y_mainCharacter) {
            enemy->y_speed = -1;
        } else {
            enemy->y_speed = 0;
        }
        
    } else if (movement_type == 2) {
        double rate_of_change = (y_mainCharacter - enemy->y) / (x_mainCharacter - enemy->x);
        if (enemy->x < x_mainCharacter) {
            enemy->x_speed = 1;
            enemy->y_speed = rate_of_change;
        }
        if (enemy->x > x_mainCharacter) {
            enemy->x_speed = -1;
            enemy->y_speed = -rate_of_change;
        }
    } else if (movement_type == 3) {
        if (enemy->y < y_mainCharacter) {
        enemy->y_speed = 1;
        enemy->x_speed = 0;
        }
        if (enemy->y > y_mainCharacter) {
            enemy->y_speed = -1;
            enemy->x_speed = 0;
        }
        if (enemy->x < x_mainCharacter) {
            enemy->x_speed = 1;
            enemy->y_speed = 0;
        }
        if (enemy->x > x_mainCharacter) {
            enemy->x_speed = -1;
            enemy->y_speed = 0;
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

    // call enemy_movement_selector
    enemy_movement_selector(enemy, selected_movement_type);
}



void enemy_collision() {
    // Deactivate the enemy if it collides with a bullet - Frankenstein code man
    // Given that enemy is a 3x3 square
    // And that bullet is a 2x2 square
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            for (int j = 0; j < enemies_amount; j++) {
                if (enemies[j].active) {
                    for (int ex = 0; ex < 3; ex++) {
                        for (int ey = 0; ey < 3; ey++) {
                            for (int bx = 0; bx < 2; bx++) {
                                for (int by = 0; by < 2; by++) {
                                    if ((bullets[i].x + bx == enemies[j].x + ex) && (bullets[i].y + by == enemies[j].y + ey)) {
                                        enemies[j].active = 0;
                                        bullets[i].active = 0;
                                        temp_score++;
                                        ex = ey = bx = by = 3; // Break out of all inner loops
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Deactivate the enemy if it collides with the mainCharacter
    for (int i = 0; i < enemies_amount; i++) {
        if (enemies[i].active) {
            if (enemies[i].x == x_mainCharacter && enemies[i].y == y_mainCharacter) {
                enemies[i].active = 0;
                // Game over
            }
        }
    }
}

// Update all active enemies movement
void update_all_enemies() {
    // Update enemies
    for (int i = 0; i < enemies_amount; i++) {
        if (enemies[i].active) {
            update_enemy(&enemies[i]);
            enemy_collision();

            // Check if the enemy is still active after moving
            if (!enemies[i].active) {
                // Handle deactivation of enemy, e.g., remove from array, free memory, etc.
            }
        }
    }
}

void render_enemies() {
    damage_check();
    for (int i = 0; i < enemies_amount; i++) {
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

int spawn_rate_timeout = 0;


/*
----------------------------------------
DISPLAY NUMBER LOGIC - FOR IN-GAME SCORE AND LIVES
By: Simon Svanberg
----------------------------------------
*/

// Function to display the current score
// display score in top left corner of the screen
// base 10, support for 3 digits

// Array of pointers to digit images
const uint8_t (*digit_images[10])[7][7] = {
    &zero,
    &one,
    &two,
    &three,
    &four,
    &five,
    &six,
    &seven,
    &eight,
    &nine
};

void display_score() {
    // Display the score
    if (temp_score < 10) {
        display_image(1, 0, 7, *digit_images[0]); // Display the first digit
        display_image(9, 0, 7, *digit_images[0]); // Display the second digit
        display_image(17, 0, 7, *digit_images[temp_score]); // Display the third digit
    } else if (temp_score < 100) {
        display_image(1, 0, 7, *digit_images[0]); // Display the first digit
        display_image(9, 0, 7, *digit_images[temp_score / 10]); // Display the second digit
        display_image(17, 0, 7, *digit_images[temp_score % 10]); // Display the third digit
    } else {
        display_image(1, 0, 7, *digit_images[temp_score / 100]); // Display the first digit
        display_image(9, 0, 7, *digit_images[(temp_score / 10) % 10]); // Display the second digit
        display_image(17, 0, 7, *digit_images[temp_score % 10]); // Display the third digit
    }
}

int lives = 3;
int game_state = 0; // 0 = game running, 1 = game over, 2 = score screen

int damage_check() {
    // game over if mainCharacter collides with enemy
    // mainCharacter is a 5x5 square
    // enemies are 3x3 squares
    int collisionDetected = 0;
    for (int i = 0; i < enemies_amount; i++) {
        if (enemies[i].active) {
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 5; y++) {
                    if ((enemies[i].x <= x_mainCharacter + x && enemies[i].x + 2 >= x_mainCharacter + x) &&
                        (enemies[i].y <= y_mainCharacter + y && enemies[i].y + 2 >= y_mainCharacter + y)) {
                        collisionDetected = 1;
                        enemies[i].active = 0; // Make the enemy inactive after a collision
                        break;
                    }
                }
                if (collisionDetected) break;
            }
        }
        if (collisionDetected) {
            lives--;
            game_state = 1; // highscore screen
            if (lives < 0) {
                lives = 0;
                // Game over
            }
            break;
        }
    }
    return lives;
}

void display_life_amount() {
    // Display amount of lifes
    // top right corner of the screen
    display_image(113, 0, 7, heart); // Display heart symbol
    if (lives < 10) {
        display_image(120, 0, 7, *digit_images[lives]);
    }

}





/*
----------------------------------------
MAIN CHARACTER LOGIC
By: Simon Svanberg and Isak Erikson Winbäck
----------------------------------------
*/



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

/*
----------------------------------------
EEPROM HIGHSCORES LOGIC - CURRENTLY NOT WORKING ENTIRELY
By: Isak Erikson Winbäck
Modified by: Simon Svanberg
----------------------------------------
*/ 
int highscores[3];

int new_highscores[3] = {1, 5, 8};


/*
// update eeprom with values
// returns updated values as int array - format {n, n, n}
int* update_eeprom(int* values) {
    // Write values to the EEPROM
    metodTillSparningAvScore(values);
    // read values from eeprom
    int* fixed_score = metodTillkollektionAvScore();
    // return read values
    return fixed_score;

}

void display_highscores_string_eeprom(int* highscores) {
    // Update highscores and get the updated values
    int* updated_highscores = update_eeprom(highscores);
    // Now use updated_highscores for displaying highscores

    char string[16];
    int number = updated_highscores[0];       //1307152
    integer_to_string(string, number);

    // highscores[1] convert
    char string2[16];
    number = updated_highscores[1];       //1307152
    IntegertoString(string2, number);

    // highscores[2] convert
    char string3[16];
    number = updated_highscores[2];       //1307152
    IntegertoString(string3, number);

    display_string(0, "HIGHSCORES");
    display_string(1, string);
    display_string(2, string2);
    display_string(3, string3);
    display_update();
}




*/

// display_highscore takes the highscore array and displays it on the screen

void display_highscore( int score) {
    // Check if the score is negative
    // display 0 if score is negative
    if (score < 0) {
        score = 9;
    }
    // Display the score on the 7-segment display
    if (score < 10) {
        display_image(1, 0, 7, *digit_images[0]); // Display the first digit
        display_image(9, 0, 7, *digit_images[0]); // Display the second digit
        display_image(17, 0, 7, *digit_images[score]); // Display the third digit
    } else if (score < 100) {
        display_image(1, 0, 7, *digit_images[0]); // Display the first digit
        display_image(9, 0, 7, *digit_images[score / 10]); // Display the second digit
        display_image(17, 0, 7, *digit_images[score % 10]); // Display the third digit
    } else if (score < 1000){
        display_image(1, 0, 7, *digit_images[score / 100]); // Display the first digit
        display_image(9, 0, 7, *digit_images[(score / 10) % 10]); // Display the second digit
        display_image(17, 0, 7, *digit_images[score % 10]); // Display the third digit
    }
}


// convert int array of size 3 to string array of size 3
// display string array on screen
void display_highscores(int* highscores) {

    // set highscores to 1, 1, 1 in eeprom
    metodTillSparningAvScore(highscores);
    // read highscores from eeprom
    int* fixed_score = metodTillkollektionAvScore();

    // int* fixed_score = highscores;
    // highscores = {}


    // Display the highscores on the 7-segment display
    display_highscore(fixed_score[0]);
}


void save_temp_score(int score) {
    // Write 1, 1, 1 to the EEPROM
    int scores[3] = {1, 1, 1};
    metodTillSparningAvScore(scores);
    

    int* fixed_score = metodTillkollektionAvScore();

    // display fixed_score
    for (int i = 0; i < 3; i++) {
        display_highscore(fixed_score[i]);
    }
}


// clear eeprom scores (higscores arr of size 3)
void clear_eeprom_scores() {
    // Clear the high scores in the EEPROM
    int new_highscores[3] = {0, 0, 0};
    metodTillSparningAvScore(new_highscores);
}



/*
----------------------------------------
MENU LOGIC
By: Simon Svanberg
----------------------------------------
*/


// CONTINUE LOGIC
// display
void display_continue() {
    // display bottom right corner
    display_image(108, 21, 9, circled_1);
    display_image(120, 22, 7, right_arrow);
    // listen for btn1 press
    if (0x8 & getbtn1()) {
        clear_display();
        game_state++; 
        if (game_state > 4) {   // safety check to not get stuck in meny. Resumes game
            game_state = 0;
        }
        delay(5000);
        display_string(0, " ");
        display_string(1, " ");
        display_string(2, " ");
        display_string(3, " ");
        clear_display();
    }
}

void display_continue_initials() {
    // display bottom right corner
    display_image(108, 21, 9, circled_1);
    display_image(120, 22, 7, right_arrow);
    // listen for btn1 press
    if (0x8 & getbtn1()) {
        clear_display();
        game_state++; 
        if (game_state > 4) {   // safety check to not get stuck in meny. Resumes game
            game_state = 0;
        }

        display_string(0, " ");
        display_string(1, " ");
        display_string(2, " ");
        display_string(3, " ");
        clear_display();

        delay(5000);
    }
}

void display_continue_to_game() {
    // display bottom right corner
    display_image(108, 21, 9, circled_1);
    display_image(120, 22, 7, right_arrow);
    // listen for btn1 press
    if (0x8 & getbtn1()) {
        clear_display();
        game_state = 0; 
        display_string(0, " ");
        display_update();
        delay(5000);
    }
}


/*
----------------------------------------
INPUT INITIALS LOGIC
By: Simon Svanberg
----------------------------------------
*/

// input letters are based on switches. Switches represent a 4 bit number
// the 4 bit number is converted to a letter
// btn2 is used to confirm the letter and move to next letter


// switch to letter logic
char switch_to_letter(int switch_sum) {
    switch (switch_sum) {
        case 0: return 'A';
        case 1: return 'B';
        case 2: return 'C';
        case 3: return 'D';
        case 4: return 'E';
        case 5: return 'F';
        case 6: return 'G';
        case 7: return 'H';
        case 8: return 'I';
        case 9: return 'J';
        case 10: return 'K';
        case 11: return 'L';
        case 12: return 'M';
        case 13: return 'N';
        case 14: return 'O';
        case 15: return 'P';
        default: return 'A';
    }
}

char initials[3] = {'A', 'A', 'A'};

int i_letter = 0;

// switch input confirmation
char* input_initials() {
    char letter = switch_to_letter(switchinput());
    initials[i_letter] = letter;
    // use switch_to_letter to 

    // add 

    if (0x1 & getbtns()) { // listen for btn2 press
        i_letter++;
        delay(1000);
        if (i_letter > 2) {
            i_letter = 0;
        }
    }
    return initials;
}

void display_initials() {
    display_string(0, "ENTER INITIALS");
    display_update();
    display_string(1, initials);
    display_update();
    display_string(3, "btn2=nxt c");
    display_update();
    if (i_letter == 0) {
        display_string(2, "^");
        display_update();
    } else if (i_letter == 1) {
        display_string(2, " ^");
        display_update();
    } else if (i_letter == 2) {
        display_string(2, "  ^");
        display_update();
    }
}

/*
----------------------------------------
TEMPORARY HIGHSCORES LOGIC
By: Simon Svanberg
----------------------------------------
*/
int temp_highscores[3] = {0, 0, 0};
char temp_initials[3][4] = {"AAA", "AAA", "AAA"};

// position int corresponds to highscore_and_initials1, 2 or 3. higher position is better
// if temp_score is > highscore_and_initials1, 2 or 3, position is 1, 2 or 3
int position = 0;


char highscore_and_initials[3][12] = {{"1. AAA: 000"}, {"2. AAA: 000"}, {"3. AAA: 000"}};

// detect position of temp_score in highscores
// if temp_score is > highscore_and_initials1, 2 or 3, position is 1, 2 or 3
// if temp_score is < highscore_and_initials1, 2 or 3, position is 0
// as long as any element in highscores is < temp_score, position is at least 3
int check_position(int score) {
    if (score == 0) {
        return 0;
    }
    for (int i = 0; i < 3; i++) {
        if (score >= my_atoi(highscore_and_initials[i] + 8)) {
            return i + 1;
        }
    }
    return 0;
}

// sort highscores and initials
// index [i][3-5] is the initials, index [i][8-10] is the score

void sort_highscores_and_initials(int score, char* initials) {
    char score_str[4];
    integer_to_string(score_str, score, 3);

    char new_highscore[20];
    my_strcpy(new_highscore, "3. ");
    my_strcat(new_highscore, initials);
    my_strcat(new_highscore, ": ");
    my_strcat(new_highscore, score_str);

    // Find the correct position for the new score
    int position = 2; // Default position is the end of the array
    for (int i = 0; i < 3; i++) {
        char *score_part = highscore_and_initials[i];
        while (*score_part != ':' && *score_part != '\0') {
            score_part++; // Find the score part of the string
        }
        if (*score_part == ':') {
            score_part += 2; // Skip over the ': ' characters
        }
        if (score > my_atoi(score_part)) {
            position = i;
            break;
        }
    }

    // Shift down the scores below the new score
    for (int i = 2; i > position; i--) {
        my_strcpy(highscore_and_initials[i], highscore_and_initials[i-1]);
        highscore_and_initials[i][0] = '0' + (i + 1); // Update the rank
    }

    // Insert the new score
    new_highscore[0] = '0' + (position + 1); // Set the rank
    my_strcpy(highscore_and_initials[position], new_highscore);
}



// use placement and update_higscore_and_initials to determine which index of highscore_and_initials to update
// display highscore_and_initials on screen
void display_highscores_string() {
    display_string(0, "HIGHSCORES");
    display_update();
    display_string(1, highscore_and_initials[0]);
    display_update();
    display_string(2, highscore_and_initials[1]);
    display_update();
    display_string(3, highscore_and_initials[2]);
    display_update();
}



/*
----------------------------------------
GAME LOOP
By: Simon Svanberg
Modified by: Isak Erikson Winbäck
----------------------------------------
*/

int halfspeedtimeout1 = 0;
int halfspeedtimeout2 = 0;

int highscores_sorted = 0;

int spawned_inside_mainCharacter[MAX_BULLETS] = {0}; // Initialize to 0 for all bullets
void game_loop(void) {
    // Game speed
    if (IFS(0) & 0x100) {
        IFSCLR(0) = 0x100;
        // REDUNDANT, 0.5x speed
    }

    

    if (IFS(0) & 0x1000) {
        IFSCLR(0) = 0x1000;

        if (game_state == 1) {
            clear_display();
            game_state = 1;
            display_string(0, "GAME OVER!");
            display_update();

            delay(10000);
            quicksleep(1000000);
            clear_display();

            // wait 10 seconds
            game_state = 3;

            position = check_position(temp_score);

            
            //update_hs_in(temp_score, initials);
            if (position >= 1) { // performance > 0
                highscores_sorted = 0;
                display_string(0, "NEW HIGHSCORE!");
                display_update();

                delay(10000);
                quicksleep(1000000);

                display_string(0, "ENTER INITIALS");
                display_update();

                delay(10000);
                quicksleep(1000000);

                display_string(0, " ");
                display_update();
                game_state = 2;
            }
        }

        if (game_state == 2) {
            // INITIALS INPUT SCREEN
            input_initials();
            display_initials();

            display_continue_initials();

            
        }

        if (game_state == 3) {
            // SHOW HIGHSCORES SCREEN

            // highscores

            if (!highscores_sorted) {
                sort_highscores_and_initials(temp_score, initials);
                highscores_sorted = 1;
            }
            //update_highscore_and_initials(position, temp_score, initials);

            




            display_highscores_string();
            display_update();

            display_continue();
            // display highscores
        }

        if (game_state == 4) {
            // PLAY AGAIN SCREEN

            // play again
            display_string(0, "PLAY AGAIN?");
            display_update();

            display_continue_to_game();

            // reset game

            lives = 3;
            spawn_rate_timeout = 0;
            enemies_amount = 1;
            // set all enemies to inactive
            for (int i = 0; i < MAX_ENEMIES; i++) {
                enemies[i].active = 0;
            }
            // set all bullets to inactive
            for (int i = 0; i < MAX_BULLETS; i++) {
                bullets[i].active = 0;
            }
            x_mainCharacter = 64;
            y_mainCharacter = 14;


            temp_score = 0;

        }

        if (game_state == 0) {
            // resume playing

            // bullet logic
            // Update bullets
            // collisions
            border_collision_bullet();
            border_collision();

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
            

            //enemy logic
            // Update enemies
            spawn_rate_timeout++;
            
            // difficulty logic
            // randomize movement type
            // increase spawn rate as time goes on
            if (spawn_rate_timeout >= 1000) {
                spawn_rate_timeout = 0;
                enemies_amount++;
            }

            // randomly select movement type
            if (temp_score % 40 == 0) {
                selected_movement_type = 1;
            } else if (temp_score % 40 == 10) {
                selected_movement_type = 2;
            } else if (temp_score % 40 == 20) {
                selected_movement_type = 3;
            } else if (temp_score % 40 == 30) {
                selected_movement_type = 4;
            }



            // Update half as fast as bullets
            halfspeedtimeout1++;
            halfspeedtimeout2++;
            if (halfspeedtimeout2 >= 2) {
                halfspeedtimeout2 = 0;
                update_mainCharacter(); // Move the main character
                
            }

            // display bullets
            render_bullets();

            // spawn enemies
            spawn_enemy_random(); // Draw the enemy

            if (halfspeedtimeout1 >= 2) {
                halfspeedtimeout1 = 0;

                // inputs
                spaktryck(&x_speed_bullet, &y_speed_bullet);
                

                update_all_enemies(); // Move the enemy
                display_score();
                display_life_amount();
                bullet_fire_delay++;  // Increment the delay counter at the end of the game loop
            }
            
            

            

            render_enemies();
        }


    }

}





