/**************************************************************************
*
*                           School of Engineering
*                  The Robert Gordon University, Aberdeen
*
***************************************************************************
*
*                  File name: assessment1.c
*                  Author:    christopher Stoddart
*                  Created:	  11 Nov 2017
*                  Modified:  GRAD 11 Oct 2017
*                  Class:     
*                  Group:     
*
***************************************************************************
*
* Description: 
*    This program generates a 440Hz square wave on channel 2 of the timer
*    port using interrupts
*    
//****************************include librarys*********************************/
#include <stdio.h>
#include <mc9s12dp512.h>
#include "DBug12New.h" 
//**********************set frequency of wave form*****************************/
#define DELVAL  852.48 // Count for 1136 ms (ECLOCK = 12 MHz, Prescaler = 32)
/******************************************************************************/

unsigned char Prevval = 0;
void Timer_Init(void);
void TimerCh2_ISR(void);

//*****************************Main Loop***************************************/

int main(void)
{                                  
    VectTimerCh2 = (unsigned)TimerCh2_ISR;   /* Set up interrupt vector*/
    Timer_Init();                            /* Initialise the timer */
	asm("cli");                              /* sets mask interupt*/
	
/****************************Set up I/O Ports*********************************/
	DDRH = 0x00;                    /* make Port H an input port  */
    PERH = 0xFF;                    /* enable Port H */
/*****************************************************************************/

    while (1)                                /*while loop*/
	{
	if                                       /*print loop*/
	( ((PTH & 0x80) == 0)                    /*when bit 7 is 0*/
	 && 
	((Prevval & 0x80) == 0x80))              /*triggers on negative edge only*/ 
	{  
	puts("Christopher Stoddart\r");          /* Display on screen*/
	}
		Prevval = PTH;                      /*remembers switch value*/
	}
    return 0;
}

/**************************************************************************
 *
 * Name:        Timer_Init               
 * Parameters:  none
 * Returns:     nothing
 * Globals:     none
 * Description: Initialises the timer using channel 2 in toggle mode. 
 * 				The prescaler is set to 32.
 *             
 *************************************************************************/
 
void Timer_Init(void)
{
    TIE = 0x04;     // Enable interrupt for channel 2 - disable all others
    TSCR2 = 0x05;   // Set prescaler to 32
    TCTL2 = 0x10;   // Initialise OC2 to toggle on successful compare
    TIOS = 0x04;    // Make channel 2 function as an output compare
    TC3 = DELVAL;   // Set initial compare value
    TSCR1 = 0x80;   // Enable timer
}

/**************************************************************************
 *
 * Name:        TimerCh2_ISR               
 * Parameters:  none
 * Returns:     nothing
 * Globals:     none
 * Description: Interrupt service routine to generate a 440Hz square wave on 
 *     			channel 2.        
 *************************************************************************/
 
#pragma interrupt_handler TimerCh2_ISR

void TimerCh2_ISR( void )
{
    TC2 += DELVAL;   /* Calculate the next target count */
	TFLG1 |= 0x04;   /* Clear the OC2 flag */
}

