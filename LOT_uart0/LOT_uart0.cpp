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
const uint8_t LOT_RXC { _BV( RXC0 ) };
const uint8_t LOT_TXC { _BV( TXC0 ) };
const uint8_t LOT_UDRE { _BV( UDRE0 ) };
const uint8_t LOT_FE { _BV( FE0 ) };
const uint8_t LOT_DOR { _BV( DOR0 ) };
const uint8_t LOT_UPE { _BV( UPE0 ) };
const uint8_t LOT_U2X { _BV( U2X0 ) };
const uint8_t LOT_MPCM { _BV( MPCM0 ) };

/// UCSRB
const uint8_t LOT_RXCIE { _BV( RXCIE0 ) };
const uint8_t LOT_TXCIE { _BV( TXCIE0 ) };
const uint8_t LOT_UDRIE { _BV( UDRIE0 ) };
const uint8_t LOT_RXEN { _BV( RXEN0 ) };
const uint8_t LOT_TXEN { _BV( TXEN0 ) };
const uint8_t LOT_UCSZ2 { _BV( UCSZ02 ) };
const uint8_t LOT_RXB8 { _BV( RXB80 ) };
const uint8_t LOT_TXB8 { _BV( TXB80 ) };

/// UCSRC
const uint8_t LOT_UMSEL1 { _BV( UMSEL01 ) };
const uint8_t LOT_UMSEL0 { _BV( UMSEL00 ) };
const uint8_t LOT_UPM1 { _BV( UPM01 ) };
const uint8_t LOT_UPM0 { _BV( UPM00 ) };
const uint8_t LOT_USBS { _BV( USBS0 ) };
const uint8_t LOT_UCSZ1 { _BV( UCSZ01 ) };
const uint8_t LOT_UCSZ0 { _BV( UCSZ00 ) };
const uint8_t LOT_UCPOL { _BV( UCPOL0 ) };

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
    ucsra |= LOT_TXC;
    if( tx_buf_head == tx_buf_tail ) { ucsrb &= ~LOT_UDRIE; }
}

void LOT_uart0::setup( const uint32_t baud_rate,
                       const uint8_t  data_bits,
                       const uint8_t  stop_bits,
                       const uint8_t  parity )
{
    /// @todo 9bit 지원
    /// @todo u2x 문제 있나 확인

    ucsra         = LOT_U2X;
    uint16_t temp = ( F_CPU / baud_rate / 8 ) - 1;

    ucsrb = LOT_RXCIE | LOT_RXEN | LOT_TXEN;
    if( data_bits < 9 ) { ucsrc = ( data_bits - 5 ) << 1; }
    else
    {
        ucsrc = ( 8 - 5 ) << 1;
    }

    switch( parity )
    {
        /// odd parity
        case 1: ucsrc |= LOT_UPM0;
        /// even parity
        case 2: ucsrc |= LOT_UPM1; break;
    }

    if( stop_bits == 2 ) { ucsrc |= LOT_USBS; }

    ubrrh = temp >> 8;
    ubrrl = temp;

    sei();
}

void LOT_uart0::transmit_basic( uint8_t data )
{
    if( ( tx_buf_head == tx_buf_tail ) && ( ucsra & LOT_UDRE ) )
    {
        udr = data;
        ucsra |= LOT_TXC;
        return;
    }
    tx_buf[tx_buf_head] = data;
    tx_buf_head         = ( tx_buf_head + 1 ) % LOT_UART0_TX_BUF_SIZE;
    while( tx_buf_head == tx_buf_tail ) {}
    ucsrb |= LOT_UDRIE;
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

#if defined( USART_RX_vect )
ISR( USART_RX_vect )
#elif defined( USART0_RX_vect )
ISR( USART0_RX_vect )
#endif
{
    uart0.rx_isr();
}

#if defined( USART_UDRE_vect )
ISR( USART_UDRE_vect )
#elif defined( USART0_UDRE_vect )
ISR( USART0_UDRE_vect )
#endif
{
    uart0.udre_isr();
}