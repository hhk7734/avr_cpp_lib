/**
 * @file LOT_i2c0.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief I2C 마스터 통신
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

LOT_i2c0::LOT_i2c0()
    : error_state( 0xFF )
    , error_count( 0 )
{
}

inline LOT_status_typedef LOT_i2c0::SLA_W( const uint8_t slave_address )
{
    control( LOT_TWINT | LOT_TWSTA | LOT_TWEN );
    twdr = slave_address << 1;
    return control( LOT_TWINT | LOT_TWEA | LOT_TWEN );
}

inline LOT_status_typedef LOT_i2c0::SLA_R( const uint8_t slave_address )
{
    control( LOT_TWINT | LOT_TWSTA | LOT_TWEN );
    twdr = ( slave_address << 1 ) | 0x01;
    return control( LOT_TWINT | LOT_TWEA | LOT_TWEN );
}

inline LOT_status_typedef LOT_i2c0::transmit_data( const uint8_t data )
{
    twdr = data;
    return control( LOT_TWINT | LOT_TWEA | LOT_TWEN );
}
inline uint8_t LOT_i2c0::receive_data( void )
{
    control( LOT_TWINT | LOT_TWEA | LOT_TWEN );
    return twdr;
}
inline uint8_t LOT_i2c0::receive_last_data( void )
{
    control( LOT_TWINT | LOT_TWEN );
    return twdr;
}

inline void LOT_i2c0::stop( void )
{
    twcr = LOT_TWINT | LOT_TWSTO | LOT_TWEN;
    // while( (twcr & LOT_TWSTO) !=0 ) {}
}

void LOT_i2c0::setup( const uint32_t scl_clock )
{
    twsr = 0;
    twbr = ( ( F_CPU / scl_clock ) - 16 ) / 2;
}

LOT_status_typedef
    LOT_i2c0::transmit( const uint8_t slave_address, const uint8_t *data, uint8_t size )
{
    if( SLA_W( slave_address ) == LOT_OK )
    {
        for( uint8_t i = 0; i < size; ++i ) { transmit_data( data[i] ); }
        stop();
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

LOT_status_typedef LOT_i2c0::transmit( const uint8_t  slave_address,
                                       const uint8_t  register_address,
                                       const uint8_t *data,
                                       uint8_t        size )
{
    if( SLA_W( slave_address ) == LOT_OK )
    {
        transmit_data( register_address );
        for( uint8_t i = 0; i < size; ++i ) { transmit_data( data[i] ); }
        stop();
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

LOT_status_typedef LOT_i2c0::receive( const uint8_t slave_address, uint8_t *data, uint8_t size )
{
    if( SLA_R( slave_address ) == LOT_OK )
    {
        for( uint8_t i = 0; i < size - 1; ++i ) { data[i] = receive_data(); }
        data[size - 1] = receive_last_data();
        stop();
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

LOT_status_typedef LOT_i2c0::receive( const uint8_t slave_address,
                                      const uint8_t register_address,
                                      uint8_t *     data,
                                      uint8_t       size )
{
    if( SLA_W( slave_address ) == LOT_OK )
    {
        transmit_data( register_address );
        SLA_R( slave_address );
        for( uint8_t i = 0; i < size - 1; ++i ) { data[i] = receive_data(); }
        data[size - 1] = receive_last_data();
        stop();
        return LOT_OK;
    }
    else
    {
        error();
        return LOT_ERROR;
    }
}

LOT_status_typedef __attribute__( ( noinline ) ) LOT_i2c0::control( const uint8_t _twcr )
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
    switch( error_state )
    {
        case 0xFF: break;
        case TW_MT_ARB_LOST: control( LOT_TWINT | LOT_TWEA | LOT_TWEN ); break;
        case TW_MT_SLA_NACK:
        case TW_MT_DATA_NACK:
        case TW_MR_SLA_NACK:
        case TW_BUS_ERROR: stop(); break;
        default: break;
    }
    error_state = 0xFF;
    ++error_count;
    if( error_count > 10 )
    {
        for( ;; ) {}
    }
}

LOT_i2c0 i2c0;