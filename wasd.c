#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <stdlib.h>
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "functiondefinitions.h"  /* Declarations for functions */


int direction_gun=0;        //0=upp, 1=höger, 2=ned, 3=vänster, 4=upp-höger, 5=ned-höger, 6=ned-vänster, 7=upp-vänster

/*
-------------------------------------
SLC AND SDA - FOR I2C
By: Isak Erikson Winbäck
-------------------------------------
*/

//clock => 14 data=> 12
  void SLC(int i)
  {
	  if((TRISB>>14)%2==0)//om 0, 1 ställ beroende på i
	  {
		TRISB+=(0x4000*i);
	  }
	  else//om 1, 0 ställ beroende på i
	  {
		  i=(i-1)*(i-1);//(i-1)^2 = antingen 0^2 eller 1
		  TRISB-=(0x4000*i);//14=>0x4000	
	  }
  } // I2C clock 
  //19=>14 bit index
  void SDA(int i)
  {
	  if((TRISB>>12)%2==0)//om 0, 1 ställ beroende på i
	  {
		TRISB+=(0x1000*i);
	  }
	  else//om 1, 0 ställ beroende på i
	  {
		  i=(i-1)*(i-1);//(i-1)^2 = antingen 0^2 eller 1
		  TRISB-=(0x1000*i);//12=>0x1000	
	  }
  }

  //18=>12 bit index
  int SLCin()
  {
	  return ((PORTB>>14)%2);  //clock in
  }
  int SDAin()
  {
	  return ((PORTB>>12)%2);  //data in
  }

  void SLCut(int i)
  {
	  if(SLCin()==0)//om 0, 1 ställ beroende på i
	  {
		PORTB+=(0x4000*i);
	  }
	  else//om 1, 0 ställ beroende på i
	  {
		  i=(i-1)*(i-1);//(i-1)^2 = antingen 0^2 eller 1
		  PORTB-=(0x4000*i);//14=>0x4000	
	  }
  } // I2C clock 
  //19=>14 bit index
  void SDAut(int i)
  {
	  if(SDAin()==0)//om 0, 1 ställ beroende på i
	  {
		PORTB+=(0x1000*i);
	  }
	  else//om 1, 0 ställ beroende på i
	  {
		  i=(i-1)*(i-1);//(i-1)^2 = antingen 0^2 eller 1
		  PORTB-=(0x1000*i);//12=>0x1000	
	  }
  }
  

//8,4,2,1

/*
--display--
bredd: 128
höjd: 32
-----------
*/


/*
-------------------------------------
INPUTS - BUTTONS AND SWITCHES
By: Isak Erikson Winbäck
Modified by: Simon Svanberg
-------------------------------------
*/

// bit1 == bit2 is not valid in C. Use bit1 & bit2 instead.
int x_mainCharacter=64;     //fixa till logisk start sen
int y_mainCharacter=14;     //fixa till logisk start sen
void knapptryck()
{
    int buttons = getbtns();
    int btn1 = getbtn1();
    if (0x4 & buttons) {      // Button 4 (mapped to RD11)
        x_mainCharacter--;     
    }
    if (0x2 & buttons) {      // Button 3 (mapped to RD10)
        y_mainCharacter--;    
    }
    if (0x1 & buttons) {      // Button 2 (mapped to RD9)
        y_mainCharacter++;    
    }
    if (0x8 & btn1) {      // Button 1 (mapped to RF1)
        x_mainCharacter++;    
    }
}

// Test shooting with switches
// On switch activation, spawn a "bullet" in the direction of the switch
int bullet_x, bullet_y;
extern const uint8_t filled_square[][5];
// bullet_init
void bullet_init() {
    bullet_x = x_mainCharacter;
    bullet_y = y_mainCharacter;
}

int should_spawn_bullet() {
  return 1;
}


