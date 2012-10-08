#include "Declaration.h"

void Initialization( )
{
    HeadLcdDirecton = GndLcdDirecton = 0xFF ;                        // as output
    SensorDirection = 0x00 ;                                         // as input
    LoadDirection   = 0xFF ;                                        // motor and speaker as output
    Load = 0 ;

    unsigned char number = 0 ;

    Commandfunction ( 0x33 , number ) ;     // for 4 bit lcd
    Commandfunction ( 0x32 , number ) ;     // for 4 bit lcd
    Commandfunction ( 0x28 , number  ) ;    // for 4 bit lcd
    Commandfunction ( 0x0E , number  ) ;    // cursor on
    Commandfunction ( 0x06 , number  ) ;
    Commandfunction ( 0x01 , number  ) ;    // clear

    number = 1 ;

    Commandfunction ( 0x33 , number ) ;     // for 4 bit lcd
    Commandfunction ( 0x32 , number ) ;     // for 4 bit lcd
    Commandfunction ( 0x28 , number  ) ;    // for 4 bit lcd
    Commandfunction ( 0x0E , number  ) ;    // cursor on
    Commandfunction ( 0x06 , number  ) ;
    Commandfunction ( 0x01 , number  ) ;    // clear

}

void OOperation ( )
{
    unsigned char number = 1 ;
    for ( ;; )
        {
            Out= 0xFF ; // Empty by default
            OClearScreen  ;

            if ( OEmpty )
                {
                    Printfunction ( "Overhead : Empty" , number ) ;
                    GOperation() ;
                    while ( OEmpty ) ;
                    OClearScreen  ;
                }

            else if ( OFull )
                {
                    Printfunction ( "Overhead : Full" , number ) ;
                    while ( OFull ) ;
                    OClearScreen  ;
                    GOperation() ;
                }

            else if( OOneThird )
                {
                    Printfunction ( "Overhead : 3/4" , number ) ;
                    GOperation() ;
                    while( OOneThird ) ;
                    OClearScreen  ;

                }

            else if ( OHalf )
                {
                    Printfunction ( "Overhead : 1/2" , number ) ;
                    GOperation() ;
                    while ( OHalf ) ;
                    OClearScreen  ;
                }

            else if( OOneFourth )
                {
                    Commandfunction ( 0x80 , number ) ;
                    Printfunction ( "Overhead : 1/4" , number ) ; // 1/4
                    Commandfunction ( 0x01 , number ) ;
                    if ( GFull | GOneThird | GHalf )
                        {
                            Printfunction( "Overhead Motor"  , number ) ;
                            Commandfunction( 0xC0 , number ) ;
                            Printfunction( "starting" , number )  ;

                            delay ( 200 ) ;
                            OClearScreen  ;

                            LoadOn ( number ) ;
                            OClearScreen  ;

                            Load |= OSpeaker_On ;

                            delay ( 100 ) ;
                            Load &= OSpeaker_Off ;
                        }

                    else if ( GOneFourth )
                        {
                            Printfunction( "Gnd motor dont" , number  ) ;
                            Commandfunction( 0xC0 , number ) ;
                            Printfunction( "have water" , number )  ;
                            LoadOn( 0 ) ; // ground unit motor is started
                        }
                }

            else
                {
                    Printfunction ( "Error in" , number ) ;
                    Commandfunction( 0xC0 , 1 ) ;
                    Printfunction( "Overhead tank" , number )  ;

                }

            delay ( 200 ) ;
        }
}

void GOperation ( )
{
    unsigned char number = 0 ;
    for ( ;; )
        {
            Out= 0xFF ; // Empty by default
            GClearScreen  ;

            if ( GEmpty )
                {
                    Printfunction ( "Ground : Empty" , number ) ;
                    OOperation() ;
                    while ( GEmpty ) ;
                    GClearScreen  ;
                }

            else if ( GFull )
                {
                    Printfunction ( "Ground : Full" , number ) ;
                    OOperation() ;
                    while ( GFull ) ;
                    GClearScreen  ;
                }

            else if( GOneThird )
                {
                    Printfunction ( "Ground : 3/4" , number ) ;
                    OOperation() ;
                    while( GOneThird ) ;
                    GClearScreen  ;
                }

            else if ( GHalf )
                {

                    Printfunction ( "Ground : 1/2" , number ) ;
                    OOperation() ;
                    while ( GHalf ) ;
                    GClearScreen  ;

                }

            else if( GOneFourth )
                {
                    Commandfunction ( 0x80 , number ) ;
                    Printfunction ( "Ground : 1/4" , number ) ; // 1/4
                    Commandfunction ( 0x01 , number ) ;

                    Printfunction( "Ground Motor"  , number ) ;
                    Commandfunction( 0xC0 , number ) ;
                    Printfunction( "starting" , number )  ;

                    delay ( 200 ) ;
                    GClearScreen  ;

                    LoadOn ( number ) ;
                    GClearScreen  ;
                }

            else
                {
                    Printfunction ( "Error in" , number ) ;
                    Commandfunction( 0xC0 , number ) ;
                    Printfunction( "Ground tank" , number )  ;

                    Load |= GSpeaker_On ;

                    delay ( 100 ) ;
                    Load &= GSpeaker_Off ;
                }

            delay ( 200 ) ;
        }
}

