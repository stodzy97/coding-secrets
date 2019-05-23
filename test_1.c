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
* Design and write the code to complete part of the overall display software
* including the user interface and part of the real-time controlling elements, 
* making use of the hardware and software elements covered in the first part of 
* the module plus additional features not yet covered.
*    
/****************************include librarys**********************************/
#include <stdio.h>
#include "lcdv2.h"
#include "DBug12New.h" 
#include <mc9s12dp512.h>
/******************************Set Values**************************************/
#define   KEYPRESS 0x10            /* 0b00010000 key pressed bit mask    */
#define   KEYVAL   0x0f            /* 0b00001111 key code value bit mask */
#define   Alarm_Value  0x3800
#define   StartAddress 0x3800
/******************************************************************************/

 void Alarm_count (void);                 /* menu 'A' function*/
 void Test_Alarms(void);                  /* menu 'B' function*/
 void Simulate_Alarms(void);              /* menu 'C' function*/
 void Timer_Init(void);                   /* intialize timer*/
 void TimerCh2_ISR(void);                 /* set interupt*/
 unsigned char Fstring;                   /* code from alarm*/
 unsigned char Prevval = 0;               /* define prevval to 0*/
 unsigned char keycode;                   /* code from keypad */
 unsigned char asciicode;                 /* ascii code to be displayed */
 unsigned int value;                      /* return value from subroutine*/
 unsigned long delay;                     /* Allows time delay to vary*/
 unsigned int DELVAL;                     /* set Delval to Variable*/
 unsigned string;                         /* variable string length*/
 unsigned locatestring(unsigned int,int); /* subroutine message pointer*/ 
 char Fstring;                            /* variable string value*/                             

int main(void)
{
    char buf[24];	                          // Memory buffer for output string 
                       
/***************************** Set up I/O ports *******************************/    
    DDRH = 0x00;                              // make Port H an input port
    PERH = 0xFF;                              // enable Port H 
/******************************************************************************/
    lcd_init();                               // Initialize the LCD panel
	lcd_clear();                              // Clear LCD display
	
	lcd_move(0,4);                            // move position across 4 spaces
	lcd_putstr(" Acme Alarms ");              // display on lcd
	lcd_move(1,9);                            // move position down 1,across 9
	lcd_putstr("By");                         // display on lcd
    lcd_move(2,0);                            // move position down 2
	lcd_putstr("Christopher Stoddart");	      // display on lcd
    lcd_move(3,5);                            // move position down 3,across 5
	lcd_putstr("ID 1705593");                 // display on lcd
	
    for (delay = 0; delay < 500000; delay++);    // Introduce Time Delay
    if (delay = 4900000)                         // clear "splash screen" 
    {
		lcd_clear();                             // clear lcd
    } 
		
	lcd_menu();                                  // main menu
	Alarm_count();                               // menu 'A'
	Test_Alarms();                               // menu 'B'
	Simulate_Alarms();                           // menu 'C'
	
	asm( "swi" );                                // return to D-Bug12
	
	return 0;                                    // return
    }
/*******************************************************************************
*   Name: 			lcd_menu         
*   Description:    main menu for routine that the sub menus and functions
*                   branch from.
*******************************************************************************/
    void lcd_menu(void)                          
   {
   
	lcd_move(0,0);                               // start position on lcd
    lcd_putstr("A - Count Alarms");              // display on lcd
	lcd_move(1,0);                               // move position down 1 
	lcd_putstr("B - Test Alarms");               // display on lcd
	lcd_move(2,0);                               // move position down 2
	lcd_putstr("C - Simulated Alarm");           // display on lcd
	lcd_move(3,0);                               // move position down 3
	lcd_putstr("Press a Key");	                 // display on lcd 
	 
while( 1 )                                       // response for keypress
	{ 
    while ( ( PTH & KEYPRESS ) == 0 );           // wait for key to be pressed
	keycode = PTH & KEYVAL;                      // read latched key code
	
	if      ((keycode) == 0xA)                   // if 'A' is pressed
	 {
	      Alarm_count();                         // go to alarm_count function
     }
	else if ((keycode) == 0xB)                   // if 'B' is pressed
	{
	     Test_Alarms();                          // go to test_alarm function
	}
	else if ((keycode) == 0xC)                   // if 'C' is pressed
	 {
	     Simulate_Alarms();                      // go to simulate_alarms funtion
	 }
	else                                         // if invalid key is pressed
	{
	lcd_clear();                                 // clear lcd 
	lcd_init();                                  // initalise lcd
	lcd_move(2,2);                               // move position down 2,across 2
	lcd_putstr("Invalid Keypress");              // display to lcd
	for (delay = 0; delay < 500000; delay++);    // set Time Delay for message
	lcd_menu();                                  // return to main menu
	}
    while ( ( PTH & KEYPRESS ) != 0 );           // wait for key to be released
   }
  }
