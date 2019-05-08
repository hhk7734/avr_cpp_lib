/**
 * @file LOT_uart.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief AVR UART 통신
 */

#ifndef _LOT_UART_H_
#define _LOT_UART_H_

#include <LOT_iostream.h>
#include <LOT_macro.h>
#include <LOT_uart_const.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#if !defined( __AVR_ATmega328P__ )
#    warning "Untested device"
#endif

/**
 * @name UART 선택
 * 사용하는 경우 1, 아니면 0
 * @{
 */
#define USE_LOT_UART_0 1
#define USE_LOT_UART_1 0
#define USE_LOT_UART_2 0
#define USE_LOT_UART_3 0
/// @}

/**
 * @name ring buffer
 * 버퍼 크기는 32, 64, 128 ... 2의 지수로 설정
 * @{
 */
#define LOT_UART_TX_BUF_SIZE 32
#define LOT_UART_RX_BUF_SIZE 32
/// @}

class LOT_uart : public LOT_iostream {
    public:
    /**
     * @brief UART 레지스터 초기화
     */
    LOT_uart( volatile uint8_t &_ucsra, volatile uint8_t &_ucsrb, volatile uint8_t &_ucsrc, volatile uint8_t &_ubrrl,
              volatile uint8_t &_ubrrh, volatile uint8_t &_udr );

    /**
     * @brief UART 통신 설정
     * @param uint32_t baud_rate
     * @{
     */

    /**
     * @param uint8_t data_bits 5, 6, 7, 8 or 9 bits
     * @param uint8_t stop_bits 1 or 2 bits
     * @param uint8_t parity 0-none, 1-odd, 2-even
     */
    void setup( const uint32_t baud_rate, const uint8_t data_bits, const uint8_t stop_bits, const uint8_t parity );

    inline __attribute__( ( always_inline ) ) void setup( const uint32_t baud_rate )
    {
        setup( baud_rate, 8, 1, 0 );
    }
    /// @}

    /**
     * @brief 데이터 송신
     * @param uint8_t data
     * @return uint8_t 송신한 데이터 수
     */
    virtual uint8_t put( uint8_t data );
    using LOT_ostream::put;

    /**
     * @brief 데이터 수신
     * @{
     */

    /// @param char 수신 데이터
    uint8_t           get( void );
    virtual LOT_uart &get( char *p )
    {
        *p = get();
        return *this;
    }

    /**
     * @param uint8_t *data 수신 데이터
     * @param uint8_t n 수신 데이터 수
     * @return LOT_uart&
     */
    LOT_uart &        get( uint8_t *data, uint8_t n );
    virtual LOT_uart &get( char *p, uint8_t n )
    {
        return get( ( uint8_t * )p, n );
    }
    /// @}

    /**
     * @brief 읽지 않은 데이터 수
     * @return uint8_t 읽지 않은 데이터 수
     */
    virtual uint8_t gcount( void );

    /**
     * @brief USART_RX 인터럽트 서비스 루틴에서 호출할 함수
     */
    inline __attribute__( ( always_inline ) ) void rx_isr( void );

    /**
     * @brief USART_UDRE 인터럽트 서비스 루틴에서 호출할 함수
     */
    inline __attribute__( ( always_inline ) ) void udre_isr( void );

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

    volatile uint8_t tx_buf[LOT_UART_TX_BUF_SIZE];
    volatile uint8_t rx_buf[LOT_UART_RX_BUF_SIZE];
};

#include <LOT_uart_inline.h>

#if USE_LOT_UART_0
extern LOT_uart uart0;
#    define uart uart0
#endif

#if USE_LOT_UART_1
exter LOT_uart uart1;
#endif

#endif // _LOT_UART_H_