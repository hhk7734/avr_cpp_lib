/**
 * @file avr_uart0.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief UART0 통신
 */

#ifndef _AVR_UART0_H_
#define _AVR_UART0_H_

#include <avr/io.h>

#include "avr_uart_basic.h"

/// 버퍼 크기는 32, 64, 128 ... 2의 지수로 설정
#define LOT_UART0_TX_BUF_SIZE 64
#define LOT_UART0_RX_BUF_SIZE 64

class LOT_uart0 : public LOT_uart_basic {
public:
    LOT_uart0()
        : LOT_uart_basic( UCSR0A, UCSR0B, UCSR0C, UBRR0L, UBRR0H, UDR0 )
    {
    }

    void    rx_isr( void );
    void    udre_isr( void );
    void    setup( const uint32_t baud_rate );
    uint8_t receive( uint8_t *data, uint8_t max_size );
    void    transmit( const uint8_t data );

    using LOT_uart_basic::transmit;

protected:
    volatile uint8_t tx_buf[LOT_UART0_TX_BUF_SIZE];
    volatile uint8_t rx_buf[LOT_UART0_RX_BUF_SIZE];
};

/// 객체 생성
extern LOT_uart0 uart0;

#endif    // _AVR_UART0_H_