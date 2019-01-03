#include <avr/io.h>

volatile uint8_t &LOT_spcr0 { SPCR };
volatile uint8_t &LOT_spsr0 { SPSR };
volatile uint8_t &LOT_spdr0 { SPDR };

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