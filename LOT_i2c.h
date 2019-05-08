/**
 * @file LOT_i2c.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief AVR I2C 마스터 통신
 */

#ifndef _LOT_I2C_H_
#define _LOT_I2C_H_

#include <LOT_macro.h>
#include <stdint.h>
#include <LOT_i2c_const.h>
#include <util/twi.h>

class LOT_i2c {
public:
    /**
     * @brief I2C 레지스터 초기화
     */
    LOT_i2c( volatile uint8_t &_twbr,
             volatile uint8_t &_twsr,
             volatile uint8_t &_twdr,
             volatile uint8_t &_twcr );

    /**
     * @name I2C 통신 설정
     * @{
     */

    /// @param uint32_t scl_clock scl 주파수
    void setup( const uint32_t scl_clock );

    void setup( void );
    /// @}

    /**
     * @name 데이터 송신
     * @{
     */

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
    /// @}

    /**
     * @name 데이터 수신
     * @{
     */

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
    /// @}

    /// @todo 통신 사이에 간격이 필요한 경우 코드 작성 필요

protected:
    /**
     * @brief I2C 통신을 통해 데이터를 전송\n
     * 자주 호출되는 함수 -> noinline
     * @param uint8_t _twcr I2C0 컨트롤 레지스터
     * @return LOT_status_typedef
     */
    LOT_status_typedef control( const uint8_t _twcr );

    /**
     * @brief slave + write(0) 신호
     * @param uint8_t slave_address
     * @return LOT_status_typedef
     */
    LOT_status_typedef SLA_W( const uint8_t slave_address );

    /**
     * @brief slave + read(1) 신호
     * @param uint8_t slave_address
     * @return LOT_status_typedef
     */
    LOT_status_typedef SLA_R( const uint8_t slave_address );

    /**
     * @brief 데이터 송신
     * @param uint8_t data
     * @return LOT_status_typedef
     */
    LOT_status_typedef transmit_data( const uint8_t data );

    /**
     * @brief 데이터 수신 + ACK
     * @return uint8_t 수신 데이터
     */
    uint8_t receive_data( void );

    /**
     * @brief 데이터 수신 + NACK
     * @return uint8_t 수신 데이터
     */
    uint8_t receive_last_data( void );

    /**
     * @brief 통신 종료
     */
    void stop( void );

    /**
     * @brief 통신 에러가 발생한 경우 처리하는 함수
     */
    void error( void );

    volatile uint8_t &twbr;
    volatile uint8_t &twsr;
    volatile uint8_t &twdr;
    volatile uint8_t &twcr;

    uint8_t error_state;
    uint8_t error_count;
};

#include <LOT_i2c_inline.h>

#if defined( _USE_LOT_I2C_0_ )
extern LOT_i2c i2c0;
#    define i2c i2c0
#endif

#endif    // _LOT_I2C_H_
