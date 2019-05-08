/**
 * @file LOT_i2c_inline.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 */

inline __attribute__( ( always_inline ) ) LOT_status_typedef LOT_i2c::transmit(
    const uint8_t slave_address, const uint8_t register_address, const uint8_t data )
{
    return transmit( slave_address, register_address, &data, 1 );
}

inline __attribute__( ( always_inline ) ) uint8_t LOT_i2c::receive( const uint8_t slave_address,
                                                                    const uint8_t register_address )
{
    uint8_t temp;
    receive( slave_address, register_address, &temp, 1 );
    return temp;
}

inline __attribute__( ( always_inline ) ) void LOT_i2c::setup( void )
{
    setup( LOT_I2C0_DEFAULT_CLOCK );
}

inline __attribute__( ( always_inline ) ) LOT_status_typedef
    LOT_i2c::SLA_W( const uint8_t slave_address )
{
    control( _BV( LOT_TWINT ) | _BV( LOT_TWSTA ) | _BV( LOT_TWEN ) );
    twdr = slave_address << 1;
    return control( _BV( LOT_TWINT ) | _BV( LOT_TWEA ) | _BV( LOT_TWEN ) );
}

inline __attribute__( ( always_inline ) ) LOT_status_typedef
    LOT_i2c::SLA_R( const uint8_t slave_address )
{
    control( _BV( LOT_TWINT ) | _BV( LOT_TWSTA ) | _BV( LOT_TWEN ) );
    twdr = ( slave_address << 1 ) | 0x01;
    return control( _BV( LOT_TWINT ) | _BV( LOT_TWEA ) | _BV( LOT_TWEN ) );
}

inline __attribute__( ( always_inline ) ) LOT_status_typedef
    LOT_i2c::transmit_data( const uint8_t data )
{
    twdr = data;
    return control( _BV( LOT_TWINT ) | _BV( LOT_TWEA ) | _BV( LOT_TWEN ) );
}
inline __attribute__( ( always_inline ) ) uint8_t LOT_i2c::receive_data( void )
{
    control( _BV( LOT_TWINT ) | _BV( LOT_TWEA ) | _BV( LOT_TWEN ) );
    return twdr;
}
inline __attribute__( ( always_inline ) ) uint8_t LOT_i2c::receive_last_data( void )
{
    control( _BV( LOT_TWINT ) | _BV( LOT_TWEN ) );
    return twdr;
}

inline __attribute__( ( always_inline ) ) void LOT_i2c::stop( void )
{
    twcr = _BV( LOT_TWINT ) | _BV( LOT_TWSTO ) | _BV( LOT_TWEN );
    // while( (twcr & _BV(LOT_TWSTO)) !=0 ) {}
}