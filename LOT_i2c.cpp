/**
 * @file LOT_i2c.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief AVR I2C 마스터 통신
 */

#include "LOT_i2c.h"

LOT_i2c::LOT_i2c( volatile uint8_t &_twbr,
                  volatile uint8_t &_twsr,
                  volatile uint8_t &_twdr,
                  volatile uint8_t &_twcr )
    : twbr( _twbr )
    , twsr( _twsr )
    , twdr( _twdr )
    , twcr( _twcr )
    , error_state( 0xFF )
    , error_count( 0 )
{
}

LOT_status_typedef __attribute__( ( noinline ) ) LOT_i2c::control( const uint8_t _twcr )
{
    twcr = _twcr;
#if LOT_I2C0_TIME_OUT <= 0xFF
    uint8_t count = LOT_I2C0_TIME_OUT;
#else
    uint16_t count = LOT_I2C0_TIME_OUT;
#endif
    while( _is_bit_clear( twcr & LOT_TWINT ) )
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

void LOT_i2c::error( void )
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
