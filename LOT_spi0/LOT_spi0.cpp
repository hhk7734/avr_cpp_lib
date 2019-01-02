/**
 * @file LOT_spi0.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#include "LOT_spi0.h"

#include <avr/io.h>

volatile uint8_t &spcr { SPCR };
volatile uint8_t &spsr { SPSR };
volatile uint8_t &spdr { SPDR };

/// SPCR
const uint8_t LOT_SPIE { _BV( SPIE ) };
const uint8_t LOT_SPE { _BV( SPE ) };
const uint8_t LOT_DORD { _BV( DORD ) };
const uint8_t LOT_MSTR { _BV( MSTR ) };
const uint8_t LOT_CPOL { _BV( CPOL ) };
const uint8_t LOT_CPHA { _BV( CPHA ) };
const uint8_t LOT_SPR1 { _BV( SPR1 ) };
const uint8_t LOT_SPR0 { _BV( SPR0 ) };

/// SPSR
const uint8_t LOT_SPIF { _BV( SPIF ) };
const uint8_t LOT_WCOL { _BV( WCOL ) };
const uint8_t LOT_SPI2X { _BV( SPI2X ) };

LOT_spi0::LOT_spi0()
    : error_count( 0 )
{
}

void LOT_spi0::setup( LOT_spi_data_order_typedef        data_order,
                      LOT_spi_clk_idle_state_typedef    clk_idle,
                      LOT_spi_clk_sampling_edge_typedef clk_sampling_edge,
                      LOT_spi_clk_divider_typedef       clk_divider )
{
    /// MOSI output
    /// MISO input
    /// SCK output
#if defined( __AVR_ATmega328P__ )
    DDRB |= _BV( DDB3 );
    DDRB &= ~_BV( DDB4 );
    DDRB |= _BV( DDB5 );
#endif

    spcr = LOT_SPE | LOT_MSTR;

    if( data_order == LOT_SPI_LSB_FIRST ) { spcr |= LOT_DORD; }

    if( clk_idle == LOT_SPI_CLK_IDLE_HIGH ) { spcr |= LOT_CPOL; }

    if( clk_sampling_edge == LOT_SPI_CLK_SAMPLING_2_EDGE ) { spcr |= LOT_CPHA; }

    spsr &= ~LOT_SPI2X;
    switch( clk_divider )
    {
        case LOT_SPI_SCK_DIV_2: spsr |= LOT_SPI2X;
        case LOT_SPI_SCK_DIV_4: break;

        case LOT_SPI_SCK_DIV_8: spsr |= LOT_SPI2X;
        case LOT_SPI_SCK_DIV_16: spcr |= LOT_SPR0; break;

        case LOT_SPI_SCK_DIV_32: spsr |= LOT_SPI2X;
        case LOT_SPI_SCK_DIV_64: spcr |= LOT_SPR1; break;

        case LOT_SPI_SCK_DIV_128: spcr |= LOT_SPR1 | LOT_SPR0; break;
    }
}

LOT_status_typedef
    LOT_spi0::transmit( const uint8_t register_address, const uint8_t *data, uint8_t size )
{
    if( transmit( register_address ) == LOT_OK )
    {
        for( ; size > 0; --size ) { transmit( *data ); }
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

inline LOT_status_typedef LOT_spi0::transmit( const uint8_t data )
{
    spdr = data;
#if LOT_SPI0_TIME_OUT <= 0xFF
    uint8_t count = LOT_SPI0_TIME_OUT;
#else
    uint16_t count = LOT_SPI0_TIME_OUT;
#endif
    while( ( spsr & LOT_SPIF ) == 0 )
    {
        --count;
        if( count == 0 ) { return LOT_ERROR; }
    }
    return LOT_OK;
}

inline uint8_t LOT_spi0::transceive( const uint8_t data )
{
    transmit( data );
    return spdr;
}

LOT_status_typedef LOT_spi0::transmit( const uint8_t register_address, const uint8_t data )
{
    transmit( register_address );
    return transmit( data );
}

LOT_status_typedef LOT_spi0::receive( const uint8_t register_address, uint8_t *data, uint8_t size )
{
    if( transmit( register_address ) == LOT_OK )
    {
        for( ; size > 0; --size )
        {
            transmit( 0 );
            *data++ = spdr;
        }
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

uint8_t LOT_spi0::receive( const uint8_t register_address )
{
    transmit( register_address );
    transmit( 0 );
    return spdr;
}

LOT_status_typedef LOT_spi0::transceive( uint8_t *data, uint8_t size )
{
    if( transmit( *data ) == LOT_OK )
    {
        for( ; size > 1; --size )
        {
            *data++ = spdr;
            transmit( *data );
        }
        *data = spdr;
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

void LOT_spi0::error( void )
{
    ++error_count;
    if( error_count > 10 )
    {
        for( ;; ) {}
    }
}

LOT_spi0 spi0;