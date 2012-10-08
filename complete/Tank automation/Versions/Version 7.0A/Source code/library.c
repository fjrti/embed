#ifndef LIBRARY_C_INCLUDED
#define LIBRARY_C_INCLUDED
#include "header.h"




///  ____________________________________________________________________________________________ INITIALIZATION

void Initialization( )
{

    LcdDirecton         = 0xFF ;                                                            // as output
    SensorDirection     = 0x00 ;                                                            // as input
    LoadDirection       = 0xFF ;                                                            // motor and speaker as output
    MiscDirection       = 0x00 ;
    MiscOut             = ( ( on<<Line )|( on<<ByPass ) ) ;                                 // not enabled by default

    Commandfunction ( 0x33 )    ;                                                           // for 4 bit lcd
    Commandfunction ( 0x32 )    ;                                                           // for 4 bit lcd
    Commandfunction ( 0x28 )    ;                                                           // for 4 bit lcd
    Commandfunction ( 0x0E )    ;                                                           // cursor on
    Commandfunction ( 0x06 )    ;
    Commandfunction ( 0x01 )    ;                                                           // clear

}




///  ____________________________________________________________________________________________ OVERHEAD SCREEN CLEAR

void OClearScreen ( )
{

    Commandfunction ( 0x80 ) ;                                                              // selecting just upper portion of lcd
    Printfunction ( "                   " ) ;                                               // gives effect of clear upper lcd row
    Commandfunction ( 0x80 ) ;

}




///  ____________________________________________________________________________________________ GROUND SCREEN CLEAR

void GClearScreen ( )
{

    Commandfunction ( 0xC0 ) ;                                                              // selecting just lower portion of lcd
    Printfunction ( "                       " ) ;                                           // gives effect of clear lower lcd row
    Commandfunction ( 0xC0 ) ;

}




///  ____________________________________________________________________________________________ OPERATIONS

void Operation ( )
{
    for ( ;; )
        {
            Out = 0xFF ;                                                                      // tanks empty by default
            delay( 5 ) ;                                                                      // to stablize line


// ---------------------------------------------------------------- bypass condition
            if ( clr( MiscIn , ByPass ) )
                {
                    delay ( rechecktime ) ;
                    if ( clr( MiscIn , ByPass ) )
                        {
                            LoadOn( ground ) ;
                            LoadOn( overhead ) ;

                            Commandfunction( 0x01 ) ;
                            Printfunction ( "SYSTEM DISABLED" ) ;

                            while ( clr( MiscIn , ByPass ) ) ;
                            delay ( rechecktime ) ;
                            while ( clr( MiscIn , ByPass ) ) ;

                            LoadOff( ground ) ;
                            LoadOff( overhead ) ;

                            Commandfunction ( 0x01 ) ;
                        }
                }


//----------------------------------------------------------------  Full conditions
            else
                {
                    if ( OFull )                                                                    // overhead-tank full condition
                        {
                            delay ( rechecktime ) ;
                            if ( OFull )                                                            // overhead-tank full condition
                                {
                                    OClearScreen() ;
                                    Printfunction ( "Upper : Full" ) ;
                                    delay( delaynumber ) ;
                                    LoadOff( overhead ) ;
                                }
                        }

                    if ( GFull )                                                                    // ground-tank full condition
                        {
                            delay ( rechecktime ) ;
                            if ( GFull )                                                            // ground-tank full condition
                                {
                                    GClearScreen() ;
                                    Printfunction ( "Lower : Full" ) ;
                                    delay( delaynumber ) ;
                                    LoadOff( ground ) ;
                                }
                        }


//----------------------------------------------------------------  One-third conditions
                    if( OOneThird )                                                                 // overhead-tank onethird condition
                        {
                            delay ( rechecktime ) ;
                            if( OOneThird )                                                         // overhead-tank onethird condition
                                {
                                    OClearScreen( )  ;
                                    Printfunction ( "Upper : Medium" ) ;
                                    delay ( delaynumber ) ;
                                }
                        }

                    if( GOneThird )                                                                 // ground-tank onethird condition
                        {
                            delay ( rechecktime ) ;
                            if( GOneThird )                                                         // ground-tank onethird condition
                                {
                                    GClearScreen( )  ;
                                    Printfunction ( "Lower : Medium" ) ;
                                    delay ( delaynumber ) ;
                                }
                        }


//----------------------------------------------------------------  Half conditions
                    if ( OHalf )                                                                    // overhead-tank half condition
                        {
                            delay ( rechecktime ) ;
                            if ( OHalf )                                                            // overhead-tank half condition
                                {
                                    OClearScreen( )  ;
                                    Printfunction ( "Upper : Half" ) ;
                                    delay ( delaynumber ) ;
                                }
                        }

                    if ( GHalf )                                                                    // ground-tank half condition
                        {
                            delay ( rechecktime ) ;
                            if ( GHalf )                                                            // ground-tank half condition
                                {
                                    GClearScreen( )  ;
                                    Printfunction ( "Lower : Half" ) ;
                                    delay ( delaynumber ) ;
                                }
                        }


//----------------------------------------------------------------  One forth conditions
                    if( OOneFourth )
                        {
                            delay ( rechecktime ) ;
                            if( OOneFourth )
                                {
                                    OClearScreen() ;
                                    Printfunction( "Upper : Low" ) ;
                                    delay ( delaynumber ) ;
                                }
                        }

                    if( GOneFourth )
                        {
                            delay ( rechecktime ) ;
                            if( GOneFourth )
                                {
                                    GClearScreen() ;
                                    Printfunction( "Lower : Low" ) ;
                                    delay ( delaynumber ) ;
                                }
                        }


//----------------------------------------------------------------  Empty conditions
                    if ( OEmpty )                                                                   // overhead-tank empty condition
                        {
                            delay ( rechecktime ) ;
                            if ( OEmpty )                                                           // overhead-tank empty condition
                                {
                                    OClearScreen( )  ;
                                    Printfunction( "Upper : Very Low" ) ;
                                    delay ( delaynumber ) ;
                                    OClearScreen( ) ;
                                    if ( GFull || GOneThird || GHalf || GOneFourth )                // checking lower tank for sufficient water
                                        {
                                            LoadOn( overhead ) ;
                                            OClearScreen ( ) ;
                                            Printfunction( "Filling tank" ) ;
                                        }
                                }
                        }

                    if ( GEmpty )                                                                   // ground-tank empty condition
                        {
                            delay ( rechecktime ) ;
                            if ( GEmpty )                                                           // ground-tank empty condition
                                {
                                    LoadOff(overhead) ;
                                    GClearScreen( )  ;
                                    Printfunction( "lower : Very Low"  ) ;
                                    //buzzer ( ) ;
                                }
                        }


//-----------------------------------------------------------------  faulty conditions
                    if  ( Error )
                        {
                            delay( rechecktime ) ;

                            if  ( Error )
                                {
                                    Commandfunction( 0x01 ) ;
                                    Printfunction ( "   SOME ERROR" ) ;

                                    LoadOff ( ground ) ;
                                    LoadOff ( overhead ) ;

                                    while ( Error ) ;
                                }
                        }
                }
        }
}




