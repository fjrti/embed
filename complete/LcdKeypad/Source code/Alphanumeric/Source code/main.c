/**
        LICENSE

  	Name: Keypad and LCD.c
 	Author: Aurangzaib Siddiqui
  	Creation Date: 2012-June-18
  	Tabsize: 4
 	License: GNU Public License (GPL) v3
      Output size : 1.77 ( HEX file ) KB
      Compiler : avr-gcc 4.6

		ALGORITHM

Set ddr of LCD_data , LCD_cmd , keypad and send "Press any key" msg to LCD
Make keypad = 0xf0 i.e col as high and row as low. Check if keypad value is not 0xf0 means any of key is pressed i.e any col is low.
Now make row=0 and see for col==0 one by one and if ( bit_is_clear( PinD , ::: ) then send data to LCD( ::: means any pin of PORTD )

**/

/******		COMPILER DIRECTIVES		******/


#include <avr/io.h>
#include <util/delay.h>

#define Keypad_Out PORTB // to send data
#define Keypad_In PINB	 // to get data

#define Row1 PB0
#define Row2 PB1
#define Row3 PB2
#define Row4 PB3

#define Col1 PB4
#define Col2 PB5
#define Col3 PB6
#define Col4 PB7

#define RS 6 // register select of lcd
#define E 7 //  enable of lcd

#define Data PORTC // lcd data
#define Command PORTD // lcd command

#define LcdDataDirecton DDRC
#define LcdCmdDirection DDRD
#define KeypadDirection DDRB

// declaration :

void Initialization (    ) ;
void Checking (  ) ;
void Commandfunction    ( unsigned char ) ; // lcd command routine
void Datafunction       ( unsigned char ) ; // lcd command routine
void Printfunction      ( char*)  ;         // lcd print routine

/******		MAIN FUNCTION		******/

int main (  ) {

    Initialization( ) ;                                                     // Initialize LCD
    Printfunction( "Press key" ) ;
    _delay_ms ( 10 ) ;

    Commandfunction( 0x01 ) ;
    _delay_ms ( 2 ) ;

    for ( ;; ) {

        Keypad_Out = 0xF0 ; // row =0, col =1
        _delay_ms ( 20 ) ;
        unsigned char buffer = Keypad_In ;
        if ( buffer != 0xF0 )   { // i.e any column key is pressed

// now repeated same thing again to ensure that key was pressed

            _delay_ms( 20 )    ;
            buffer = Keypad_In ;

            if ( buffer != 0xF0 )   Checking( ) ;  // some key is pressed
        }
    }
}

void Initialization ( ) {

//	setting direction registers :

    KeypadDirection = 0X0F ;                                                   // row as output , col as input ( 0b00001111 )
    LcdDataDirecton = 0XFF ;                                                   // data port - LCD
    LcdCmdDirection = (1<<RS) | (1<<E) ;
    Keypad_Out = 0xF0 ;

    Commandfunction ( 0x33 ) ; // for 4 bit lcd
    Commandfunction ( 0x32 ) ;  // for 4 bit lcd
    Commandfunction ( 0x28 ) ;  // for 4 bit lcd
    Commandfunction ( 0x0E ) ;  // cursor on
    Commandfunction ( 0x06 ) ;
    Commandfunction ( 0x01 ) ;  // clear

}

// Keypad Operation  Routine :

void Checking(  ) {

// Checking row 1 :

    Keypad_Out = 0xFE ; // 0xFE                                          // make row1=0 and see for any col=0
    _delay_ms ( 1 ) ;

    unsigned char buffer = Keypad_In ;

    if ( ((Keypad_Out&(1<<Row1))==0) ) {

        if 	   ( ((buffer&(1<<Col1))==0) )      Printfunction ( "1") ;
        else if ( ( buffer & (1<<Col2) ) == 0 ) Printfunction ( "2") ;
        else if ( ( buffer & (1<<Col3) ) == 0 ) Printfunction ( "3") ;
        else if ( ( buffer & (1<<Col4) ) == 0 ) Printfunction ( "A") ;

    }

// Checking row 2 :

    Keypad_Out = 0xFD ;     // 0xFD                                       // make row2=0 and see for any col=0
    _delay_ms ( 1 ) ;

    buffer = Keypad_In ;

    if ( ((Keypad_Out&(1<<Row2))==0) ) {

        if 	   ( ((buffer&(1<<Col1))==0) )       Printfunction ( "4") ;
        else if  ( ( buffer & (1<<Col2) ) == 0 ) Printfunction ( "5") ;
        else if  ( ( buffer & (1<<Col3) ) == 0 ) Printfunction ( "6") ;
        else if  ( ( buffer & (1<<Col4) ) == 0 ) Printfunction ( "B") ;

    }

// Checking row 3 :

    Keypad_Out = 0xFB ; // 0xFB                                            // make row3=0 and see for any col=0
    _delay_ms ( 1 ) ;

    buffer = Keypad_In ;
    if ( ((Keypad_Out&(1<<Row3))==0) ) {

        if 		 ( ( buffer & (1<<Col1) ) == 0 ) Printfunction ( "7") ;
        else if  ( ( buffer & (1<<Col2) ) == 0 ) Printfunction ( "8") ;
        else if  ( ( buffer & (1<<Col3) ) == 0 ) Printfunction ( "9") ;
        else if  ( ( buffer & (1<<Col4) ) == 0 ) Printfunction ( "C") ;

    }

// Checking row 4 :

    Keypad_Out = 0xF7 ; // 0xF7                                             // make row4=0 and see for any col=0
    _delay_ms ( 1 ) ;

    buffer = Keypad_In ;

    if ( ((Keypad_Out&(1<<Row4))==0) ) {

        if 		 ( ( buffer & (1<<Col1) ) == 0 ) Printfunction ( "*" );
        else if  ( ( buffer & (1<<Col2) ) == 0 ) Printfunction ( "0") ;
        else if  ( ( buffer & (1<<Col3) ) == 0 ) Printfunction ( "#") ;
        else if  ( ( buffer & (1<<Col4) ) == 0 ) Printfunction ( "D") ;

    }
    _delay_ms ( 100 ) ;

}

void Commandfunction ( unsigned char a ) {

    Data = a & 0xF0 ;               // masking lower nibble
    Command = (0<<RS) | ( 1<<E) ;

    _delay_ms ( 1 ) ;
    Command = (0<<RS) | ( 0<<E) ;   // strobing enable pin

    Data = a<<4 ;                   // masking higher nibble
    Command = (0<<RS) | ( 1<<E) ;

    _delay_ms ( 1 ) ;
    Command = (0<<RS) | ( 0<<E) ;   // strobing enable pin

    _delay_ms ( 10 ) ;

}

void Datafunction ( unsigned char a) {

    Data = a & 0xF0 ;               // masking lower nibble
    Command = (1<<RS) | ( 1<<E) ;

    _delay_ms ( 1 ) ;
    Command = (1<<RS) | ( 0<<E) ;   // strobing enable pin

    Data = a<<4 ;                   // masking higher nibble
    Command = (1<<RS) | ( 1<<E) ;

    _delay_ms ( 1 ) ;
    Command = (1<<RS) | ( 0<<E) ;   // strobing enable pin

    _delay_ms ( 10 ) ;
}

void Printfunction  ( char *a) {

    for ( ; *a !='\0' ; a++ )   Datafunction ( *a ) ;

}


/******	    HARDWARE INSTRUCTIONS		 ******/

/*LCD-data is connected to PORTC */
/*LCD-command is connected to PORTD. RS = PD6 , E = PD7 */
/*Keypad is connected to PORTB */
/*See ckt diagram */


