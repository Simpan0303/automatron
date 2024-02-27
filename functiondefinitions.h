/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

/* Declare display-related functions from mipslabfunc.c */
void display_image(int x, int y, int size, const uint8_t data[][size]); // size is length and/or width of the image
void clear_display(void);
void clear_image(int x, int y, int size, const uint8_t data[][size]);

void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from mipslabfunc.c */
char * itoaconv( int num );
void game_loop(void);
int nextprime( int inval );
void quicksleep(int cyc);
void tick( unsigned int * timep );
void labinit( void );

/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr );

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

/* Declare functions written by students.
   Note: Since we declare these functions here,
   students must define their functions with the exact types
   specified in the laboratory instructions. */
/* Written as part of asm lab: delay, time2string */
void delay(int);
void time2string( char *, int );


// Declare display-related functions from mipslabfunc.c
// and images as 2D arrays
extern const uint8_t filled_square[][5];
extern const uint8_t bullet[][2];
extern const uint8_t enemy[][3];

unsigned int getbtns(void);
unsigned int getbtn1(void);
int getsw(void);
void enable_interrupt(void);

void knapptryck(void);

#define STATE_DRAW 0
#define STATE_CLEAR 1


extern int x_mainCharacter;
extern int y_mainCharacter;
void spaktryck(int* x_speed, int* y_speed);
extern int lastButtonPressTime;
#define BUTTON_PRESS_DELAY 10  // Adjust this value based on your needs
extern int direction_gun;

void border_collision(void);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
// bullet logic
#define DIRECTION_UP -1
#define DIRECTION_DOWN 1
#define DIRECTION_RIGHT 1
#define DIRECTION_LEFT -1

extern int bullet_direction;

typedef struct {
   int x;
   int y;
   int prev_x;
   int prev_y;

   int active;
   int x_speed;  // Speed in the x direction
   int y_speed;  // Speed in the y direction
} Bullet;

void spawn_bullet(int x, int y, int x_speed, int y_speed);

extern int bullet_x, bullet_y;
void bullet_init(void);
void shoot_bullet(void);

int should_spawn_bullet(void);
extern int should_fire_bullet;
extern int x_speed;
extern int y_speed;

void update_bullets(Bullet* bullet);



#define BULLET_SPEED 2
#define BULLET_WIDTH 2
#define MAX_BULLETS 100 // arbitrary large number
#define BULLET_FIRE_DELAY_MAX 5


Bullet bullets[MAX_BULLETS];

extern int bullet_direction;


// Enemy logic
#define MAX_ENEMIES 1
typedef struct {
   int x;
   int y;
   int prev_x;
   int prev_y;

   int active;
   int x_speed;  // Speed in the x direction
   int y_speed;  // Speed in the y direction
} Enemy;

void spawn_enemy(int x, int y);
void update_enemy();
int[] metodTillkollektionAvScore();
void metodTillSparningAvScore(int[]);


