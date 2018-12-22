/**
 * @file LOT_uart0.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#ifndef _LOT_UART0_H_
#define _LOT_UART0_H_

#include <inttypes.h>

#include "LOT_transmit.h"
#include "LOT_receive.h"

#if !defined( __AVR_ATmega328P__ ) && !defined( __AVR_ATmega128__ )
#    warning "Untested device"
#endif

/// 버퍼 크기는 32, 64, 128 ... 2의 지수로 설정
#define LOT_UART0_TX_BUF_SIZE 64
#define LOT_UART0_RX_BUF_SIZE 64

class LOT_uart0
    : public LOT_transmit
    , public LOT_receive {
public:
    LOT_uart0();

    /**
     * @brief USART_RX 인터럽트 서비스 루틴에서 호출할 함수
     */
    void rx_isr( void );

    /**
     * @brief USART_UDRE 인터럽트 서비스 루틴에서 호출할 함수
     */
    void udre_isr( void );

    void        setup( const uint32_t baud_rate,
                       const uint8_t  data_bits,
                       const uint8_t  stop_bit,
                       const uint8_t  parity );
    inline void setup( const uint32_t baud_rate ) { setup( baud_rate, 8, 1, 0 ); }

    virtual void transmit_basic( uint8_t data );

    virtual uint16_t receive_basic( uint8_t *data, uint16_t max_size );
    virtual uint16_t receive_size( void );
    virtual uint8_t  receive_basic( void );

protected:
    volatile uint8_t tx_buf_head;
    volatile uint8_t tx_buf_tail;
    volatile uint8_t rx_buf_head;
    volatile uint8_t rx_buf_tail;

    volatile uint8_t tx_buf[LOT_UART0_TX_BUF_SIZE];
    volatile uint8_t rx_buf[LOT_UART0_RX_BUF_SIZE];

private:
};

extern LOT_uart0 uart0;

#endif    // _LOT_UART0_H_
