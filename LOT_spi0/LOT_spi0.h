/**
 * @file LOT_spi0.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#ifndef _LOT_SPI0_H_
#define _LOT_SPI0_H_

#include <inttypes.h>

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
    uint8_t error_count;

    /**
     * @brief 통신 에러가 발생한 경우 처리하는 함수
     */
    void __attribute__( ( weak ) ) error( void );
};

extern LOT_spi0 spi0;
#endif    // _LOT_SPI0_H_
