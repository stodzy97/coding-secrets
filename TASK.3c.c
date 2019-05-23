/*******************************************************************************
;*
;*                  School of Engineering
;*
;*                  The Robert Gordon University, Aberdeen
;*
;*******************************************************************************
;*
;*                   Filename:   Task3.c
;*                   Author:     Christopher Stoddart
;*                   Language:   68hc12 Assembly Language
;*                   Date:       21 Nov 2017
;*                   Class:      
;*
;******************************************************************************
*
* Description: 
*    This program demonstrates the use of the analogue to digital conversion
*	 on the 68HCS12. It takes a user generated 8 pixel array then displays it
*	 on the LCD. A subroutine has been purposely created called (lcd_make)
*    to handle this program.
*    
**************************************************************************/
#include <stdio.h>
#include "lcdv2.h"
#include <mc9s12dp512.h>

int main(void)
{  
    char symbol1[8] = {0x00,0x07,0x09,0x09,0x07,0x05,0x09,0x00};
	// Cyrillic (Russian)“ya” (the backwards letter R)
	char symbol1_address = 0x00;              
	// symbol Address
	
	char symbol2[8] = {0x00,0x11,0x13,0x15,0x19,0x11,0x00,0x00}; 
	//Cyrillic (Russian)“i” (the backwards letter N)
	char symbol2_address = 0x01;              
	// symbol Address
	
	char buf[24];	                          // Memory buffer for output string  
	
    lcd_init();                               // Initialize the LCD panel
	lcd_clear();                              // Clear LCD display

	lcd_move(0,1);                            // Move cursor right 1 space
	lcd_putchar(0xB6);                        // Display the character for pi
	lcd_putstr(" = 3.14159");                 // Display on LCD
 	lcd_move(1,5);                            // Move cursor right 4 spaces
    lcd_putstr("New symbols");                // Displaying the contents
        
	lcd_make(symbol1_address, symbol1);        
	//Make Cyrillic (Russian)“ya” (the backwards letter R)
	
	lcd_make(symbol2_address, symbol2);        
	//Make Cyrillic (Russian)“i” (the backwards letter N)
		          
	lcd_move(2,1);                            // Move curcor to line 3
	lcd_putchar(0x00);                        // Call 1st russian cyrillic symbol
		
	lcd_move(2,6);                            // Move cursor along 5 spaces
	lcd_putchar(0x01);                        // Call 2nd russian cyrillic symbol
		
    asm("swi");  			                  // Return control to DBug12 
	
 	return 0;
}