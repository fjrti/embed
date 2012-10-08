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

    Commandfunction ( 0x80 ) ;                                              // selecting just upper portion of lcd
    Printfunction ( "                   " ) ;                               // gives effect of clear upper lcd row
    Commandfunction ( 0x80 ) ;

}


///  ____________________________________________________________________________________________ GROUND SCREEN CLEAR


void GClearScreen ( )
{

    Commandfunction ( 0xC0 ) ;                                              // selecting just lower portion of lcd
    Printfunction ( "                       " ) ;                           // gives effect of clear lower lcd row
    Commandfunction ( 0xC0 ) ;

}


///  ____________________________________________________________________________________________ OPERATIONS


void Operation ( )
{

    for ( ;; )
        {
            Out= 0xFF ;                                                     // tanks empty by default

//----------------------------------------------------------------  Empty conditions

            if ( OEmpty )                                                   // overhead-tank empty condition
                {
                    delay ( rechecktime ) ;
                    if ( OEmpty )                                                   // overhead-tank empty condition
                        {
                            OClearScreen() ;
                            Printfunction ( "Upper : Empty" ) ;
                            buzzer() ;
                        }
                }

            if ( GEmpty )                                                   // ground-tank empty condition
                {
                    delay ( rechecktime ) ;
                    if ( GEmpty )                                                   // ground-tank empty condition
                        {
                            GClearScreen() ;
                            Printfunction ( "Lower : Empty" ) ;
                            buzzer() ;
                        }
                }

//----------------------------------------------------------------  Full conditions

            if ( OFull )                                                   // overhead-tank full condition
                {
                    delay ( rechecktime ) ;
                    if ( OFull )                                                   // overhead-tank full condition
                        {

                            OClearScreen() ;
                            Printfunction ( "Upper : Full" ) ;
                            buzzer() ;
                        }
                }

            if ( GFull )                                                   // ground-tank full condition
                {
                    delay ( rechecktime ) ;
                    if ( GFull )                                                   // ground-tank full condition
                        {
                            GClearScreen() ;
                            Printfunction ( "Lower : Full" ) ;
                            buzzer() ;
                        }
                }


//----------------------------------------------------------------  One-third conditions

            if( OOneThird )                                                   // overhead-tank onethird condition
                {
                    delay ( rechecktime ) ;
                    if( OOneThird )                                                   // overhead-tank onethird condition
                        {
                            OClearScreen( )  ;
                            Printfunction ( "Upper : 3/4" ) ;
                            delay ( delaynumber ) ;
                        }
                }

            if( GOneThird )                                                   // ground-tank onethird condition
                {
                    delay ( rechecktime ) ;
                    if( GOneThird )                                                   // ground-tank onethird condition
                        {
                            GClearScreen( )  ;
                            Printfunction ( "Lower : 3/4" ) ;
                            delay ( delaynumber ) ;
                        }
                }

//----------------------------------------------------------------  Half conditions

            if ( OHalf )                                                   // overhead-tank half condition
                {
                    delay ( rechecktime ) ;
                    if ( OHalf )                                                   // overhead-tank half condition
                        {
                            OClearScreen( )  ;
                            Printfunction ( "Upper : 1/2" ) ;
                            delay ( delaynumber ) ;
                        }
                }

            if ( GHalf )                                                   // ground-tank half condition
                {
                    delay ( rechecktime ) ;
                    if ( GHalf )                                                   // ground-tank half condition
                        {
                            GClearScreen( )  ;
                            Printfunction ( "Lower : 1/2" ) ;
                            delay ( delaynumber ) ;
                        }
                }

//----------------------------------------------------------------  One forth conditions

            if( OOneFourth )
                {
                    delay ( rechecktime ) ;
                    if( OOneFourth )
                        {
                            if ( ( GFull ) | ( GOneThird ) | ( GHalf ) )            // checking lower tank for sufficient water
                                {
                                    delay ( rechecktime ) ;
                                    if ( ( GFull ) | ( GOneThird ) | ( GHalf ) )            // checking lower tank for sufficient water
                                        {
                                            OClearScreen( )  ;
                                            Printfunction( "Upper : 1/4" ) ;
                                            delay ( delaynumber ) ;

                                            OClearScreen( )  ;
                                            Printfunction( "Upper : filling" ) ;
                                            delay( delaynumber ) ;

                                            LoadOn ( overhead )  ;
                                        }
                                }

                            else if ( ( GOneFourth ) | ( GEmpty ) )              // conditions when not to run upper motor
                                {
                                    delay ( rechecktime ) ;
                                    if ( ( GOneFourth ) | ( GEmpty ) )              // conditions when not to run upper motor
                                        {
                                            OClearScreen( )  ;
                                            Load &= OMotor_Off ;
                                            Printfunction( "Upper : cant run" ) ;
                                            delay ( delaynumber ) ;
                                            OClearScreen( )  ;
                                            Printfunction( "Lower tank low" ) ;
                                            LoadOn( ground ) ;
                                        }
                                }
                        }
                }

            if( GOneFourth )
                {
                    delay ( rechecktime ) ;
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
                }

//-----------------------------------------------------------------  faulty conditions

            if  ( ( OError ) || ( GError ) )
                {
                    delay( rechecktime ) ;
                    if  ( ( OError ) || ( GError ) )
                        {
                            Commandfunction( 0x01 ) ;
                            Printfunction ( " Error !!" ) ;

                            Load &= GMotor_Off ;
                            Load &= OMotor_Off ;

                            while ( ( OError ) || ( GError ) )
                                {
                                    buzzer() ;
                                }
                        }
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
            delay ( 150 ) ;
            Load &= Speaker_Off ;

        }
}
#endif // LIBRARY_C_INCLUDED
