/**
 * @file LOT_i2c0.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#ifndef _LOT_I2C0_H_
#define _LOT_I2C0_H_

#include <inttypes.h>

#if !defined( __AVR_ATmega328P__ )
#    warning "Untested device"
#endif

#define LOT_I2C0_DEFAULT_CLOCK 400000UL

#define LOT_I2C0_TRANSFER_INTERVAL_US 20
#define LOT_I2C0_TIME_OUT 255

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

    /**
     * @brief I2C0 통신 속도 설정
     * @param uint32_t scl_clock scl 주파수
     */
    void        setup( const uint32_t scl_clock );
    inline void setup( void ) { setup( LOT_I2C0_DEFAULT_CLOCK ); }

    LOT_status_typedef transmit( const uint8_t slave_address, const uint8_t *data, uint8_t size );
    LOT_status_typedef transmit( const uint8_t slave_address,
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
    uint8_t error_state;

    /**
     * @brief
     * I2C 통신을 통해 데이터를 전송\n
     * 자주 호출되는 함수 -> noinline
     * @param uint8_t twcr I2C0 컨트롤 레지스터
     * @return LOT_status_typedef
     */
    LOT_status_typedef control( const uint8_t &_twcr ) __attribute__( ( noinline ) );
    void               error( void );
};

#endif    // _LOT_I2C0_H_