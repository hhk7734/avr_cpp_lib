/**
 * @file LOT_i2c.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief AVR I2C 마스터 통신
 */

#include <LOT_i2c.h>

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

void LOT_i2c::setup( const uint32_t scl_clock )
{
    twsr = 0;
    twbr = ( ( F_CPU / scl_clock ) - 16 ) / 2;
}

LOT_status_typedef
    LOT_i2c::transmit( const uint8_t slave_address, const uint8_t *data, uint8_t size )
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

LOT_status_typedef LOT_i2c::transmit( const uint8_t  slave_address,
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

LOT_status_typedef LOT_i2c::receive( const uint8_t slave_address, uint8_t *data, uint8_t size )
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

LOT_status_typedef LOT_i2c::receive( const uint8_t slave_address,
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

LOT_status_typedef __attribute__( ( noinline ) ) LOT_i2c::control( const uint8_t _twcr )
{
    twcr = _twcr;
#if LOT_I2C_TIME_OUT <= 0xFF
    uint8_t count = LOT_I2C_TIME_OUT;
#else
    uint16_t count = LOT_I2C_TIME_OUT;
#endif
    while( _is_bit_clear( twcr, LOT_TWINT ) )
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
        case TW_MT_ARB_LOST: control( _BV( LOT_TWINT ) | _BV( LOT_TWEA ) | _BV( LOT_TWEN ) ); break;
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

#if defined( _USE_LOT_I2C_0_ )

#    if !defined( __AVR_ATmega328P__ )
#        warning "Untested device"
#    endif

LOT_i2c i2c0( TWBR, TWSR, TWDR, TWCR );

#endif    /// _USE_LOT_I2C_0_