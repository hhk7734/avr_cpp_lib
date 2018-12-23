/**
 * @file LOT_i2c0_isr.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief i2c 마스터 통신
 */

#include "LOT_i2c0_isr.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/twi.h>

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

enum
{
    I2C_READY,
    I2C_WRITE,
    I2C_READ,
    I2C_WRITE_READ
};

LOT_i2c0::LOT_i2c0()
    : _slave_address_rw( 0 )
    , error_state( 0xFF )
    , state( I2C_READY )
    , buf_index( 0 )
    , buf_length( 0 )
{
}

void LOT_i2c0::twi_isr( void )
{
    switch( twsr & tws_mask )
    {
        /// 시작 또는 재시작 신호 송신
        case TW_START:
        case TW_REP_START:
            /// slave 주소 + R/W 송신
            twdr = _slave_address_rw;
            ack();
            break;

        /// slave 주소 + W 송신 후, ACK 수신
        case TW_MT_SLA_ACK:
            /// 데이터 송신
            twdr = buf[buf_index++];
            ack();
            break;

        /// 데이터 송신 후, ACK 수신
        case TW_MT_DATA_ACK:
            /// 송신할 데이터가 있으면 송신하고, 없으면 재시작 또는 종료
            if( buf_index < buf_length )
            {
                twdr = buf[buf_index++];
                ack();
            }
            else
            {
                if( state == I2C_WRITE_READ )
                {
                    /// SLA_R == (address << 1) | 1
                    _slave_address_rw = _slave_address_rw | 0x01;
                    /// index 초기화
                    /// buf 마지막에 수신할 데이터 수 저장
                    buf_index  = 0;
                    buf_length = buf[buf_length];
                    start();
                }
                else
                {
                    stop();
                }
            }
            break;

        /// slave 주소 + R 송신 후, ACK 수신
        case TW_MR_SLA_ACK:
            /// 데이터 수신 후, ACK 송신 준비
            ack();
            break;

        /// 데이터 수신 후, ACK 송신
        case TW_MR_DATA_ACK:
            /// 수신 데이터 저장
            buf[buf_index++] = twdr;
            /// 다음으로 수신할 데이터가 마지막이면 NACK, 아니면 ACK 준비
            if( buf_index < buf_length - 1 ) { ack(); }
            else
            {
                nack();
            }
            break;

        /// 데이터 수신 후, NACK 송신(마지막 데이터)
        case TW_MR_DATA_NACK:
            buf[buf_index] = twdr;
            stop();
            break;

        /// arbitration lost 상태
        case TW_MT_ARB_LOST:
            error_state = twsr & tws_mask;
            /// master 권한을 다시 가져옴
            ack();
            state = I2C_READY;
            break;

        case TW_NO_INFO: break;

        case TW_MT_SLA_NACK:
        case TW_MT_DATA_NACK:
        case TW_MR_SLA_NACK:
        case TW_BUS_ERROR:
            error_state = twsr & tws_mask;
            stop();
            break;
    }
}

void LOT_i2c0::setup( const uint32_t scl_clock )
{
    twsr = 0;
    twbr = ( ( F_CPU / scl_clock ) - 16 ) / 2;
    sei();
}

LOT_status_typedef
    LOT_i2c0::transmit( const uint8_t slave_address, const uint8_t *data, uint8_t size )
{
    check( I2C_WRITE );

    _slave_address_rw = slave_address << 1;

    if( size > LOT_I2C0_BUF_SIZE ) { return LOT_ERROR; }
    else
    {
        buf_length = size;
    }

    for( uint8_t i = 0; i < buf_length; ++i ) { buf[i] = data[i]; }

    start();
    return LOT_OK;
}

LOT_status_typedef LOT_i2c0::transmit( const uint8_t slave_address,
                                       const uint8_t register_address,
                                       uint8_t *     data,
                                       uint8_t       size )
{
    check( I2C_WRITE );

    _slave_address_rw = slave_address << 1;

    if( size > LOT_I2C0_BUF_SIZE - 1 ) { return LOT_ERROR; }
    else
    {
        buf_length = size;
    }

    buf[0] = register_address;
    for( uint8_t i = 1; i < buf_length + 1; ++i ) { buf[i] = data[i]; }

    start();
    return LOT_OK;
}

void LOT_i2c0::transmit( const uint8_t slave_address, const uint8_t register_address, uint8_t data )
{
    transmit( slave_address, register_address, &data, 1 );
}

uint8_t LOT_i2c0::receive( const uint8_t slave_address, uint8_t *data, uint8_t max_size )
{
    check( I2C_READ );

    _slave_address_rw = ( slave_address << 1 ) | 0x01;

    if( max_size > LOT_I2C0_BUF_SIZE ) { max_size = LOT_I2C0_BUF_SIZE; }

    buf_length = max_size;

    start();
    return buf_length;
}

uint8_t LOT_i2c0::receive( const uint8_t slave_address,
                           const uint8_t register_address,
                           uint8_t *     data,
                           uint8_t       max_size )
{
    check( I2C_WRITE_READ );

    _slave_address_rw = slave_address << 1;

    if( max_size > LOT_I2C0_BUF_SIZE ) { max_size = LOT_I2C0_BUF_SIZE; }

    buf[0]     = register_address;
    buf[1]     = max_size;
    buf_length = 1;

    start();

    /// @todo data 반환이 비효율적

    return max_size;
}

uint8_t LOT_i2c0::receive( const uint8_t slave_address, const uint8_t register_address ) {}

void LOT_i2c0::transmit_slow( const uint8_t slave_address, const uint8_t *data, uint8_t size ) {}
void LOT_i2c0::transmit_slow( const uint8_t slave_address,
                              const uint8_t register_address,
                              uint8_t       data )
{
}
uint8_t LOT_i2c0::receive_slow( const uint8_t slave_address, uint8_t *data, uint8_t max_size ) {}
uint8_t LOT_i2c0::receive_slow( const uint8_t slave_address,
                                const uint8_t register_address,
                                uint8_t *     data,
                                uint8_t       max_size )
{
}
uint8_t LOT_i2c0::receive_slow( const uint8_t slave_address, const uint8_t register_address ) {}

void LOT_i2c0::start( void ) { twcr = twint | twsta | twen | twie; }

void LOT_i2c0::ack( void ) { twcr = twint | twea | twen | twie; }

void LOT_i2c0::nack( void ) { twcr = twint | twen | twie; }

void LOT_i2c0::stop( void )
{
    twcr = twint | twsto | twen;
    // while( ( twcr & twsto ) != 0 ) {}
    state = I2C_READY;
}

void LOT_i2c0::check( uint8_t _state )
{
    while( state != I2C_READY ) {}
    state     = _state;
    buf_index = 0;
}

LOT_i2c0 i2c0;

ISR( TWI_vect ) { i2c0.twi_isr(); }