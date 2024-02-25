/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "functiondefinitions.h"  /* Declarations for functions */





int main(void) {


        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;
	SPI2BRG = 4;
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;




	
	display_init();
	
	display_update();
	
	// display_image(0, 0, 5, filled_square);
	// display_image(32, 18, 5, filled_square);
	// display_image(64, 0, 5, filled_square);
	// display_image(64, 14, 5, filled_square);
	// display_image(64, 27, 5, filled_square);
	
	labinit(); /* Do any lab-specific initialization */
	// knapptryck();
	while( 1 )
	{
		game_loop(); /* Do lab-specific things again and again */


		// on button press, move main character right on x axis
		// use knapptryck() from wasd.c

		
		
		// display_image(64, 32, 5, filled_square);


	}
	return 0;
}
