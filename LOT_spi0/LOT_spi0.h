/**
 * @file LOT_spi0.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#ifndef _LOT_SPI0_H_
#define _LOT_SPI0_H_

#include "LOT_spi0_pre.h"

#if !defined( __AVR_ATmega328P__ )
#    warning "Untested device"
#endif

#define LOT_SPI0_TIME_OUT 255

#define LOT_SPI0_READ_MASK 0x80

class LOT_spi0 {
public:
    LOT_spi0();

    void        setup( LOT_spi_data_order_typedef        data_order,
                       LOT_spi_clk_idle_state_typedef    clk_idle,
                       LOT_spi_clk_sampling_edge_typedef clk_sampling_edge,
                       LOT_spi_clk_divider_typedef       clk_divider );
    inline void setup( LOT_spi_clk_divider_typedef clk_divider )
    {
        setup( LOT_SPI_MSB_FIRST, LOT_SPI_CLK_IDLE_LOW, LOT_SPI_CLK_SAMPLING_1_EDGE, clk_divider );
    }

    LOT_status_typedef
                       transmit( const uint8_t register_address, const uint8_t *data, uint8_t size );
    LOT_status_typedef transmit( const uint8_t register_address, const uint8_t data );

    LOT_status_typedef receive( const uint8_t register_address, uint8_t *data, uint8_t size );
    uint8_t            receive( const uint8_t register_address );

    LOT_status_typedef transceive( uint8_t *data, uint8_t size );
    uint8_t            transceive( const uint8_t data );

protected:
private:
    uint8_t error_count;

    LOT_status_typedef transmit( const uint8_t data );

    /**
     * @brief 통신 에러가 발생한 경우 처리하는 함수
     */
    void __attribute__( ( weak ) ) error( void );
};

inline LOT_status_typedef LOT_spi0::transmit( const uint8_t data )
{
    LOT_spdr0 = data;
#if LOT_SPI0_TIME_OUT <= 0xFF
    uint8_t count = LOT_SPI0_TIME_OUT;
#else
    uint16_t count = LOT_SPI0_TIME_OUT;
#endif
    while( ( LOT_spsr0 & LOT_SPIF ) == 0 )
    {
        --count;
        if( count == 0 ) { return LOT_ERROR; }
    }
    return LOT_OK;
}

inline uint8_t LOT_spi0::transceive( const uint8_t data )
{
    transmit( data );
    return LOT_spdr0;
}

extern LOT_spi0 spi0;
#endif    // _LOT_SPI0_H_
