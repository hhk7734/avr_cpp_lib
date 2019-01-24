/**
 * @file LOT_transmit.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 문자열 송신
 */

#ifndef _LOT_TRANSMIT_H_
#define _LOT_TRANSMIT_H_

#include <stdint.h>

class LOT_transmit {
public:
    inline LOT_transmit() {}

    /**
     * @brief 데이터 송신 가상 함수
     * @param uint8_t data 송신할 데이터
     */
    virtual void transmit_basic( uint8_t data ) = 0;

    /**
     * @brief 데이터 송신 가상 함수
     * @param uint8_t *data 송신할 데이터
     * @param uint16_t size 송신할 데이터 수
     */
    virtual void transmit_basic( uint8_t *data, uint16_t size ) = 0;

    /**
     * @brief 숫자형 데이터를 문자열로 송신
     * @param type data 송신할 데이터
     */
    inline void transmit( uint8_t *data, uint8_t size ) { transmit_basic( data, size ); }
    inline void transmit( uint8_t data ) { transmit_basic( data ); }
    inline void transmit( char data ) { transmit_basic( static_cast<uint8_t>( data ) ); }
    void        transmit( const char data[] );
    void        transmit( uint16_t data );
    void        transmit( int16_t data );
    void        transmit( uint32_t data );
    void        transmit( int32_t data );
    void        transmit( uint64_t data );
    void        transmit( int64_t data );
    void        transmit( double data, uint8_t digits = 2 );
    inline void transmit( float data, uint8_t digits = 2 )
    {
        transmit( static_cast<double>( data ), digits );
    }

    /**
     * @brief 원하는 데이터 송신 후 "\r\n" 송신
     * @param T data 송신할 데이터
     */
    template<typename T>
    inline void transmit_CR_NL( T data )
    {
        transmit( data );
        transmit( "\r\n" );
    }

    inline void transmit_CR_NL( double data, uint8_t digits = 2 )
    {
        transmit( data, digits );
        transmit( "\r\n" );
    }

    inline void transmit_CR_NL( float data, uint8_t digits = 2 )
    {
        transmit( data, digits );
        transmit( "\r\n" );
    }

protected:
private:
};

#endif    // _LOT_TRANSMIT_H_
