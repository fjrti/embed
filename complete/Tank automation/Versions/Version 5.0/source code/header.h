/*

RELEASE INFO :

    Author : Aurangzaib
    Comany : not-decided
    Date   : 29-July-12
    Version : 4

BUGS INFO :

    bug : artificial error conditions
    bug : bitwise operators instead of || in functions, by mistake

*/

#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <avr/io.h>
#include <util/delay.h>

#define RS                  2                                                   // register select of lcd
#define E                   3                                                   //  enable of lcd

#define Load                PORTA                                               // load port
#define In                  PINB                                                // sensor input
#define Out                 PORTB                                               // sensor output

#define OMotor              4                                                   // motor is connected at PA4
#define GMotor              6                                                   // motor is connected at PA4
#define Speaker             5                                                   // speaker is connected at PA5

#define SensorDirection     DDRB                                                // conductors to sense level
#define LoadDirection       DDRA

#define LcdDirecton         DDRC

#define set                 bit_is_set                                          // wrapper
#define clr                 bit_is_clear                                        // wrapper
#define delay 		        _delay_ms                                           // wrapper

#define delaynumber         1400                                                // just to save time when editing next time
#define rechecktime         1000

#define OEmpty              (set(In,0) && set(In,1) && set(In,2) && set(In,3))  // overhead tank empty condition
#define OFull               (clr(In,0) && clr(In,1) && clr(In,2) && clr(In,3))  // overhead tank full condition
#define OHalf               (clr(In,0) && clr(In,1) && set(In,2) && set(In,3))  // overhead tank half condition
#define OOneThird           (clr(In,0) && clr(In,1) && clr(In,2) && set(In,3))  // overhead tank one-third condition
#define OOneFourth          (clr(In,0) && set(In,1) && set(In,2) && set(In,3))  // overhead tank one-fourth condition

#define GEmpty              (set(In,4) && set(In,5) && set(In,6) && set(In,7))  // ground tank empty condition
#define GFull               (clr(In,4) && clr(In,5) && clr(In,6) && clr(In,7))  // ground tank full condition
#define GHalf               (clr(In,4) && clr(In,5) && set(In,6) && set(In,7))  // ground tank half condition
#define GOneThird           (clr(In,4) && clr(In,5) && clr(In,6) && set(In,7))  // ground tank one-third condition
#define GOneFourth          (clr(In,4) && set(In,5) && set(In,6) && set(In,7))  // ground tank one-fourth condition

//#define OCondition1         set(In,0) && (set(In,1) || set(In,2) || clr(In,3))
//#define OCondition2         clr(In,0) && set(In,1) && clr(In,2) && clr(In,3)
//#define OCondition3         clr(In,0) && clr(In,1) && set(In,2) && clr(In,3)
//#define OCondition4         clr(In,0) && clr(In,1) && set(In,2) && clr(In,3)
//
//#define GCondition1         set(In,4) && (set(In,5) || set(In,6) || clr(In,7))
//#define GConditionTwo       clr(In,4) && set(In,5) && clr(In,6) && clr(In,7)
//#define GConditionThree     clr(In,4) && clr(In,5) && set(In,6) && clr(In,7)
//
//#define OError              (OConditionOne) || (OConditionTwo) || (OConditionThree)
//#define GError              (GConditionOne) || (GConditionTwo) || (GConditionThree)

#define OError              (!OEmpty) && (!OFull) && (!OHalf) && (!OOneThird) && (!OOneFourth)
#define GError              (!GEmpty) && (!GFull) && (!GHalf) && (!GOneThird) && (!GOneFourth)

#define Speaker_On         (  1<<Speaker)
#define Speaker_Off        (~(1<<Speaker))

#define OMotor_On           (1<<OMotor)
#define OMotor_Off          (~(1<<OMotor))

#define GMotor_On           (1<<GMotor)
#define GMotor_Off          (~(1<<GMotor))

#define Data                PORTC
#define Command             PORTC

#define off                 0                                               // wrapper
#define on                  1

#define ground              0
#define overhead            1

void OClearScreen           ( ) ;                                           // to clear lcd portion for overhead tank
void GClearScreen           ( ) ;                                           // to clear lcd portion for ground tank

void Initialization         (  ) ;                                          // lcd and sensor
void Commandfunction        ( unsigned char ) ;                             // lcd command routine
void Datafunction           ( unsigned char ) ;                             // lcd command routine
void Printfunction          ( char* )  ;                                    // lcd print routine

void Operation              ( ) ;                                           // sensing and decisioning
void LoadOn                 ( unsigned char ) ;                             // starting motors and speakers
void buzzer                 ( ) ;                                           // beeping


#endif // HEADER_H_INCLUDED