int bullet_direction = DIRECTION_RIGHT;  // Default direction
// spaktryck
void spaktryck(int* x_speed, int* y_speed) {
    int switches = getsw();
    should_fire_bullet = 0;
    *x_speed = 0;
    *y_speed = 0;
    if (0x8 & switches) {      // Switch 4 (mapped to RD3)
        //vänster  
        *x_speed = DIRECTION_LEFT;
        should_fire_bullet = 1;
        bullet_direction = DIRECTION_LEFT;
    }
    if (0x4 & switches) {      // Switch 3 (mapped to RD2)
        //upp
        *y_speed = DIRECTION_UP;
        should_fire_bullet = 1;
        bullet_direction = DIRECTION_UP;
    }
    if (0x2 & switches) {      // Switch 2 (mapped to RD1)
        //höger
        *y_speed = DIRECTION_DOWN;
        should_fire_bullet = 1;
        bullet_direction = DIRECTION_DOWN;
    }
    if (0x1 & switches) {      // Switch 1 (mapped to RD0)
        //ned
        *x_speed = DIRECTION_RIGHT;
        should_fire_bullet = 1; 
        bullet_direction = DIRECTION_RIGHT;
    }
}

// FOR INITIALS SELECTION

int switchinput() {
    int switches = getsw();
    int sw_input[4];
    int output = 0;

    if (0x8 & switches) {      // Switch 4 (mapped to RD3)
        sw_input[3] = 1;
    } else {
        sw_input[3] = 0;
    }
    if (0x4 & switches) {      // Switch 3 (mapped to RD2)
        sw_input[2] = 1;
    } else {
        sw_input[2] = 0;
    }
    if (0x2 & switches) {      // Switch 2 (mapped to RD1)
        sw_input[1] = 1;
    } else {
        sw_input[1] = 0;
    }
    if (0x1 & switches) {      // Switch 1 (mapped to RD0)
        sw_input[0] = 1;
    } else {
        sw_input[0] = 0;
    }

    // add together 4-bit value sw_input to output
    for (int i = 0; i < 4; i++) {
        output += sw_input[i] << (3 - i);
    }

    return output;
}


/*
-------------------------------------
EEPROM - I2C
By: Isak Erikson Winbäck
-------------------------------------
*/

//metod men för nu mest bara information som jag kanske kan ha nytta av, anta att score är en array med alla scores då denna kommer ge det till den

//metoder för i2c
void simpeldelayf()     //jag har inte riktigt bråttom när jag använder i2c i det här programmet (vi funderade till och med på en artificiell loading screen där i2c används)
{
  for(int simpeldelay=0;simpeldelay<=105;simpeldelay++)
  {
    volatile int i=0;     //bara för något delay
  }
}

void starti2c()         //start
{
  //1010000
  SDA(1);
  simpeldelayf();
  SLC(1);
  simpeldelayf();
  SDA(0);
  simpeldelayf();
  SLC(0);
}

int address(int lsb)         //1=>read, 0=>write          den här är överflödig men jag skapade den först
{
  //1010000
  SLC(0);
  simpeldelayf();
  SDA(1);
  simpeldelayf();
  SLC(1);
  simpeldelayf();
  SLC(0);
  simpeldelayf();
  //skickar 1
  SDA(0);
  simpeldelayf();
  SLC(1);
  simpeldelayf();
  SLC(0);
  simpeldelayf();
  //skickar 0
  SDA(1);
  simpeldelayf();
  SLC(1);
  simpeldelayf();
  SLC(0);
  simpeldelayf();
  //skickar 1
  for(int o=0;o<4;o++)
  {
    SDA(0);
    simpeldelayf();
    SLC(1);
    simpeldelayf();
    SLC(0);
    simpeldelayf();
  //skickar 0000
  }
  SDA(lsb);
  simpeldelayf();
  SLC(1);
  simpeldelayf();
  SLC(0);
  simpeldelayf();
  //skickar lsb, dvs read eller writeSCL=1;
  simpeldelayf();
  SLC(1);//clock upp för ack
  simpeldelayf();
  int b = SDAin();                                           //acknowlege
  SLC(0);
  simpeldelayf();
  return b;
}

