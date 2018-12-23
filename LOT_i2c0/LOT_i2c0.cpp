/**
 * @file LOT_i2c0.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#include "LOT_i2c0.h"

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

volatile uint8_t &twbr { TWBR };
volatile uint8_t &twsr { TWSR };
volatile uint8_t &twdr { TWDR };
volatile uint8_t &twcr { TWCR };

/// TWSR
const uint8_t LOT_TWS_MASK { 0xF8 };
const uint8_t LOT_TWPS1 { _BV( TWPS1 ) };
const uint8_t LOT_TWPS0 { _BV( TWPS0 ) };

/// TWCR
/// interrupt flag
const uint8_t LOT_TWINT { _BV( TWINT ) };
/// ACK
const uint8_t LOT_TWEA { _BV( TWEA ) };
/// start
const uint8_t LOT_TWSTA { _BV( TWSTA ) };
/// stop
const uint8_t LOT_TWSTO { _BV( TWSTO ) };
/// wirte collision
const uint8_t LOT_TWWC { _BV( TWWC ) };
/// i2c enable
const uint8_t LOT_TWEN { _BV( TWEN ) };
/// interrupt enable
const uint8_t LOT_TWIE { _BV( TWIE ) };

LOT_i2c0::LOT_i2c0() {}

void LOT_i2c0::setup( const uint32_t scl_clock )
{
    twsr = 0;
    twbr = ( ( F_CPU / scl_clock ) - 16 ) / 2;
}

LOT_status_typedef
    LOT_i2c0::transmit( const uint8_t slave_address, const uint8_t *data, uint8_t size )
{
    /// START
    control( LOT_TWINT | LOT_TWSTA | LOT_TWEN );
    twdr = slave_address << 1;
    /// ACK or transmit
    if( control( LOT_TWINT | LOT_TWEA | LOT_TWEN ) == LOT_OK )
    {
        for( uint8_t i = 0; i < size; ++i )
        {
            twdr = data[i];
            control( LOT_TWINT | LOT_TWEA | LOT_TWEN );
        }
    }
    else
    {
        error();
    }
}

LOT_status_typedef LOT_i2c0::transmit( const uint8_t slave_address,
                                       const uint8_t register_address,
                                       uint8_t *     data,
                                       uint8_t       size )
{
}

LOT_status_typedef LOT_i2c0::control( const uint8_t &_twcr ) __attribute__( ( noinline ) )
{
    twcr = _twcr;
#if LOT_I2C0_TIME_OUT <= 0xFF
    uint8_t count = LOT_I2C0_TIME_OUT;
#else
    uint16_t count = LOT_I2C0_TIME_OUT;
#endif
    while( ( twcr & LOT_TWINT ) == 0 )
    {
        --count;
        if( count == 0 )
        {
            error_state = twsr & LOT_TWS_MASK;
            twcr        = 0;
            return LOT_ERROR;
        }
    }
    return LOT_OK;
}

void LOT_i2c0::error( void )
{
    for( ;; ) {}
}

inline void nack( void ) { twcr = twint | twen; }
inline void stop( void )
{
    twcr = twint | twsto | twen;
    // while( ( twcr & twsto ) != 0 ) {}
}