/// ____________________________________________________________________________________________ RUNNING THE LOAD

void LoadOn ( unsigned char number )
{
    if        ( number == overhead )    { Load |= OMotor_On ; }
    else if   ( number == ground )      { Load |= GMotor_On ; }
}

void LoadOff ( unsigned char number )
{
    if      ( number == overhead )      { Load &= OMotor_Off ; }
    else if ( number == ground )        { Load &= GMotor_Off ; }
}




///  ____________________________________________________________________________________________ LCD COMMANDS

void Commandfunction ( unsigned char a )
{


    Data = ( a&0xF0 ) | ( Data&0x0F ) ;                                                     // masking lower nibble

    Command &= ( ~( 1<<RS ) ) ;

    Command |= ( 1<<E ) ;
    delay( 1 ) ;
    Command &= ~( 1<<E ) ;

    Data = ( a<<4 ) | ( Data&0x0F ) ;                                                       // masking higher nibble

    Command |= ( 1<<E ) ;
    delay( 1 ) ;
    Command &= ~( 1<<E ) ;

    delay( 1 ) ;

}




///  ____________________________________________________________________________________________ LCD DATA

void Datafunction ( unsigned char a )
{

    Data = ( a&0xF0 ) | ( Data&0x0F ) ;                                                     // masking lower nibble

    Command |= ( 1<<RS ) ;

    Command |= ( 1<<E ) ;
    delay( 1 ) ;
    Command &= ~( 1<<E ) ;

    Data = ( a<<4 ) | ( Data&0x0F ) ;                                                       // masking higher nibble

    Command |= ( 1<<E ) ;
    delay( 1 ) ;
    Command &= ~( 1<<E ) ;


    delay( 1 ) ;

}




///  ____________________________________________________________________________________________ LCD PRINTING

void Printfunction  ( char *a )
{
    for ( ; *a !='\0' ; a++ ) { Datafunction ( *a ) ; }
}




///  ____________________________________________________________________________________________ LINE OPERATIONS

unsigned char LineChecking ( )
{
    LoadOn( ground ) ;                                                                      // running ground motor for 1 minute
    GClearScreen() ;
    Printfunction( "Checking line" ) ;
    delay ( 3000 ) ;
    if        ( LineOn ) { return on ; }
    return off ;                                                                            // else if ( LineOff ) { return off ; }
}




///  ____________________________________________________________________________________________ SPEAKER OPERATIONS

void buzzer ( )
{
    unsigned char loop = 0 ;

    for ( ; loop<5 ; loop++ )                                                               // making speaker buzz 5 times
        {
            Load |= Speaker_On  ;
            delay ( delaynumber ) ;
            Load &= Speaker_Off ;
            delay ( delaynumber ) ;
        }
}

#endif                                                                                      // LIBRARY_C_INCLUDED
