

/**
	Author : Aurangzaib Siddiqui
	GNU public license
    Compiler : gcc-avr 4.6

	ABSTRACT :
	This program takes temperature in form of binary voltage at PA0
	Then convert it to ASCII
	And send to 16x2 LCD

**/


// Compiler Directives :

#include <avr/io.h>
#include <util/delay.h>

#define RS 			        6
#define E 			        7

#define Data 			    	PORTC
#define Command 		    	PORTD
#define ADC_Pin			        PINA

#define ADC_direction 			DDRA
#define Data_direction 			DDRC
#define Command_direction 		DDRD


// function declaration :


void Initialization     (  )  ;
unsigned char Recieving (  )  ;

void Operation          ( unsigned char ) ;
void Commandfunction    ( unsigned char ) ;

void Datafunction       ( unsigned char ) ;
void LCD                ( char*)  ;


// main function :


int main                ( ) {

    Initialization      ( ) ;// lcd and adc
    LCD                 ( "Starting..." ) ;
    Commandfunction(0xc3) ;
    // Second line , 3rd character

    while ( 1 ) {
        Commandfunction (0x01) ;
        unsigned char variable = 0 ;
        for ( ; variable < 10 ; variable++)
        		Operation           (variable) ;

    }
    return 0 ;
}


// function definition :


void Initialization     ( ) {

    Data_direction = 0xFF ;
    Command_direction = 0xFF ;
    ADC_direction &= 0<<ADC_Pin ;

    ADMUX  = (1<<REFS1) | (1<<REFS0) | (1<<ADLAR) ; // 0xE0
    ADCSRA = (1<<ADEN)  | (1<<ADPS2) | (1<<ADPS1) ; // clk/4

    Commandfunction     ( 0x33 ) ;
    // 4 bit mode is used
    _delay_ms           (1) ;

    Commandfunction     ( 0x32 ) ;
    _delay_ms           (1) ;

    Commandfunction     ( 0x28 ) ;
    _delay_ms           (1) ;

    Commandfunction     ( 0x0E ) ;
    _delay_ms           (1) ;

    Commandfunction     ( 0x01 ) ;
    _delay_ms           (1) ;

    Commandfunction     ( 0x06 ) ;
    _delay_ms           (1) ;

}


void Commandfunction    ( unsigned char a) { // 4 bit method is used

    Data = a & 0xF0 ;
    // masking lower nibble

    Command = (0<<RS) | ( 1<<E) ;
    _delay_ms           ( 1) ;

    Command = (0<<RS) | ( 0<<E) ;
    Data = a<<4 ;
    // masking higher nibble

    Command = (0<<RS) | ( 1<<E) ;
    _delay_ms           ( 1) ;

    Command = (0<<RS) | ( 0<<E) ;
    _delay_ms           ( 50 ) ;

}


void Datafunction       ( unsigned char a) { // 4 bit method is used

    Data = a & 0xF0 ;
    // masking lower nibble

    Command = (1<<RS) | ( 1<<E) ;
    _delay_ms           ( 1 ) ;
    Command = (1<<RS) | ( 0<<E) ;

    Data = a<<4 ;
    // masking higher nibble

    Command = (1<<RS) | ( 1<<E) ;
    _delay_ms           ( 1 ) ;
    Command = (1<<RS) | ( 0<<E) ;

    _delay_ms           ( 50 ) ;
}


void LCD                ( char *a) { // char char to LCD

    unsigned char z=0 ;
    for ( ; a[z] !=0 ; z++ )    Datafunction ( a[z] ) ;

}


unsigned char Recieving (  ) {

    ADCSRA |= ( 1<<ADSC ) ; // setting on start conversion bit

    while               ( (ADCSRA & (1<<ADIF)) == 0 ) ; // until interrupt flag is low
    ADCSRA |= (1<<ADIF) ;

    return              ( ADC ) ; // ADC means ADCH and ADCL

}


void Operation          ( unsigned char variable ) {

//      From binary to ASCII

    unsigned char convert , loop = 0 ;
    char position = 0xc3 ;

    for  ( ; loop<3 ; loop++ ) {

        convert = variable % 10 ; // to make it bcd
        variable /= 10 ;

        Commandfunction ( position ) ; // this is very important

        Datafunction ( convert + 48 ) ; // to make data ascii
        position-- ;
    }
Commandfunction( 0xC5) ;
    LCD( " *C") ; // degree centigrade

    _delay_ms(5 ) ;
    // show new data every 5th second

}