/*******************************************************************************
*
*   Name: 			Alarm_count         
*   Description:    searches subroutine for number of alarms and returns a
*   value before displaying then returns to main menu.
*******************************************************************************/
	void Alarm_count(void)
	{
	char buf[24];                                  // Memory buffer for output string 
	unsigned char counter = 0;                     // set intial counter value to 0
	
	lcd_clear();                                   // clear lcd
	counter = AlarmCount(Alarm_Value);             // return value from subroutine
	sprintf(buf, "%d Alarm Types", counter);       // lcd memory buffer
	lcd_move(1,3);                                 // move position down 1,across 3
	lcd_putstr( buf );                             // display to lcd
	lcd_move(3,2);                                 // move position down 3,across 2
	lcd_putstr( "Any key for menu" );              // display to lcd
	
	for (delay = 0; delay < 100000; delay++);      // Introduce Time Delay
	
	while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
	while ( ( PTH & KEYPRESS ) != 0 );             // wait for key to be released
	lcd_clear();                                   // clear lcd 
	lcd_menu();                                    // go to lcd main menu
	return;
	}
/***************************************************************************
*
*   Name: 			Test_Alarms         
*   Description:    searches subroutine for an entered alarms and returns an
*   address, alarm type, alarm duration and string length which is displayed on the 
*   lcd.
***************************************************************************/
	
	void Test_Alarms(void)
    {
	int combine;                                   // define numeric variables holding whole numbers
    unsigned FirstAddr;                            // set variable 16 bits wide
    unsigned char Code;                            // set variable character code
    unsigned char Length;                          // set variable character legnth
    unsigned char FirstMSG;                        // set variable character message
    char buf[24];                                  // Memory buffer for output string 
	
    lcd_clear();                                   // clear lcd display
    for (delay = 0; delay < 50000; delay++);       // Introduce Time Delay
	sprintf(buf, "Enter Alarm:");
    lcd_putxy(0,0,buf);                            // display to lcd
         
    
//******************************For first Digit*******************************//
    while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
    keycode=PTH & KEYVAL;                          // read latched key code
    sprintf(buf,"%x", keycode);                    
    lcd_putxy(0,12,buf);                           // and dipslay it
    combine = keycode*100;                         // add top bits of number 
    while ( ( PTH & KEYPRESS ) != 0 );             // wait for key to be released

//******************************For Second Digit******************************//
    while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
    keycode = PTH & KEYVAL;                        // read latched key code
    sprintf(buf, "%x", keycode);                   
    lcd_putxy(0,13,buf);                           // and dipslay it
    combine += keycode*10;                         // add top bits of number 
    while ( ( PTH & KEYPRESS ) != 0 );             // wait for key to be released

//******************************For Third Digit*******************************//
    while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
    keycode = PTH & KEYVAL;                        // read latched key code
    sprintf(buf, "%x", keycode);
    lcd_putxy(0,14,buf);                           // and display it
    combine += keycode;                            // add in lower 4 bits of number
    while ( ( PTH & KEYPRESS ) != 0 );             // wait for key to be released

//*****************************Display values*********************************//       
 // This snippet of code is bracketed out as i was unable to make the error string 
 // alarm work when the string length was exceeded. 
 // FirstMSG= MSGlen(StartAddress,combine);	
 // if (FirstMSG==255)
 // {
 //lcd_clear();
 //lcd_move(1,2);
 //lcd_putstr("too large");
 //}
    FirstAddr = AlarmAddr(StartAddress,combine);  // find address of alarm
    if( FirstAddr==0)                             // if alarm isnt found
   {
    lcd_clear();                                  // lcd clear
    lcd_move(1,2);                                // move position down 1,across 2
    lcd_putstr("Alarm Not Found");                // display on lcd
    for (delay = 0; delay < 500000; delay++);     // Introduce Time Delay
    Test_Alarms();                                // go to test alarm menu
   }
    else                                          // if alarm is found 
   {
    lcd_move(1,0);                                // move position down 1
    lcd_putstr("Addr  Type  Len  Str\r");         // display alarm headers

    sprintf(buf, "%x", FirstAddr);                    
    lcd_putxy(2,0,buf);                           // and display it

    Code= AlarmCode(StartAddress,combine);        // find alarm code
    sprintf(buf,"%c", Code);                          
    lcd_putxy(2,7,buf);                           // and display it

    Length= AlarmLen(StartAddress,combine);       // find length of alarm
    sprintf(buf,"%d", Length);                        
    lcd_putxy(2,13,buf);                          // and display it

    FirstMSG= MSGlen(StartAddress,combine);       // returns length of message
    sprintf(buf,"%d", FirstMSG);
    lcd_putxy(2,17,buf);

    lcd_move(3,2);                                 // move position down 3,across 2
    lcd_putstr("Any key for Menu");                // display to lcd

    for (delay = 0; delay < 100000; delay++);      // time delay before key is pressed
	
	while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
	while ( ( PTH & KEYPRESS ) != 0 );             // wait for key to be released
	lcd_clear();                                   // clear lcd
	lcd_menu();                                    // go to lcd main menu

    }
   }