void LoadOn ( unsigned char number )
{

    if ( number == 1 )
        {
            OClearScreen ;
            while ( OOneFourth )
                {
                    if ( !OOneFourth )
                        {
                            Load &= OMotor_Off ;
                            OClearScreen  ;
                            Printfunction ( "Overhead Motor off now" , number ) ;
                            delay ( 1000 ) ;
                            break ;
                        }
                    Commandfunction( 0x80 , number  ) ;
                    Printfunction( "Overhead Motor"  , number ) ;

                    Commandfunction( 0xC0 , number ) ;
                    Printfunction( "starting" , number )  ;

                    Load |= OMotor_On ;
                    Load |= OSpeaker_On ;

                    delay ( 1000 ) ;
                    Load &= OSpeaker_Off ;
                    delay ( 1000 ) ;

                    GOperation() ;
                }
        }

    else if ( number == 0 )
        {
            GClearScreen ;
            while ( GOneFourth )
                {
                    if ( !GOneFourth )
                        {
                            Load &= GMotor_Off ;
                            GClearScreen  ;
                            Printfunction ( "Ground Motor off now" , number ) ;
                            delay ( 1000 ) ;
                            break ;
                        }
                    Commandfunction( 0x80 , number  ) ;
                    Printfunction( "Ground Motor"  , number ) ;

                    Commandfunction( 0xC0 , number ) ;
                    Printfunction( "starting" , number )  ;

                    Load |= GMotor_On ;
                    Load |= GSpeaker_On ;

                    delay ( 1000 ) ;
                    Load &= GSpeaker_Off ;
                    delay ( 1000 ) ;

                    OOperation() ;
                }
        }
}

void Commandfunction ( unsigned char a, unsigned char number )
{
    if ( number == 1 )
        {
            OData = ( a&0xF0 ) | ( OData&0x0F ) ;           // masking lower nibble

            OCommand &= ( ~( 1<<RS ) ) ;
            OCommand |= ( 1<<E ) ;

            delay ( 1 ) ;

            OCommand &= ( ~( 1<<RS ) ) ;
            OCommand &= ( ~( 1<<E ) )  ;

            OData = ( a<<4 ) | ( OData&0x0F ) ;               // masking higher nibble

            OCommand &= ( ~( 1<<RS ) ) ;
            OCommand |= ( 1<<E ) ;

            delay ( 1 ) ;

            OCommand &= ( ~( 1<<RS ) ) ;
            OCommand &= ( ~( 1<<E ) ) ;

            delay ( 10 ) ;
        }
    else if ( number == 0 )
        {
            GData = ( a&0xF0 ) | ( GData&0x0F ) ;           // masking lower nibble

            GCommand &= ( ~( 1<<RS ) ) ;
            GCommand |= ( 1<<E ) ;

            delay ( 1 ) ;

            GCommand &= ( ~( 1<<RS ) ) ;
            GCommand &= ( ~( 1<<E ) )  ;

            GData = ( a<<4 ) | ( GData&0x0F ) ;               // masking higher nibble

            GCommand &= ( ~( 1<<RS ) ) ;
            GCommand |= ( 1<<E ) ;

            delay ( 1 ) ;

            GCommand &= ( ~( 1<<RS ) ) ;
            GCommand &= ( ~( 1<<E ) ) ;

            delay ( 10 ) ;
        }
}

void Datafunction ( unsigned char a, unsigned char number )
{
    if ( number == 0 )
        {
            GData = ( a&0xF0 ) | ( GData&0x0F ) ;           // masking lower nibble

            GCommand |= ( 1<<RS ) ;
            GCommand |= ( 1<<E ) ;

            delay ( 1 ) ;

            GCommand |= ( 1<<RS ) ;
            GCommand &= ( ~( 1<<E ) )  ;

            GData = ( a<<4 ) | ( GData&0x0F ) ;               // masking higher nibble

            GCommand |= ( 1<<RS ) ;
            GCommand |= ( 1<<E ) ;

            delay ( 1 ) ;

            GCommand |= ( 1<<RS ) ;
            GCommand &= ( ~( 1<<E ) ) ;

            delay ( 10 ) ;
        }
    else if ( number == 1 )
        {
            OData = ( a&0xF0 ) | ( OData&0x0F ) ;           // masking lower nibble

            OCommand |= ( 1<<RS ) ;
            OCommand |= ( 1<<E ) ;

            delay ( 1 ) ;

            OCommand |= ( 1<<RS ) ;
            OCommand &= ( ~( 1<<E ) )  ;

            OData = ( a<<4 ) | ( OData&0x0F ) ;               // masking higher nibble

            OCommand |= ( 1<<RS ) ;
            OCommand |= ( 1<<E ) ;

            delay ( 1 ) ;

            OCommand |= ( 1<<RS ) ;
            OCommand &= ( ~( 1<<E ) ) ;

            delay ( 10 ) ;
        }
}

void Printfunction  ( char *a, unsigned char number )
{
    for ( ; *a !='\0' ; a++ )
        {
            Datafunction ( *a , number ) ;
            delay ( 10 ) ;
        }
}
