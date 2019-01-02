/**
 * @file LOT_spi0.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#ifndef _LOT_SPI0_H_
#define _LOT_SPI0_H_

#include <avr/io.h>

#if !defined( __AVR_ATmega328P__ )
#    warning "Untested device"
#endif

#define LOT_SPI0_TIME_OUT 255

#ifndef _LOT_STATUS_TYPEDEF_
#    define _LOT_STATUS_TYPEDEF_
typedef enum
{
    LOT_OK = 0x00,
    LOT_ERROR,
    LOT_BUSY,
    LOT_TIMEOUT
} LOT_status_typedef;
#endif

typedef enum
{
    LOT_SPI_SCK_DIV_2 = 0x00,
    LOT_SPI_SCK_DIV_4,
    LOT_SPI_SCK_DIV_8,
    LOT_SPI_SCK_DIV_16,
    LOT_SPI_SCK_DIV_32,
    LOT_SPI_SCK_DIV_64,
    LOT_SPI_SCK_DIV_128
} LOT_spi_clk_divider_typedef;

typedef enum
{
    LOT_SPI_MSB_FIRST = 0x00,
    LOT_SPI_LSB_FIRST
} LOT_spi_data_order_typedef;

typedef enum
{
    LOT_SPI_CLK_IDLE_LOW = 0x00,
    LOT_SPI_CLK_IDLE_HIGH
} LOT_spi_clk_idle_state_typedef;

typedef enum
{
    LOT_SPI_CLK_SAMPLING_1_EDGE = 0x00,
    LOT_SPI_CLK_SAMPLING_2_EDGE
} LOT_spi_clk_sampling_edge_typedef;

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

class LOT_spi0 {
public:
    LOT_spi0( volatile uint8_t &_spcr, volatile uint8_t &_spsr, volatile uint8_t &_spdr );
    LOT_spi0( const uint8_t spi_number );

    void        setup( LOT_spi_data_order_typedef        data_order,
                       LOT_spi_clk_idle_state_typedef    clk_idle,
                       LOT_spi_clk_sampling_edge_typedef clk_sampling_edge,
                       LOT_spi_clk_divider_typedef       clk_divider );
    inline void setup( LOT_spi_clk_divider_typedef clk_divider )
    {
        setup( LOT_SPI_MSB_FIRST, LOT_SPI_CLK_IDLE_LOW, LOT_SPI_CLK_SAMPLING_1_EDGE, clk_divider );
    }

    LOT_status_typedef transmit( const uint8_t data );
    LOT_status_typedef
                       transmit( const uint8_t register_address, const uint8_t *data, uint8_t size );
    LOT_status_typedef transmit( const uint8_t register_address, const uint8_t data );

    LOT_status_typedef receive( const uint8_t register_address, uint8_t *data, uint8_t size );
    uint8_t            receive( const uint8_t register_address );

    LOT_status_typedef transceive( uint8_t *data, uint8_t size );
    uint8_t            transceive( const uint8_t data );

protected:
private:
    uint8_t           error_count;
    volatile uint8_t &spcr;
    volatile uint8_t &spsr;
    volatile uint8_t &spdr;

    /**
     * @brief 통신 에러가 발생한 경우 처리하는 함수
     */
    void __attribute__( ( weak ) ) error( void );
};

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
    return SPDR;
}

extern LOT_spi0 spi0;
#endif    // _LOT_SPI0_H_
