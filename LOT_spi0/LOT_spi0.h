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

    void setup( LOT_spi_clk_divider_typedef       clk_divider,
                LOT_spi_data_order_typedef        data_order,
                LOT_spi_clk_idle_state_typedef    clk_idle,
                LOT_spi_clk_sampling_edge_typedef clk_sampling_edge );

protected:
private:
};

#endif    // _LOT_SPI0_H_
