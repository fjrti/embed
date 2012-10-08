
/*
RELEASE INFO :

    Author : Aurangzaib
    Comany : not-decided
    Date   : 28-June-12
    Version : 1

BUGS INFO :

    bug : Highly complex structure
    bug : (serious) Functions engagement/locking
    bug : once any motor starts, it runs to infinite
    info: two lcd used

*/

#ifndef DECLARATION_H_INCLUDED
#define DECLARATION_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>

#define RS              2 // register select of lcd
#define E               3 //  enable of lcd

#define Load            PORTA
#define In              PINB  // sensor input
#define Out             PORTB // sensor output

#define OMotor           PA4   // motor is connected at PA4
#define OSpeaker         PA5   // speaker is connected at PA5

#define GMotor           PA6   // motor is connected at PA4
#define GSpeaker         PA7   // speaker is connected at PA5

#define SensorDirection DDRB // conductors to sense level
#define LoadDirection   DDRA

#define HeadLcdDirecton DDRC
#define GndLcdDirecton  DDRD

#define set             bit_is_set
#define clr             bit_is_clear
#define delay 		    _delay_ms

#define OEmpty           set(In,0) && set(In,1) && set(In,2) && set(In,3)
#define OFull            clr(In,0) && clr(In,1) && clr(In,2) && clr(In,3)
#define OHalf            clr(In,0) && clr(In,1) && set(In,2) && set(In,3)
#define OOneThird        clr(In,0) && clr(In,1) && clr(In,2) && set(In,3)
#define OOneFourth       clr(In,0) && set(In,1) && set(In,2) && set(In,3)

#define GEmpty           set(In,4) && set(In,5) && set(In,6) && set(In,7)
#define GFull            clr(In,4) && clr(In,5) && clr(In,6) && clr(In,7)
#define GHalf            clr(In,4) && clr(In,5) && set(In,6) && set(In,7)
#define GOneThird        clr(In,4) && clr(In,5) && clr(In,6) && set(In,7)
#define GOneFourth       clr(In,4) && set(In,5) && set(In,6) && set(In,7)

#define OSpeaker_On       (1<<OSpeaker)
#define OSpeaker_Off      (~(1<<OSpeaker))

#define OMotor_On         (1<<OMotor)
#define OMotor_Off        (~(1<<OMotor))

#define GSpeaker_On       (1<<GSpeaker)
#define GSpeaker_Off      (~(1<<GSpeaker))

#define GMotor_On         (1<<GMotor)
#define GMotor_Off        (~(1<<GMotor))

#define OClearScreen      Commandfunction ( 0x01 , 1 )
#define GClearScreen      Commandfunction ( 0x01 , 0 )

#define OData           PORTD
#define OCommand        PORTD

#define GData           PORTC
#define GCommand        PORTC

void Initialization     (  ) ;                                      // lcd and sensor
void Commandfunction    ( unsigned char , unsigned char ) ;         // lcd command routine

void Datafunction       ( unsigned char , unsigned char ) ;         // lcd command routine
void Printfunction      ( char* , unsigned char )  ;                 // lcd print routine

void GOperation         ( ) ;                                       // sensing and decisioning
void OOperation         ( ) ;                                       // sensing and decisioning
void LoadOn             ( unsigned char ) ;                                     // starting motor and beeping

#endif // DECLARATION_H_INCLUDED
