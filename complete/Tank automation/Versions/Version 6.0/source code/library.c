#ifndef LIBRARY_C_INCLUDED
#define LIBRARY_C_INCLUDED
#include "header.h"


///  ____________________________________________________________________________________________ INITIALIZATION


void Initialization( )
{

    LcdDirecton         = 0xFF ;                                                            // as output

    /***/
    Data                = 0x00 ;                                                            // making low all data pins of lcd

    SensorDirection     = 0x00 ;                                                            // as input
    LoadDirection       = 0xFF ;                                                            // motor and speaker as output
    Load                = off  ;

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
            Out= 0xFF ;                                                                     // tanks empty by default
            /****/Data = 0x00 ;
            delay( 1000 ) ;                                                                 // to stablize line


//----------------------------------------------------------------  Full conditions


            if ( OFull )                                                                    // overhead-tank full condition
                {
                    delay ( rechecktime ) ;
                    if ( OFull )                                                            // overhead-tank full condition
                        {
                            OClearScreen() ;
                            Printfunction ( "100% Full" ) ;
                            buzzer() ;
                            LoadOff( overhead ) ;
                        }
                }

            if ( GFull )                                                                    // ground-tank full condition
                {
                    delay ( rechecktime ) ;
                    if ( GFull )                                                            // ground-tank full condition
                        {
                            GClearScreen() ;
                            Printfunction ( "100% Full" ) ;
                            buzzer() ;
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
                            Printfunction ( "Less than 100%" ) ;
                            delay ( delaynumber ) ;
                        }
                }

            if( GOneThird )                                                                 // ground-tank onethird condition
                {
                    delay ( rechecktime ) ;
                    if( GOneThird )                                                         // ground-tank onethird condition
                        {
                            GClearScreen( )  ;
                            Printfunction ( "Less than 100% " ) ;
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
                            Printfunction ( "Less than 75%" ) ;
                            delay ( delaynumber ) ;
                        }
                }

            if ( GHalf )                                                                    // ground-tank half condition
                {
                    delay ( rechecktime ) ;
                    if ( GHalf )                                                            // ground-tank half condition
                        {
                            GClearScreen( )  ;
                            Printfunction ( "Less than 75%" ) ;
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
                            Printfunction( "Less than 50%" ) ;
                            delay ( delaynumber ) ;
                        }
                }

            if( GOneFourth )
                {
                    delay ( rechecktime ) ;
                    if( GOneFourth )
                        {
                            GClearScreen() ;
                            Printfunction( "Less than 50%" ) ;
                            delay ( delaynumber ) ;
                        }
                }


//----------------------------------------------------------------  Empty conditions


            if ( OEmpty )                                                                   // overhead-tank empty condition
                {
                    delay ( rechecktime ) ;
                    if ( OEmpty )                                                           // overhead-tank empty condition
                        {
                            if ( GFull || GOneThird || GHalf || GOneFourth )                // checking lower tank for sufficient water
                                {
                                    OClearScreen( )  ;
                                    Printfunction( "Less than 25%" ) ;
                                    delay ( delaynumber ) ;

                                    OClearScreen( )  ;
                                    Printfunction( "Filling tank" ) ;
                                    delay( delaynumber ) ;

                                    LoadOn ( overhead )  ;
                                }

                            else if ( GEmpty )                                              // conditions when not to run upper motor
                                {
                                    delay ( rechecktime ) ;
                                    if ( GEmpty )                                           // conditions when not to run upper motor
                                        {
                                            Load &= OMotor_Off ;

                                            OClearScreen( )  ;
                                            Printfunction( "Upper : cant run" ) ;
                                            delay ( delaynumber ) ;
                                            OClearScreen( )  ;
                                            Printfunction( "Lower tank low" ) ;

                                            unsigned char check = LineChecking( )  ;

                                            if      ( check == on ) { LoadOn( ground ) ; }
                                            else if ( check == off )
                                                {
                                                    Load &= GMotor_Off ;
                                                    GClearScreen() ;
                                                    Printfunction( "No Line water" ) ;
                                                }
                                        }
                                }
                        }
                }

            if ( GEmpty )                                                                   // ground-tank empty condition
                {
                    delay ( rechecktime ) ;
                    if ( GEmpty )                                                           // ground-tank empty condition
                        {
                            GClearScreen( )  ;
                            Printfunction( "Less than 25%"  ) ;
                            delay ( delaynumber ) ;
                            GClearScreen( )  ;
                            Printfunction( "Filling tank" ) ;

                            unsigned char check = LineChecking( )  ;

                            if      ( check == on ) { LoadOn( ground ) ; }
                            else if ( check == off )
                                {
                                    Load &= GMotor_Off ;
                                    GClearScreen() ;
                                    Printfunction( "No Line water" ) ;

                                }
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
            Load |= OMotor_On ;
            buzzer() ;
        }

    else if ( number == ground )
        {
            Load |= GMotor_On ;
            buzzer() ;
        }
}

void LoadOff ( unsigned char number )
{

    if ( number == overhead )
        {
            delay ( 1000 ) ;
            if  ( OFull )
                {
                    Load &= OMotor_Off ;
                    OClearScreen() ;
                    Printfunction( "Upper motor off" ) ;
                }
        }
    else if ( number == ground )
        {
            delay ( 1000 ) ;
            if  ( GFull )
                {
                    Load &= GMotor_Off ;
                    GClearScreen() ;
                    Printfunction( "Lower motor off" ) ;
                }
        }
}

///  ____________________________________________________________________________________________ LCD COMMANDS


void Commandfunction ( unsigned char a )
{

    Data = ( a&0xF0 ) | ( Data&0x0F ) ;                                                     // masking lower nibble

    Command &= ~( 1<<RS ) ;

    Command |= ( 1<<E ) ;
    delay ( 2 ) ;
    Command &= ~( 1<<E ) ;

    Data = ( a<<4 ) | ( Data&0x0F ) ;                                                       // masking higher nibble

    Command |= ( 1<<E ) ;
    delay ( 2 ) ;
    Command &= ~( 1<<E ) ;

    delay ( 7 ) ;

}


///  ____________________________________________________________________________________________ LCD DATA


void Datafunction ( unsigned char a )
{

    Data = ( a&0xF0 ) | ( Data&0x0F ) ;                                                     // masking lower nibble

    Command |= ( 1<<RS ) ;

    Command |= ( 1<<E ) ;
    delay ( 2 ) ;
    Command &= ~( 1<<E ) ;

    Data = ( a<<4 ) | ( Data&0x0F ) ;                                                       // masking higher nibble

    Command |= ( 1<<E ) ;
    delay ( 2 ) ;
    Command &= ~( 1<<E ) ;

    delay ( 7 ) ;

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
    for ( ; loop<5 ; loop++ )                                                               // making speaker buzz 5 times
        {

            Load |= Speaker_On  ;
            delay ( 150 ) ;
            Load &= Speaker_Off ;

        }
}

unsigned char LineChecking ( )
{
    unsigned char loop = 0 ;
    for ( ; loop<1 ; loop++ )                                                               // running ground motor for 1 minute
        {
            LoadOn( ground ) ;
        }
    if      ( LineOn )  { return on ; }
    return off ;                                                                            // else if ( LineOff ) { return off ; }
}

#endif                                                                                      // LIBRARY_C_INCLUDED
