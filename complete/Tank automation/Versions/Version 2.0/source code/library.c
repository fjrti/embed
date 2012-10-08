#ifndef LIBRARY_C_INCLUDED
#define LIBRARY_C_INCLUDED
#include "header.h"


///  ____________________________________________________________________________________________ INITIALIZATION


void Initialization( )
{

    LcdDirecton         = 0xFF ;                                            // as output
    SensorDirection     = 0x00 ;                                            // as input
    LoadDirection       = 0xFF ;                                            // motor and speaker as output
    Load                = off  ;

    Commandfunction ( 0x33 )    ;                                           // for 4 bit lcd
    Commandfunction ( 0x32 )    ;                                           // for 4 bit lcd
    Commandfunction ( 0x28 )    ;                                           // for 4 bit lcd
    Commandfunction ( 0x0E )    ;                                           // cursor on
    Commandfunction ( 0x06 )    ;
    Commandfunction ( 0x01 )    ;                                           // clear

}


///  ____________________________________________________________________________________________ OVERHEAD SCREEN CLEAR


void OClearScreen ( )
{

    Commandfunction ( 0x80 ) ;
    Printfunction ( "                   " ) ;
    Commandfunction ( 0x80 ) ;

}


///  ____________________________________________________________________________________________ GROUND SCREEN CLEAR


void GClearScreen ( )
{

    Commandfunction ( 0xC0 ) ;
    Printfunction ( "                       " ) ;
    Commandfunction ( 0xC0 ) ;

}


///  ____________________________________________________________________________________________ OPERATIONS


void Operation ( )
{

    for ( ;; )
        {
            Out= 0xFF ;                                                     // Empty by default

//----------------------------------------------------------------  Empty conditions

            if ( OEmpty )
                {
                    OClearScreen() ;
                    Printfunction ( "Upper : Empty" ) ;
                    buzzer() ;
                }

            if ( GEmpty )
                {
                    GClearScreen() ;
                    Printfunction ( "Ground : Empty" ) ;
                    buzzer() ;
                }

//----------------------------------------------------------------  Full conditions

            if ( OFull )
                {
                    OClearScreen() ;
                    Printfunction ( "Upper : Full" ) ;
                    buzzer() ;
                }

            if ( GFull )
                {
                    GClearScreen() ;
                    Printfunction ( "Ground : Full" ) ;
                    buzzer() ;
                }

//----------------------------------------------------------------  One-third conditions

            if( OOneThird )
                {
                    OClearScreen( )  ;
                    Printfunction ( "Upper : 3/4" ) ;
                    delay ( 1400 ) ;
                }

            if( GOneThird )
                {
                    GClearScreen( )  ;
                    Printfunction ( "Ground : 3/4" ) ;
                    delay ( 1400 ) ;
                }

//----------------------------------------------------------------  Half conditions

            if ( OHalf )
                {
                    OClearScreen( )  ;
                    Printfunction ( "Upper : 1/2" ) ;
                    delay ( delaynumber ) ;
                }
            if ( GHalf )
                {
                    GClearScreen( )  ;
                    Printfunction ( "Ground : 1/2" ) ;
                    delay ( delaynumber ) ;
                }

//----------------------------------------------------------------  One forth conditions

            if( OOneFourth )
                {
                    if ( ( GFull ) | ( GOneThird ) | ( GHalf ) )
                        {
                            OClearScreen( )  ;
                            Printfunction( "Upper : 1/4" ) ;
                            delay ( delaynumber ) ;
                            OClearScreen( )  ;
                            Printfunction( "Upper : filling" ) ;

                            delay( delaynumber ) ;
                            LoadOn ( overhead )  ;
                        }


                    else if ( ( GOneFourth ) | ( GEmpty ) )
                        {

                            OClearScreen( )  ;
                            Load &= GMotor_Off ;
                            Printfunction( "Motor cant run" ) ;
                            delay ( delaynumber ) ;
                            OClearScreen( )  ;
                            Printfunction( "Lower level low" ) ;

                        }
                }

            if( GOneFourth )
                {

                    GClearScreen( )  ;
                    Printfunction( "Lower : 1/4"  ) ;
                    delay ( delaynumber ) ;
                    GClearScreen( )  ;
                    Printfunction( "Lower : filling" ) ;

                    delay( delaynumber ) ;
                    LoadOn ( ground )  ;

                }

//-----------------------------------------------------------------  faulty conditions

            else if  ( Error )
                {

                    Commandfunction( 0x01 ) ;
                    Printfunction ( "Error in tanks" ) ;
                    buzzer() ;

                }
        }
}


/// ____________________________________________________________________________________________ RUNNING THE LOAD


void LoadOn ( unsigned char number )
{

    if ( number == overhead )
        {
            if ( !( OOneFourth ) )
                {
                    Load &= OMotor_Off ;
                    OClearScreen( )  ;
                    Printfunction ( "Upper Motor off" ) ;
                    delay ( delaynumber ) ;
                }
            else
                {
                    Load |= OMotor_On ;
                    buzzer() ;
                }
        }

    else if ( number == ground )
        {
            if ( !( GOneFourth ) )
                {
                    Load &= GMotor_Off ;
                    GClearScreen( )  ;
                    Printfunction ( "Lower Motor off" ) ;
                    delay ( delaynumber ) ;

                }
            else
                {
                    Load |= GMotor_On ;
                    buzzer() ;
                }
        }
}


///  ____________________________________________________________________________________________ LCD COMMANDS


void Commandfunction ( unsigned char a )
{

    Data = ( a&0xF0 ) | ( Data&0x0F ) ;           // masking lower nibble

    Command &= ( ~( 1<<RS ) ) ;
    Command |= ( 1<<E ) ;

    delay ( 1 ) ;

    Command &= ( ~( 1<<RS ) ) ;
    Command &= ( ~( 1<<E ) )  ;

    Data = ( a<<4 ) | ( Data&0x0F ) ;               // masking higher nibble

    Command &= ( ~( 1<<RS ) ) ;
    Command |= ( 1<<E ) ;

    delay ( 1 ) ;

    Command &= ( ~( 1<<RS ) ) ;
    Command &= ( ~( 1<<E ) ) ;

    delay ( 10 ) ;

}


///  ____________________________________________________________________________________________ LCD DATA


void Datafunction ( unsigned char a )
{

    Data = ( a&0xF0 ) | ( Data&0x0F ) ;           // masking lower nibble

    Command |= ( 1<<RS ) ;
    Command |= ( 1<<E ) ;

    delay ( 1 ) ;

    Command |= ( 1<<RS ) ;
    Command &= ( ~( 1<<E ) )  ;

    Data = ( a<<4 ) | ( Data&0x0F ) ;               // masking higher nibble

    Command |= ( 1<<RS ) ;
    Command |= ( 1<<E ) ;

    delay ( 1 ) ;

    Command |= ( 1<<RS ) ;
    Command &= ( ~( 1<<E ) ) ;

    delay ( 10 ) ;

}


///  ____________________________________________________________________________________________ LCD PRINTING


void Printfunction  ( char *a )
{
    for ( ; *a !='\0' ; a++ ) { Datafunction ( *a ) ; }
}


///  ____________________________________________________________________________________________ SPEAKER OPERATIONS


void buzzer ( )
{
    unsigned char loop = 0 ;
    for ( ; loop<5 ; loop++ )
        {

            Load |= Speaker_On  ;
            delay ( 100 ) ;
            Load &= Speaker_Off ;

        }
}
#endif // LIBRARY_C_INCLUDED
