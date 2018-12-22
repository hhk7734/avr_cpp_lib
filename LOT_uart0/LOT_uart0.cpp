/**
 * @file LOT_uart0.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief UART0 통신
 */

#include "LOT_uart0.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t &ucsra { UCSR0A };
volatile uint8_t &ucsrb { UCSR0B };
volatile uint8_t &ucsrc { UCSR0C };
volatile uint8_t &ubrrl { UBRR0L };
volatile uint8_t &ubrrh { UBRR0H };
volatile uint8_t &udr { UDR0 };

/// UCSRA
const uint8_t rxc { _BV( RXC0 ) };
const uint8_t txc { _BV( TXC0 ) };
const uint8_t udre { _BV( UDRE0 ) };
const uint8_t fe { _BV( FE0 ) };
const uint8_t dor { _BV( DOR0 ) };
const uint8_t upe { _BV( UPE0 ) };
const uint8_t u2x { _BV( U2X0 ) };
const uint8_t mpcm { _BV( MPCM0 ) };

/// UCSRB
const uint8_t rxcie { _BV( RXCIE0 ) };
const uint8_t txcie { _BV( TXCIE0 ) };
const uint8_t udrie { _BV( UDRIE0 ) };
const uint8_t rxen { _BV( RXEN0 ) };
const uint8_t txen { _BV( TXEN0 ) };
const uint8_t ucsz2 { _BV( UCSZ02 ) };
const uint8_t rxb8 { _BV( RXB80 ) };
const uint8_t txb8 { _BV( TXB80 ) };

/// UCSRC
const uint8_t umsel1 { _BV( UMSEL01 ) };
const uint8_t umsel0 { _BV( UMSEL00 ) };
const uint8_t upm1 { _BV( UPM01 ) };
const uint8_t upm0 { _BV( UPM00 ) };
const uint8_t usbs { _BV( USBS0 ) };
const uint8_t ucsz1 { _BV( UCSZ01 ) };
const uint8_t ucsz0 { _BV( UCSZ00 ) };
const uint8_t ucpol { _BV( UCPOL0 ) };

LOT_uart0::LOT_uart0()
    : rx_buf_head( 0 )
    , rx_buf_tail( 0 )
    , tx_buf_head( 0 )
    , tx_buf_tail( 0 )
{
}

void LOT_uart0::rx_isr( void )
{
    rx_buf[rx_buf_head] = udr;
    rx_buf_head         = ( rx_buf_head + 1 ) % LOT_UART0_TX_BUF_SIZE;
    if( rx_buf_head == rx_buf_tail ) { rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART0_RX_BUF_SIZE; }
}
void LOT_uart0::udre_isr( void )
{
    udr         = tx_buf[tx_buf_tail];
    tx_buf_tail = ( tx_buf_tail + 1 ) % LOT_UART0_TX_BUF_SIZE;
    ucsra |= txc;
    if( tx_buf_head == tx_buf_tail ) { ucsrb &= ~udrie; }
}

void LOT_uart0::setup( const uint32_t baud_rate,
                       const uint8_t  data_bits,
                       const uint8_t  stop_bits,
                       const uint8_t  parity )
{
    /// @todo data bits, stop_bits, parity
    /// @todo u2x 모드 사용 아두이노는 기본으로 u2x 사용하고 문제가 있는경우만 non u2x
    /// @todo ubrr 0~4095까지 가능
    /// @todo 57600 모드

    ucsra = 0;
    ucsrb = rxcie | rxen | txen;
    ucsrc = ucsz1 | ucsz0;

    uint16_t temp = ( F_CPU / baud_rate / 16 ) - 1;
    ubrrh         = temp >> 8;
    ubrrl         = temp;

    sei();
}

void LOT_uart0::transmit_basic( uint8_t data )
{
    if( ( tx_buf_head == tx_buf_tail ) && ( ucsra & udre ) )
    {
        udr = data;
        ucsra |= txc;
        return;
    }
    tx_buf[tx_buf_head] = data;
    tx_buf_head         = ( tx_buf_head + 1 ) % LOT_UART0_TX_BUF_SIZE;
    while( tx_buf_head == tx_buf_tail ) {}
    ucsrb |= udrie;
}

uint16_t LOT_uart0::receive_basic( uint8_t *data, uint16_t max_size )
{
    uint8_t temp = ( LOT_UART0_RX_BUF_SIZE + rx_buf_head - rx_buf_tail ) % LOT_UART0_RX_BUF_SIZE;
    if( temp < max_size ) { max_size = temp; }
    for( uint8_t i = 0; i < max_size; ++i )
    {
        data[i]     = rx_buf[rx_buf_tail];
        rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART0_RX_BUF_SIZE;
    }
    return max_size;
}

uint16_t LOT_uart0::receive_size( void )
{
    return ( LOT_UART0_RX_BUF_SIZE + rx_buf_head - rx_buf_tail ) % LOT_UART0_RX_BUF_SIZE;
}

uint8_t LOT_uart0::receive_basic( void )
{
    uint8_t data = rx_buf[rx_buf_tail];
    rx_buf_tail  = ( rx_buf_tail + 1 ) % LOT_UART0_RX_BUF_SIZE;
    return data;
}

LOT_uart0 uart0;

ISR( USART_RX_vect ) { uart0.rx_isr(); }

ISR( USART_UDRE_vect ) { uart0.udre_isr(); }