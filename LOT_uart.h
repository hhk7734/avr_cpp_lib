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
    LOT_uart( volatile uint8_t &_ucsra,
              volatile uint8_t &_ucsrb,
              volatile uint8_t &_ucsrc,
              volatile uint8_t &_ubrrl,
              volatile uint8_t &_ubrrh,
              volatile uint8_t &_udr );

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
    void setup( const uint32_t baud_rate,
                const uint8_t  data_bits,
                const uint8_t  stop_bits,
                const uint8_t  parity );
    void setup( const uint32_t baud_rate );
    /// @}

    /**
     * @brief 데이터 송신
     * @param uint8_t data
     * @return uint8_t 송신한 데이터 수
     */
    uint8_t         put( uint8_t data );
    virtual uint8_t put( char c );
    using LOT_ostream::put;

    /**
     * @brief 데이터 수신
     * @{
     */

    /// @param char 수신 데이터
    uint8_t           get( void );
    virtual LOT_uart &get( char *p );

    /**
     * @param uint8_t *p 수신 데이터
     * @param uint8_t n 수신 데이터 수
     * @return LOT_uart&
     */
    LOT_uart &        get( uint8_t *p, uint8_t n );
    virtual LOT_uart &get( char *p, uint8_t n );
    /// @}

    /**
     * @brief 읽지 않은 데이터 수
     * @return uint8_t 읽지 않은 데이터 수
     */
    virtual uint8_t gcount( void );

    /**
     * @brief USART_RX 인터럽트 서비스 루틴에서 호출할 함수
     */
    void rx_isr( void );

    /**
     * @brief USART_UDRE 인터럽트 서비스 루틴에서 호출할 함수
     */
    void udre_isr( void );

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

#if defined( _USE_LOT_UART_0_ )
extern LOT_uart uart0;
#    define uart uart0
#endif

#if defined( _USE_LOT_UART_1_ )
extern LOT_uart uart1;
#endif

#if defined( _USE_LOT_UART_2_ )
extern LOT_uart uart2;
#endif

#if defined( _USE_LOT_UART_3_ )
extern LOT_uart uart3;
#endif

#endif    // _LOT_UART_H_