/***************************************************************************
*
*   Name: 			Simulated_Alarms          
*   Description:    Searches subroutine for an a alarm and returns a message and code
*   value, before playing an alarm according to the database using interupts.          
*   
***************************************************************************/
	
    void Simulate_Alarms(void)
   {
    
	int combine;                                   // define numeric variables holding whole numbers
    unsigned FirstAddr;                            // set variable 16 bits wide
    unsigned char Code;                            // set variable character code
    unsigned char Length;                          // set variable character legnth
    unsigned char FirstMSG;                        // set variable character message
    char buf[24];                                  // Memory buffer for output string
	
	
    lcd_clear();                                   // clear lcd display
    for (delay = 0; delay < 50000; delay++);       // Introduce Time Delay
	sprintf(buf, "Enter Alarm:");                  
    lcd_putxy(0,0,buf);                            // display to lcd
    
//******************************For first Digit*******************************//
    while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
    keycode=PTH & KEYVAL;                          // read latched key code
    sprintf(buf,"%x", keycode);                    
    lcd_putxy(0,12,buf);                           // and display it
    combine= keycode*100;                          // add top bits of number
    while ( ( PTH & KEYPRESS ) != 0 );             // wait for key to be released

//******************************For Second Digit******************************//
    while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
    keycode = PTH & KEYVAL;                        // read latched key code
    sprintf(buf, "%x", keycode);
    lcd_putxy(0,13,buf);                           // display to lcd
    combine+= keycode*10;                          // add top bits of number 
    while ( ( PTH & KEYPRESS ) != 0 );             // wait for key to be released

//******************************For Third Digit*******************************//
    while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
    keycode = PTH & KEYVAL;                        // read latched key code
    sprintf(buf, "%x", keycode);                   
    lcd_putxy(0,14,buf);                           // display to lcd
    combine+= keycode;                             // add in lower 4 bits of number
    while ( ( PTH & KEYPRESS ) != 0 );
	
//****************************************************************************//

	string = locatestring(StartAddress,combine);   // string pointer value from subroutine
	FirstAddr = AlarmAddr(StartAddress,combine);   // find address of alarm
	if( FirstAddr==0)                              // if alarm isnt found
   {
    lcd_clear();                                   // clear lcd
    lcd_move(1,2);                                 // move position down 1,across 2
    lcd_putstr("Alarm Not Found");                 // display error message to lcd
    for (delay = 0; delay < 500000; delay++);      // wait fot time delay
    Simulate_Alarms();                             // display simulate alarms menu
   }
    else
   {
    sprintf(buf, "Enter Alarm:");
    lcd_putxy(0,0,buf);                            // and display on lcd
    lcd_move(1,0);                                 // move position down 1
    lcd_putstr("Alarm type");                      // display to lcd
    Code= AlarmCode(StartAddress,combine);         // return alarm type
    sprintf(buf, "%c", Code);
    lcd_putxy(1,11,buf);                           // and display it

    lcd_move (2,0);                                // move position down 2
    lcd_putstr((char*)string);                     // display alarm message string
    lcd_move(3,0);                                 // move position down 3
    lcd_putstr("press key to stop:");              // display to lcd
   }

    if (Code == 'a')                               // when code equals a
   {
	DELVAL = 750;                                  // set low frequency tone
	VectTimerCh2 = (unsigned)TimerCh2_ISR;         // Set up interrupt vector
    Timer_Init();                                  // Initialise the timer 
	asm("cli");                                    // sets mask interupt 
	while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
    keycode = PTH & KEYVAL;                        // read latched key code
	
	if ((keycode)!= 0)                             // if key is pressed
	 {
	 TSCR1 = 0x00;                                 // interupt alarm
	 for (delay = 0; delay < 100000; delay++);     // introduce time delay
	 lcd_clear();                                  // clear lcd
	 lcd_menu();                                   // return to main menu
	 }
	}
    else if (Code=='b')                            // when code equals b
    {
	DELVAL = 400;                                  // set mid frequency tone
	VectTimerCh2 = (unsigned)TimerCh2_ISR;         // Set up interrupt vector
    Timer_Init();                                  // Initialise the timer
	asm("cli");                                    // sets mask interupt 
	while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
    keycode = PTH & KEYVAL;                        // read latched key code
	
	if ((keycode)!= 0)                             // if key is pressed
	{
	 TSCR1 = 0x00;                                 // interupt alarm
	 for (delay = 0; delay < 100000; delay++);     // introduce time delay
	 lcd_clear();                                  // clear lcd
	 lcd_menu();                                   // return to main menu
	 }
	}
    else if (Code=='c')                            // when code equals c
    {
	DELVAL = 100;                                  // set high frequency tone
	VectTimerCh2 = (unsigned)TimerCh2_ISR;         // Set up interrupt vector
    Timer_Init();                                  // Initialise the timer
	asm("cli");                                    // sets mask interupt 
	while ( ( PTH & KEYPRESS ) == 0 );             // wait for key to be pressed
    keycode = PTH & KEYVAL;                        // read latched key code
	
	if ((keycode)!= 0)                             // if key is pressed
	{
	 TSCR1 = 0x00;                                 // interupt alarm
	 for (delay = 0; delay < 100000; delay++);     // introduce time delay
	 lcd_clear();                                  // clear lcd menu
	 lcd_menu();                                   // return to main menu
	 }
    }
    else if (Code=='f')                             // when code equals f
    {  
	DELVAL = 700;                                   // start with low frequency tone
	VectTimerCh2 = (unsigned)TimerCh2_ISR;          // Set up interrupt vector
    Timer_Init();                                   // Initialise the timer 
	asm("cli");                                     // sets mask interupt
	for (delay = 0; delay < 150000; delay++);       // operate for 1.5s time delay
	TSCR1 = 0x00;                                   // interupt alarm
	for (delay = 0; delay < 50000; delay++);        // off for 0.5s 
	
	DELVAL = 600;                                   // raise frequency slightly
	VectTimerCh2 = (unsigned)TimerCh2_ISR;          // Set up interrupt vector
    Timer_Init();                                   // Initialise the timer 
	asm("cli");                                     // sets mask interupt 
	for (delay = 0; delay < 150000; delay++);       // operate for 1.5s time delay
	TSCR1 = 0x00;                                   // interupt alarm
	for (delay = 0; delay < 50000; delay++);        // off for 0.5s 
	
	DELVAL = 500;                                   // raise frequency slightly
	VectTimerCh2 = (unsigned)TimerCh2_ISR;          // Set up interrupt vector
    Timer_Init();                                   // Initialise the timer
	asm("cli");                                     // sets mask interupt 
	for (delay = 0; delay < 150000; delay++);       // operate for 1.5s time delay
	TSCR1 = 0x00;                                   // interupt alarm
	for (delay = 0; delay < 50000; delay++);        // off for 0.5s 
	
	DELVAL = 400;                                   // raise frequency slightly
	VectTimerCh2 = (unsigned)TimerCh2_ISR;          // Set up interrupt vector
    Timer_Init();                                   // Initialise the timer
	asm("cli");                                     // sets mask interupt 
	for (delay = 0; delay < 150000; delay++);       // operate for 1.5s time delay
	TSCR1 = 0x00;                                   // interupt alarm
	for (delay = 0; delay < 50000; delay++);        // off for 0.5s 
	
	DELVAL = 300;                                   // raise frequency slightly
	VectTimerCh2 = (unsigned)TimerCh2_ISR;          // Set up interrupt vector
    Timer_Init();                                   // Initialise the timer 
	asm("cli");                                     // sets mask interupt
	for (delay = 0; delay < 150000; delay++);       // operate for 1.5s time delay
	TSCR1 = 0x00;                                   // interupt alarm
	for (delay = 0; delay < 50000; delay++);        // off for 0.5s 
	
	DELVAL = 200;                                   // raise frequency slightly
	VectTimerCh2 = (unsigned)TimerCh2_ISR;          // Set up interrupt vector
    Timer_Init();                                   // Initialise the timer 
	asm("cli");                                     // sets mask interupt
	for (delay = 0; delay < 150000; delay++);       // operate for 1.5s time delay
	TSCR1 = 0x00;                                   // interupt alarm
	for (delay = 0; delay < 50000; delay++);        // off for 0.5s
	
	DELVAL = 100;
	VectTimerCh2 = (unsigned)TimerCh2_ISR;          // Set up interrupt vector
    Timer_Init();                                   // Initialise the timer
	asm("cli");                                     // sets mask interupt
	for (delay = 0; delay < 150000; delay++);       // operate for 1.5s time delay
	TSCR1 = 0x00;                                   // interupt alarm
	for (delay = 0; delay < 50000; delay++);        // off for 0.5s
	
	lcd_clear();                                    // clear lcd display
	lcd_menu();                                     // return to main menu
	}
     
    else if (Code=='d')                             // when code equals d
    {
	 while ( ( PTH & KEYPRESS ) == 0 )              // wait for key to be pressed
	  {
	  DELVAL = 400;                                 // set mid frequency tone
	  VectTimerCh2 = (unsigned)TimerCh2_ISR;        // Set up interrupt vector
      Timer_Init();                                 // Initialise the timer 
	  asm("cli");                                   // sets mask interupt
	
	  for (delay = 0; delay < 50000; delay++);      // operate for 0.5s time delay
	  TSCR1 = 0x00;                                 // interupt alarm
	
	  for (delay = 0; delay < 50000; delay++);      // off for 0.5s
	  TSCR1 = 0x04;                                 // interupt alarm
	  }
	while ( ( PTH & KEYPRESS ) != 0 )               // wait for key to be pressed
	{
     TSCR1= 0x00;                                   // interupt alarm
	 for (delay = 0; delay < 100000; delay++);      // time delay to menu
	 lcd_clear();                                   // clear lcd display
	 lcd_menu();                                    // return to main menu
	}
	while ( ( PTH & KEYPRESS ) == 0 );              // wait for key to be pressed
	keycode = PTH & KEYVAL;                         // read latched key code
	 
	for (delay = 0; delay < 100000; delay++);       // time delay to menu 
	while ( ( PTH & KEYPRESS ) == 0 );              // wait for key to be pressed
	keycode = PTH & KEYVAL;                         // read latched key code
	while ( ( PTH & KEYPRESS ) != 0 )               // wait for key to be pressed
	lcd_clear();                                    // clear lcd display
	lcd_menu();                                     // return to main menu
   }
	  
   else if (Code=='e')                              // when code equals e
   {
	while ( ( PTH & KEYPRESS ) == 0 )               // wait for key to be pressed
	{
	DELVAL = 400;                                   // set mid frequency tone
	VectTimerCh2 = (unsigned)TimerCh2_ISR;          // Set up interrupt vector
    Timer_Init();                                   // Initialise the timer
	asm("cli");                                     // sets mask interupt
	
	for (delay = 0; delay < 150000; delay++);       // operate for 1.5s time delay
	TSCR1 = 0x00;                                   // interupt alarm
	
	for (delay = 0; delay < 50000; delay++);        // off for 0.5s
	TSCR1 = 0x04;                                   // interupt alarm
	}
	while ( ( PTH & KEYPRESS ) != 0 )               // wait for key to be pressed
	{
    TSCR1= 0x00;                                    // interupt alarm
	for (delay = 0; delay < 200000; delay++);       // 2s delay to menu
	lcd_clear();                                    // clear lcd dispaly
	lcd_menu();                                     // go to main menu
	
	}
	while ( ( PTH & KEYPRESS ) == 0 );              // wait for key to be pressed
	 keycode = PTH & KEYVAL;                        // read latched key code
	 }
	for (delay = 0; delay < 100000; delay++);       // 1s delay
	while ( ( PTH & KEYPRESS ) == 0 );              // wait for key to be pressed
	keycode = PTH & KEYVAL;                         // read latched key code
	while ( ( PTH & KEYPRESS ) != 0 )               // wait for key to be pressed
	lcd_clear();                                    // clear lcd display
	lcd_menu();                                     // go to main menu
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
/*****************************************************************************/
#pragma interrupt_handler TimerCh2_ISR

void TimerCh2_ISR( void )
{
    TC2 += DELVAL;   /* Calculate the next target count */
	TFLG1 |= 0x04;   /* Clear the OC2 flag */
}