void stopi2c()
{
  SDA(0);          
  simpeldelayf();
  SLC(1);
  simpeldelayf();     //kanske borde vara som den var
  SDA(1);
  simpeldelayf();
}

int skrivtilli2c(int tillbin)
{
  SLC(0);
  int bin[8];                                                 //det här hade kunnat göras med en loop men det är mer läsbart så här
  bin[7] = (tillbin & (128))>>7;
  bin[6] = (tillbin & (64))>>6;
  bin[5] = (tillbin & (32))>>5;
  bin[4] = (tillbin & (16))>>4;
  bin[3] = (tillbin & (8))>>3;
  bin[2] = (tillbin & (4))>>2;
  bin[1] = (tillbin & (2))>>1;
  bin[0] = tillbin & (1);

  for(int c=7;c>=0;c--)                                       //skriver de värden som jag tagit ut ovan
  {
    SDA(bin[c]);
    simpeldelayf();
    SLC(1);
    simpeldelayf();
    SLC(0);
    simpeldelayf();
  }
                                            //acknowlege
  SLC(1);
  simpeldelayf();
  int b = SDAin(); 
  simpeldelayf();
  SLC(0);
  simpeldelayf();
  return b;
}
                                                            //   128,64,32,16,8,4,2,1

int readi2c(int test)//test bör vara 0 men kanske 1 beroende på användning
{
  int bit[8];
  for(int c=7;c>=0;c--)                                       //skriver de värden som jag tagit ut ovan
  {
    simpeldelayf();
    SLC(1);
    while(SLCin()==0)                                           //väntar på 1
    {
      simpeldelayf();
      bit[c]=SDAin();
    }
    simpeldelayf();
    SLC(0);
    simpeldelayf();
  }
                                             //acknowlege
  if(test==1)
  {
    SDA(0);
  }
  else
  {
    SDA(1);
  }
  SLC(1);
  simpeldelayf();
  SLC(0);
  bit[0]+=((bit[1]<<1)+(bit[2]<<2)+(bit[3]<<3)+(bit[4]<<4)+(bit[5]<<5)+(bit[6]<<6)+(bit[7]<<7))
  ;//nop
  return bit[0];
}





int tomInt[3];

int* metodTillkollektionAvScore()//läser // Reads
{

  for(int i=0;i<3;i++)
  {
    //börja med i2c
    SLC(1);
    SDA(1);
    SLCut(0);
    SDAut(0);
  //data och clock till 1 båda in till 0
  starti2c();
  address(0);
  skrivtilli2c(i);
  starti2c();
  address(1);
  tomInt[i]=readi2c(1);
  stopi2c();
  }
  //The 7-bit I2C device address for the EEPROM is ‘1010000’(+1 bit).   (+1 bit)={lsb=1 => read lsb=0 => write}
  //i2c clock pin = 19, JP8 on Uno32
  //i2c data pin = 18, JP6 on Uno32

  // below 100KHz clock recomended
  

  //18/A4 J7-09 27 TCK/PMA11/AN12/RB12 selected by JP6
  //19/A5 J7-11 29 PMALH/PMA1/U2RTS/AN14/RB14 selected by JP8


  return tomInt;
}
void metodTillSparningAvScore(int score[])//skriver // Writes
{

  for(int i=0;i<3;i++)
  {
    //börja med i2c
    SLC(1);
    SDA(1);
    SLCut(0);
    SDAut(0);
  //data och clock till 1 båda in till 0
  starti2c();
  address(0);
  skrivtilli2c(i);
  skrivtilli2c(score[i]);
  stopi2c();
  }
  //The 7-bit I2C device address for the EEPROM is ‘1010000’(+1 bit).   (+1 bit)={lsb=1 => read lsb=0 => write}
  //i2c clock pin = 19, JP8 on Uno32
  //i2c data pin = 18, JP6 on Uno32

  // below 100KHz clock recomended
  

  //18/A4 J7-09 27 TCK/PMA11/AN12/RB12 selected by JP6
  //19/A5 J7-11 29 PMALH/PMA1/U2RTS/AN14/RB14 selected by JP8


}








