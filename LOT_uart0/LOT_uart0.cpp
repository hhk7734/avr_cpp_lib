/**
 * @file LOT_uart0.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief
 */

#include "LOT_uart0.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define ucsra UCSR0A
#define ucsrb UCSR0B
#define ucsrc UCSR0C
#define ubrrl UBRR0L
#define ubrrh UBRR0H
#define udr UDR0

/// UCSRA
#define rxc _BV( RXC0 )
#define txc _BV( TXC0 )
#define udre _BV( UDRE0 )
#define fe _BV( FE0 )
#define dor _BV( DOR0 )
#define upe _BV( UPE0 )
#define u2x _BV( U2X0 )
#define mpcm _BV( MPCM0 )

/// UCSRB
#define RXCIE0 7
#define TXCIE0 6
#define UDRIE0 5
#define RXEN0 4
#define TXEN0 3
#define UCSZ02 2
#define RXB80 1
#define TXB80 0

/// UCSRC
#define UMSEL01 7
#define UMSEL00 6
#define UPM01 5
#define UPM00 4
#define USBS0 3
#define UCSZ01 2
#define UCSZ00 1
#define UCPOL0 0

LOT_uart0::LOT_uart0()
    : tx_buf_head( 0 )
    , tx_buf_tail( 0 )
    , rx_buf_head( 0 )
    , rx_buf_tail( 0 )
{
}

void LOT_uart0::rx_isr( void ) {}

void LOT_uart0::udre_isr( void ) {}

void LOT_uart0::setup( const uint32_t baud_rate,
                       const uint8_t  data_bits,
                       const uint8_t  stop_bit,
                       const uint8_t  parity )
{
}

void LOT_uart0::transmit_basic( uint8_t data ) {}

uint16_t LOT_uart0::receive_basic( uint8_t *data, uint16_t max_size ) {}

uint16_t LOT_uart0::receive_size( void ) {}

uint8_t LOT_uart0::receive_basic( void ) {}

LOT_uart0 uart0;

ISR( USART_RX_vect ) { uart0.rx_isr(); }

ISR( USART_UDRE_vect ) { uart0.udre_isr(); }