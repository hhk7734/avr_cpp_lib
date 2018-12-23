/**
 * @file LOT_transmit.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 문자열 송신
 */

#include "LOT_transmit.h"

#include <math.h>

void LOT_transmit::transmit( const char data[] )
{
    while( *data ) { transmit( *data++ ); }
}

void LOT_transmit::transmit( uint16_t data )
{
    uint16_t temp;
    char     buf[6];
    char *   str = &buf[6 - 1];
    *str         = '\0';
    do
    {
        temp   = data / 10;
        *--str = ( data - ( temp * 10 ) ) + '0';
        data   = temp;
    } while( data != 0 );
    transmit( str );
}

void LOT_transmit::transmit( int16_t data )
{
    if( data < 0 )
    {
        transmit( '-' );
        data = -data;
    }
    transmit( static_cast<uint16_t>( data ) );
}

void LOT_transmit::transmit( uint32_t data )
{
    uint32_t temp;
    char     buf[11];
    char *   str = &buf[11 - 1];
    *str         = '\0';
    do
    {
        temp   = data / 10;
        *--str = ( data - ( temp * 10 ) ) + '0';
        data   = temp;
    } while( data != 0 );
    transmit( str );
}

void LOT_transmit::transmit( int32_t data )
{
    if( data < 0 )
    {
        transmit( '-' );
        data = -data;
    }
    transmit( static_cast<uint32_t>( data ) );
}

void LOT_transmit::transmit( uint64_t data )
{
    uint64_t temp;
    char     buf[21];
    char *   str = &buf[21 - 1];
    *str         = '\0';
    do
    {
        temp   = data / 10;
        *--str = ( data - ( temp * 10 ) ) + '0';
        data   = temp;
    } while( data != 0 );
    transmit( str );
}

void LOT_transmit::transmit( int64_t data )
{
    if( data < 0 )
    {
        transmit( '-' );
        data = -data;
    }
    transmit( static_cast<uint64_t>( data ) );
}

void LOT_transmit::transmit( double data, uint8_t digits )
{
    if( ( data > 4294967000.0 ) || ( data < -4294967000.0 ) ) { return transmit( "ovf" ); }
    else if( isinf( data ) )
    {
        return transmit( "inf" );
    }
    else if( isnan( data ) )
    {
        return transmit( "nan" );
    }

    if( data < 0.0 )
    {
        transmit( '-' );
        data = -data;
    }

    double round_ = 0.5;
    for( uint8_t i = 0; i < digits; ++i ) { round_ /= 10.0; }
    data += round_;

    uint32_t _int = static_cast<uint32_t>( data );
    transmit( _int );
    transmit( '.' );

    double _dec = data - static_cast<double>( _int );
    while( digits != 0 )
    {
        --digits;
        _dec *= 10.0;
        uint8_t temp = static_cast<uint8_t>( _dec );
        transmit( static_cast<uint8_t>( temp + '0' ) );
        _dec -= static_cast<double>( temp );
    }
}