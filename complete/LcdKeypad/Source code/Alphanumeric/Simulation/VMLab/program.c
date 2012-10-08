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

#define Command PORTD	// Lcd command port
#define Data PORTC	// Lcd data port

#define  RS PD6		// Lcd register select pin
#define  E PD7		// Lcd Enable pin

#define Row1 PB0
#define Row2 PB1
#define Row3 PB2
#define Row4 PB3

#define Col1 PB4
#define Col2 PB5
#define Col3 PB6
#define Col4 PB7

// declaration :

void Initialization (    ) ;

void Checking (  ) ;
void LCD_command (unsigned char ) ;

void LCD_data (unsigned char ) ;
void LCD_print (char*) ;


/******		MAIN FUNCTION		******/

int main (  ) {

    Initialization( ) ;                                                     // Initialize LCD
    LCD_print( "Press key" ) ;
    _delay_ms ( 10 ) ;

    LCD_command( 0x01 ) ;
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

    DDRB = 0X0F ;                                                   // row as output , col as input ( 0b00001111 )
    DDRC = 0XFF ;                                                   // data port - LCD

    DDRD = (1<<RS) | (1<<E) ;
    Keypad_Out = 0xF0 ;

//	setting lcd :

    LCD_command(0x38) ;                                                 // 8 bit mode
    _delay_ms (1) ;

    LCD_command(0x06) ;                                                 // cursor at right
    _delay_ms (1) ;

    LCD_command(0x0E) ;                                                 // cursor blinking
    _delay_ms (1) ;

    LCD_command(0x01) ;                                                  // clear lcd
    _delay_ms (2) ;

}

// Keypad Operation  Routine :

void Checking(  ) {

// Checking row 1 :

    Keypad_Out = 0xFE ; // 0xFE                                          // make row1=0 and see for any col=0
    _delay_ms ( 1 ) ;

    unsigned char buffer = Keypad_In ;

    if ( ((Keypad_Out&(1<<Row1))==0) ) {

        if 	   ( ((buffer&(1<<Col1))==0) )      LCD_print ( "0") ;
        else if ( ( buffer & (1<<Col2) ) == 0 ) LCD_print ( "1") ;
        else if ( ( buffer & (1<<Col3) ) == 0 ) LCD_print ( "2") ;
        else if ( ( buffer & (1<<Col4) ) == 0 ) LCD_print ( "3") ;

    }

// Checking row 2 :

    Keypad_Out = 0xFD ;     // 0xFD                                       // make row2=0 and see for any col=0
    _delay_ms ( 1 ) ;

    buffer = Keypad_In ;

    if ( ((Keypad_Out&(1<<Row2))==0) ) {

        if 	   ( ((buffer&(1<<Col1))==0) )       LCD_print ( "4") ;
        else if  ( ( buffer & (1<<Col2) ) == 0 ) LCD_print ( "5") ;
        else if  ( ( buffer & (1<<Col3) ) == 0 ) LCD_print ( "6") ;
        else if  ( ( buffer & (1<<Col4) ) == 0 ) LCD_print ( "7") ;

    }

// Checking row 3 :

    Keypad_Out = 0xFB ; // 0xFB                                            // make row3=0 and see for any col=0
    _delay_ms ( 1 ) ;

    buffer = Keypad_In ;
    if ( ((Keypad_Out&(1<<Row3))==0) ) {

        if 		 ( ( buffer & (1<<Col1) ) == 0 ) LCD_print ( "8") ;
        else if  ( ( buffer & (1<<Col2) ) == 0 ) LCD_print ( "9") ;
        else if  ( ( buffer & (1<<Col3) ) == 0 ) LCD_print ( "A") ;
        else if  ( ( buffer & (1<<Col4) ) == 0 ) LCD_print ( "B") ;

    }

// Checking row 4 :

    Keypad_Out = 0xF7 ; // 0xF7                                             // make row4=0 and see for any col=0
    _delay_ms ( 1 ) ;

    buffer = Keypad_In ;

    if ( ((Keypad_Out&(1<<Row4))==0) ) {

        if 		 ( ( buffer & (1<<Col1) ) == 0 ) LCD_print ( "C" ) ;
        else if  ( ( buffer & (1<<Col2) ) == 0 ) LCD_print ( "D") ;
        else if  ( ( buffer & (1<<Col3) ) == 0 ) LCD_print ( "E") ;
        else if  ( ( buffer & (1<<Col4) ) == 0 ) LCD_print ( "F") ;

    }
    _delay_ms ( 20 ) ;
    LCD_command( 0x01 ) ;

}

// Command sending Routine :

void LCD_command ( unsigned char a ) {

    Data = a ;
    Command = (0<<RS) | (1<<E) ;
    _delay_ms (1) ;
    Command = (0<<RS) | (0<<E) ;
    _delay_ms ( 40 ) ;

}

// Data Sending Routine :

void LCD_data ( unsigned char a ) {

    Data = a ;
    Command = (1<<RS) | (1<<E) ;
    _delay_ms (1) ;
    Command = (1<<RS) | (0<<E) ;
    _delay_ms ( 40 ) ;
}


// Data Printing Routine :

void LCD_print ( char *a) {

    unsigned char i = 0 ;
    for (; a[i]!='\0' ; i++)      LCD_data( a[i] ) ;
}


/******	    HARDWARE INSTRUCTIONS		 ******/

/*LCD-data is connected to PORTC */
/*LCD-command is connected to PORTD. RS = PD6 , E = PD7 */
/*Keypad is connected to PORTB */
/*See ckt diagram */


