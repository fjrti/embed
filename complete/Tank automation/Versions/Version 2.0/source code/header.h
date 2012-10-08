/*
RELEASE INFO :

    Author : Aurangzaib
    Comany : not-decided
    Date   : 25-July-12
    Version : 2

BUGS INFO :

    highly modular structure
    bug : once motor starts , it runs to infinite
    bug : erroneous level notification and action systems
    fix : buzzer function included
    fix : loadOff is done by macro

*/

#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <avr/io.h>
#include <util/delay.h>

#define RS                  2 // register select of lcd
#define E                   3 //  enable of lcd

#define Load                PORTA
#define In                  PINB  // sensor input
#define Out                 PORTB // sensor output

#define OMotor              PA4   // motor is connected at PA4
#define GMotor              PA6   // motor is connected at PA4

#define Speaker            PA5   // speaker is connected at PA5

#define SensorDirection     DDRB // conductors to sense level
#define LoadDirection       DDRA

#define LcdDirecton         DDRC

#define set                 bit_is_set
#define clr                 bit_is_clear
#define delay 		        _delay_ms
#define delaynumber         1400

#define OEmpty              set(In,0) && set(In,1) && set(In,2) && set(In,3)
#define OFull               clr(In,0) && clr(In,1) && clr(In,2) && clr(In,3)
#define OHalf               clr(In,0) && clr(In,1) && set(In,2) && set(In,3)
#define OOneThird           clr(In,0) && clr(In,1) && clr(In,2) && set(In,3)
#define OOneFourth          clr(In,0) && set(In,1) && set(In,2) && set(In,3)

#define GEmpty              set(In,4) && set(In,5) && set(In,6) && set(In,7)
#define GFull               clr(In,4) && clr(In,5) && clr(In,6) && clr(In,7)
#define GHalf               clr(In,4) && clr(In,5) && set(In,6) && set(In,7)
#define GOneThird           clr(In,4) && clr(In,5) && clr(In,6) && set(In,7)
#define GOneFourth          clr(In,4) && set(In,5) && set(In,6) && set(In,7)

#define Error               ((!OOneFourth)&((OOneThird)|(OHalf)|(OFull)))  | ((!GOneFourth)&((GOneThird)|(GHalf)|(GFull)))

#define Speaker_On         (  1<<Speaker)
#define Speaker_Off        (~(1<<Speaker))

#define OMotor_On           (1<<OMotor)
#define OMotor_Off          (~(1<<OMotor))

#define GMotor_On           (1<<GMotor)
#define GMotor_Off          (~(1<<GMotor))

#define Data                PORTC
#define Command             PORTC

#define off                 0
#define on                  1

#define ground              0
#define overhead            1

void OClearScreen           ( ) ;
void GClearScreen           ( ) ;

void Initialization         (  ) ;                                      // lcd and sensor
void Commandfunction        ( unsigned char ) ;                         // lcd command routine

void Datafunction           ( unsigned char ) ;                         // lcd command routine
void Printfunction          ( char* )  ;                                // lcd print routine

void Operation              ( ) ;                                       // sensing and decisioning
void LoadOn        ( unsigned char ) ;                         // starting motor and beeping
void buzzer                 ( ) ;                                       // beeping


#endif // HEADER_H_INCLUDED
