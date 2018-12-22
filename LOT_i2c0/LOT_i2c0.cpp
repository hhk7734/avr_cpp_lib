/**
 * @file LOT_i2c0.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief i2c 마스터 통신
 */

#include "LOT_i2c0.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t &twbr { TWBR };
volatile uint8_t &twsr { TWSR };
volatile uint8_t &twdr { TWDR };
volatile uint8_t &twcr { TWCR };

/// TWSR
const uint8_t tws_mask { 0xF8 };
const uint8_t twps1 { _BV( TWPS1 ) };
const uint8_t twps0 { _BV( TWPS0 ) };

/// TWCR
const uint8_t twint { _BV( TWINT ) };
const uint8_t twea { _BV( TWEA ) };
const uint8_t twsta { _BV( TWSTA ) };
const uint8_t twsto { _BV( TWSTO ) };
const uint8_t twwc { _BV( TWWC ) };
const uint8_t twen { _BV( TWEN ) };
const uint8_t twie { _BV( TWIE ) };

void LOT_i2c0::twi_isr( void ) {}

void LOT_i2c0::setup( const uint32_t scl_clock ) {}

void LOT_i2c0::transmit( const uint8_t device_address, const uint8_t *data, uint8_t size ) {}
void LOT_i2c0::transmit( const uint8_t device_address,
                         const uint8_t register_address,
                         uint8_t       data )
{
}
uint8_t LOT_i2c0::receive( const uint8_t device_address, uint8_t *data, uint8_t max_size ) {}
uint8_t LOT_i2c0::receive( const uint8_t device_address,
                           const uint8_t register_address,
                           uint8_t *     data,
                           uint8_t       max_size )
{
}
uint8_t LOT_i2c0::receive( const uint8_t device_address, const uint8_t register_address ) {}

void LOT_i2c0::transmit_slow( const uint8_t device_address, const uint8_t *data, uint8_t size ) {}
void LOT_i2c0::transmit_slow( const uint8_t device_address,
                              const uint8_t register_address,
                              uint8_t       data )
{
}
uint8_t LOT_i2c0::receive_slow( const uint8_t device_address, uint8_t *data, uint8_t max_size ) {}
uint8_t LOT_i2c0::receive_slow( const uint8_t device_address,
                                const uint8_t register_address,
                                uint8_t *     data,
                                uint8_t       max_size )
{
}
uint8_t LOT_i2c0::receive_slow( const uint8_t device_address, const uint8_t register_address ) {}

LOT_i2c0 i2c0;

ISR( TWI_vect ) { i2c0.twi_isr(); }