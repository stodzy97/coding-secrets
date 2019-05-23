/***************************************************************************
*
*                  School of Engineering
*
*                  The Robert Gordon University, Aberdeen
*
****************************************************************************
*
*                  File name: String_search.c
*                  Author:    Christopher Stoddart
*                  Created:   21 Nov 2017
*                  Modified:  21 Nov 2017
*                             21 Nov 2017
*                  Class:     EN3540 Computer Architecture
*                  Group:     
*
****************************************************************************
*
* Description: 
*
* EN3540 the program calls a subroutine which has three functions. A 16 bit
* address at the start of the block, the other 16 bit address at the end of 
* the block and finally the 8-bit search value. The subroutine returns a message
* dependant on if the character is found.
*
* The search character is set up in the program
***************************************************************************/

#include <stdio.h>

int main(void)
{
    int LocatePosition( unsigned ); 
	
	unsigned startaddress = 0x2000;      // Memory start Address
	unsigned endaddress = 0x3000;        // Memory End Address
	
	int  Position;                       // Find Position
	
    printf( "\n\rThe character to search for is %x ", find ); // Print value to find
       
    Position = LocatePosition( startaddress, endaddress, find ); //if the function is equal to
    if( Position < 0 )                           
	{
	    printf( "\n\rCharacter not located " ); // print if character not located
	}
	else           // print character memory location
	{
	    printf( "\n\rCharacter located at position %x ", Position );
	}
        
    asm( "swi" );
	
 	return 0;
}
