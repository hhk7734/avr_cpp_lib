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

    /**
     * @brief slave 장치로 데이터 송신
     * @param uint8_t slave_address
     * @param uint8_t *data
     * @param uint8_t size
     * @return LOT_status_typedef
     */
    LOT_status_typedef transmit( const uint8_t slave_address, const uint8_t *data, uint8_t size );
    /**
     * @brief slave 장치의 register로 송신
     * @param uint8_t slave_address
     * @param uint8_t register_address
     * @param uint8_t *data
     * @param uint8_t size 연속 쓰기가 가능한 경우 연속된 레지스터 수
     * @return LOT_status_typedef
     */
    LOT_status_typedef transmit( const uint8_t  slave_address,
                                 const uint8_t  register_address,
                                 const uint8_t *data,
                                 uint8_t        size );
    /**
     * @brief slave 장치의 register로 송신
     * @param uint8_t slave_address
     * @param uint8_t register_address
     * @param uint8_t data
     * @return LOT_status_typedef
     */
    LOT_status_typedef
        transmit( const uint8_t slave_address, const uint8_t register_address, const uint8_t data );

    /**
     * @brief slave 장치에서 데이터 수신
     * @param uint8_t slave_address
     * @param uint8_t *data 수신 데이터를 저장
     * @param uint8_t size 수신할 데이터 수
     * @return LOT_status_typedef
     */
    LOT_status_typedef receive( const uint8_t slave_address, uint8_t *data, uint8_t size );
    /**
     * @brief slave 장치의 register 데이터 수신
     * @param uint8_t slave_address
     * @param uint8_t register_address
     * @param uint8_t *data 레지스터의 데이터
     * @param uint8_t size 연속 읽기가 가능한 경우 연속된 레지스터 수
     * @return LOT_status_typedef
     */
    LOT_status_typedef receive( const uint8_t slave_address,
                                const uint8_t register_address,
                                uint8_t *     data,
                                uint8_t       size );
    /**
     * @brief slave 장치의 register 데이터 수신
     * @param uint8_t slave_address
     * @param uint8_t register_address
     * @return uint8_t 레지스터의 데이터
     */
    uint8_t receive( const uint8_t slave_address, const uint8_t register_address );

    /// @todo 통신 간격이 길어야하는 경우
    LOT_status_typedef transmit( const uint8_t  slave_address,
                                 const uint8_t *data,
                                 uint8_t        size,
                                 const uint8_t  comunication_interval_us );
    LOT_status_typedef transmit( const uint8_t  slave_address,
                                 const uint8_t  register_address,
                                 const uint8_t *data,
                                 uint8_t        size,
                                 const uint8_t  comunication_interval_us );
    LOT_status_typedef transmit( const uint8_t slave_address,
                                 const uint8_t register_address,
                                 const uint8_t data,
                                 const uint8_t comunication_interval_us );
    LOT_status_typedef receive( const uint8_t slave_address,
                                uint8_t *     data,
                                uint8_t       size,
                                const uint8_t comunication_interval_us );
    LOT_status_typedef receive( const uint8_t slave_address,
                                const uint8_t register_address,
                                uint8_t *     data,
                                uint8_t       size,
                                const uint8_t comunication_interval_us );
    uint8_t            receive( const uint8_t slave_address,
                                const uint8_t register_address,
                                const uint8_t comunication_interval_us );

protected:
private:
    uint8_t error_state;

    /**
     * @brief I2C 통신을 통해 데이터를 전송\n
     * 자주 호출되는 함수 -> noinline
     * @param uint8_t _twcr I2C0 컨트롤 레지스터
     * @return LOT_status_typedef
     */
    LOT_status_typedef __attribute__( ( noinline ) ) control( const uint8_t _twcr );

    LOT_status_typedef SLA_W( const uint8_t slave_address );
    LOT_status_typedef SLA_R( const uint8_t slave_address );
    LOT_status_typedef transmit_data( const uint8_t data );
    uint8_t            receive_data( void );
    uint8_t            receive_last_data( void );
    void               stop( void );

    /**
     * @brief 통신 에러가 발생한 경우 처리하는 함수
     */
    void __attribute__( ( weak ) ) error( void );
};

inline LOT_status_typedef LOT_i2c0::transmit( const uint8_t slave_address,
                                              const uint8_t register_address,
                                              const uint8_t data )
{
    return transmit( slave_address, register_address, &data, 1 );
}

inline uint8_t LOT_i2c0::receive( const uint8_t slave_address, const uint8_t register_address )
{
    uint8_t temp;
    receive( slave_address, register_address, &temp, 1 );
    return temp;
}

extern LOT_i2c0 i2c0;

#endif    // _LOT_I2C0_H_