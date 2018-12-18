/**
 * @file avr_uart_basic.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief UART 통신 구축
 */

#ifndef _AVR_UART_BASIC_
#define _AVR_UART_BASIC_

#include <inttypes.h>

class LOT_uart_basic {
public:
    /**
     * @brief 생성자
     * @param volatile uint8_t &xxx XXX 레지스터
     */
    LOT_uart_basic( volatile uint8_t &_ucsra,
                    volatile uint8_t &_ucsrb,
                    volatile uint8_t &_ucsrc,
                    volatile uint8_t &_ubrrl,
                    volatile uint8_t &_ubrrh,
                    volatile uint8_t &_udr );

    /**
     * @brief USART_RX 인터럽트 서비스 구현을 위한 가상 함수
     */
    virtual void rx_isr( void ) = 0;

    /**
     * @brief USART_UDRE 인터럽트 서비스 구현을 위한 가상 함수
     */
    virtual void udre_isr( void ) = 0;

    /**
     * @brief USART 통신 설정 가상 함수
     * @param const uint32_t baud_rate 보오 레이트
     */
    virtual void setup( const uint32_t baud_rate ) = 0;

    /**
     * @brief 수신 버퍼 읽기 가상 함수
     * @param uint8_t *data 읽은 데이터
     * @param uint8_t max_size 읽어들일 데이터 최대 개수
     * @return uint8_t 읽은 데이터 개수
     */
    virtual uint8_t receive( uint8_t *data, uint8_t max_size ) = 0;

    /**
     * @brief 데이터 송신 가상 함수
     * @param 송신할 데이터
     */
    virtual void transmit( const uint8_t data ) = 0;

    inline void transmit( const char data ) { transmit( static_cast<uint8_t>( data ) ); }
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
     * @brief 원하는 데이터 송신 후 "\n\r" 송신
     * @param T data 송신할 데이터
     */
    template<typename T>
    inline void transmit_nlcr( T data )
    {
        transmit( data );
        transmit( "\n\r" );
    }

    inline void transmit_nlcr( double data, uint8_t digits = 2 )
    {
        transmit( data, digits );
        transmit( "\n\r" );
    }

    inline void transmit_nlcr( float data, uint8_t digits = 2 )
    {
        transmit( data, digits );
        transmit( "\n\r" );
    }

protected:
    volatile uint8_t &ucsra;
    volatile uint8_t &ucsrb;
    volatile uint8_t &ucsrc;
    volatile uint8_t &ubrrl;
    volatile uint8_t &ubrrh;
    volatile uint8_t &udr;

    volatile uint8_t tx_buf_head;
    volatile uint8_t tx_buf_tail;
    volatile uint8_t rx_buf_head;
    volatile uint8_t rx_buf_tail;
};

#endif    // _AVR_UART_BASIC_