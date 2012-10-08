/*
RELEASE INFO :

Author : Aurangzaib
Comany : pns corporation
Date   : 13-Aug-12
Version : 7.0 ( AVR Studio Version )

BUGS INFO :

system disabling included for manual controlling
Levels shown by half etc instead of 25% or 50 % etc

TO BE IMPLEMENTED :

pwm contorl of lcd backlight ( experimental, not in commercial )

*/

#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 8000000
#endif

#define Load                PORTA                                               // load port
#define LoadDirection       DDRA
#define In                  PINB                                                // sensor input
#define Out                 PORTB                                               // sensor output
#define SensorDirection     DDRB                                                // conductors to sense level
#define MiscIn              PINC                                                // line and bypass
#define MiscOut             PORTC
#define MiscDirection       DDRC
#define LcdDirecton         DDRD
#define Data                PORTD
#define Command             PORTD

#define GMotor              PA0                                                   // motor is connected at PA4
#define OMotor              PA1                                                   // motor is connected at PA4
#define Speaker             PA2                                                   // speaker is connected at PA5
#define ByPass              PC1
#define Line                PC0
#define RS                  PD0                                                   // register select of lcd
#define E                   PD1                                                   //  enable of lcd

#define set                 bit_is_set                                          // wrapper
#define clr                 bit_is_clear                                        // wrapper
#define delay 		        _delay_ms                                           // wrapper
#define micro_delay         _delay_us                                           // wrapeer
#define delaynumber         3000                                                // just to save time when editing next time
#define rechecktime         2400

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

#define LineOn              (clr(MiscIn,Line))                                       // when water is coming in line
#define LineOff             (set(MiscIn,Line))                                       // when water isn't coming in line

#define OError              ((!OEmpty) && (!OFull) && (!OHalf) && (!OOneThird) && (!OOneFourth))
#define GError              ((!GEmpty) && (!GFull) && (!GHalf) && (!GOneThird) && (!GOneFourth))
#define Error               (( OError ) || ( GError ))

#define Speaker_On         ( 1<<Speaker)
#define Speaker_Off        (~(1<<Speaker))
#define OMotor_On           (1<<OMotor)
#define OMotor_Off          (~(1<<OMotor))
#define GMotor_On           (1<<GMotor)
#define GMotor_Off          (~(1<<GMotor))

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
void LoadOff                ( unsigned char ) ;                             // stopping motors and speakers
void buzzer                 ( ) ;                                           // beeping
unsigned char LineChecking  ( ) ;

#endif // HEADER_H_INCLUDED
