/**
 * @file LOT_i2c0.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief i2c 마스터 통신
 */

#ifndef _LOT_I2C0_H_
#define _LOT_I2C0_H_

#include <inttypes.h>

#if !defined( __AVR_ATmega328P__ )
#    warning "Untested device"
#endif

#define LOT_I2C0_DEFAULT_CLOCK 400000UL

/// 버퍼 크기는 32, 64, 128 ... 2의 지수로 설정
#define LOT_I2C0_BUF_SIZE 32

#define LOT_I2C0_TRANSFER_INTERVAL_US 1000

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

class LOT_i2c0 {
public:
    LOT_i2c0();

    void twi_isr( void );

    void        setup( const uint32_t scl_clock );
    inline void setup( void ) { setup( LOT_I2C0_DEFAULT_CLOCK ); }

    uint8_t transmit( const uint8_t slave_address, const uint8_t *data, uint8_t size );
    uint8_t transmit( const uint8_t slave_address,
                      const uint8_t register_address,
                      uint8_t *     data,
                      uint8_t       size );
    void    transmit( const uint8_t slave_address, const uint8_t register_address, uint8_t data );
    uint8_t receive( const uint8_t slave_address, uint8_t *data, uint8_t max_size );
    uint8_t receive( const uint8_t slave_address,
                     const uint8_t register_address,
                     uint8_t *     data,
                     uint8_t       max_size );
    uint8_t receive( const uint8_t slave_address, const uint8_t register_address );

    void transmit_slow( const uint8_t slave_address, const uint8_t *data, uint8_t size );
    void transmit_slow( const uint8_t slave_address, const uint8_t register_address, uint8_t data );
    uint8_t receive_slow( const uint8_t slave_address, uint8_t *data, uint8_t max_size );
    uint8_t receive_slow( const uint8_t slave_address,
                          const uint8_t register_address,
                          uint8_t *     data,
                          uint8_t       max_size );
    uint8_t receive_slow( const uint8_t slave_address, const uint8_t register_address );

protected:
private:
    volatile uint8_t _slave_address_rw;
    volatile uint8_t error_state;
    volatile uint8_t state;

    volatile uint8_t buf_index;
    volatile uint8_t buf_length;

    volatile uint8_t buf[LOT_I2C0_BUF_SIZE];

    void start( void );
    void ack( void );
    void nack( void );
    void stop( void );
};

extern LOT_i2c0 i2c0;

#endif    // _LOT_I2C0_H_
