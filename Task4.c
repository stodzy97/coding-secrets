/**************************************************************************
*
*                           School of Engineering
*                  The Robert Gordon University, Aberdeen
*
***************************************************************************
*
*                  File name: Task4.c
*                  Author:    Christopher Stoddart
*                  Created:   21 november 2017
*                  Class:     EN3540 Computer Architecture
*                  Group:     
*
***************************************************************************
*
* Description: 
*    This program demonstrates the use of the analogue to digital converter
*	 on the 68HCS12. It reads the values from 4 channels - and then displays it
*	 on the LCD. The ATD Clock frequency is controlled by ATD1CTL45, 
*    By default its set at 0x6100,it uses 10-bit unsigned, left-hand
*	 justified data.
*    
**************************************************************************/

#include <mc9s12dp512.h>
#include <stdio.h>
#include "lcd.h"

int main(void)
{
    unsigned int value;
    unsigned long delay;
    char buf[24];	    // Memory buffer for output string
    
    lcd_init();         // Initialize the LCD panel
    lcd_clear();        // Clear LCD display  
	
    ATD1CTL23 = ADPU;   // Power-up ATD module
	ATD1CTL23 |= S4C;  // Convert four channels
    //ATD1CTL45 = MULT;  // Scan multiple channels
	// Set ATD clock frequency
	//ATD1CTL45 =0x6100 10-bit, left justified, unsigned data					
	//ATD1CTL45 =0xE110 8-bit, left justified, unsigned data
	//ATD1CTL45 =0x6150 10-bit, left justified, signed data
    //ATD1CTL45 =0xE150 8-bit, left justified, signed data
	//ATD1CTL45 =0x6190 10-bit, right justified, unsigned data
	  ATD1CTL45 =0xE190; //8-bit, right justified, unsigned data	
    
    while (1)
    {
        ATD1CTL45 |= 0x00; // Convert channel 0 first

        while ((ATD1STAT0 & SCF) == 0);// Wait for conversion to be completed

        // Read result
        value = ATD1DR0;                             // Read Channel 0
        sprintf(buf,"channel 0 = %x", value);        // Print channel Value
        lcd_putxy(0,0,buf);                          // Display on line 0
        
        value = ATD1DR1;                             // Read Channel 1
        sprintf(buf,"channel 1 = %x", value);        // Print channel Value
        lcd_putxy(1,0,buf);                          // Display on line 1
        
        value = ATD1DR2;                             // Read Channel 2
        sprintf(buf,"channel 2 = %x", value);        // Print Channnel Value
        lcd_putxy(2,0,buf);                          // Display on line 2
        
        value = ATD1DR3;                             // Read Channel 3
        sprintf(buf,"channel 3 = %x", value);        // Print channel Value
        lcd_putxy(3,0,buf);                          // Display on line 3
        
        
        for (delay = 0; delay < 500000; delay++);    // Introduce Time Delay
        if (delay = 490000)                          // Clears LCD Before Displaying Value Again
        {lcd_clear();
        }    
    }
    return 0;
}

