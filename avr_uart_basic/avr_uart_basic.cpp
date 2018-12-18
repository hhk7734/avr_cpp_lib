/**
 * @file avr_uart_basic.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief UART 통신 구축
 */

#include "avr_uart_basic.h"

#include <math.h>

LOT_uart_basic::LOT_uart_basic( volatile uint8_t &_ucsra,
                                volatile uint8_t &_ucsrb,
                                volatile uint8_t &_ucsrc,
                                volatile uint8_t &_ubrrl,
                                volatile uint8_t &_ubrrh,
                                volatile uint8_t &_udr )
    : ucsra( _ucsra )
    , ucsrb( _ucsrb )
    , ucsrc( _ucsrc )
    , ubrrl( _ubrrl )
    , ubrrh( _ubrrh )
    , udr( _udr )
    , tx_buf_head( 0 )
    , tx_buf_tail( 0 )
    , rx_buf_head( 0 )
    , rx_buf_tail( 0 )
{
}

void LOT_uart_basic::transmit( const char data[] )
{
    while( *data ) { transmit( *data++ ); }
}

void LOT_uart_basic::transmit( uint16_t data )
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

void LOT_uart_basic::transmit( int16_t data )
{
    if( data < 0 )
    {
        transmit( '-' );
        data = -data;
    }
    transmit( static_cast<uint16_t>( data ) );
}

void LOT_uart_basic::transmit( uint32_t data )
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

void LOT_uart_basic::transmit( int32_t data )
{
    if( data < 0 )
    {
        transmit( '-' );
        data = -data;
    }
    transmit( static_cast<uint32_t>( data ) );
}

void LOT_uart_basic::transmit( uint64_t data )
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

void LOT_uart_basic::transmit( int64_t data )
{
    if( data < 0 )
    {
        transmit( '-' );
        data = -data;
    }
    transmit( static_cast<uint64_t>( data ) );
}

void LOT_uart_basic::transmit( double data, uint8_t digits )
{
    if( isnan( data ) )
    {
        transmit( "nan" );
        return;
    }
    else if( isinf( data ) )
    {
        transmit( "inf" );
        return;
    }
    else if( ( data > 4294967000.0 ) || ( data < -4294967000.0 ) )
    {
        transmit( "ovf" );
        return;
    }

    if( data < 0.0 )
    {
        transmit( '-' );
        data = -data;
    }

    double rounding = 0.5;
    for( uint8_t i = 0; i < digits; ++i ) { rounding /= 10.0; }
    data += rounding;

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