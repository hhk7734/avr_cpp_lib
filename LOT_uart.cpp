/**
 * @file LOT_uart.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief AVR UART 통신
 */

#include <LOT_uart.h>

LOT_uart::LOT_uart( volatile uint8_t &_ucsra, volatile uint8_t &_ucsrb, volatile uint8_t &_ucsrc,
                    volatile uint8_t &_ubrrl, volatile uint8_t &_ubrrh, volatile uint8_t &_udr )
    : ucsra( _ucsra ), ucsrb( _ucsrb ), ucsrc( _ucsrc ), ubrrl( _ubrrl ), ubrrh( _ubrrh ), udr( _udr )
{
}

void LOT_uart::setup( const uint32_t baud_rate, const uint8_t data_bits, const uint8_t stop_bits, const uint8_t parity )
{
    /// @todo u2x 문제 생기면 해결
    ucsra = LOT_U2X;
    /// ( F_CPU / baud_rate / 8 ) - 1 을 계산한 뒤 소수점 첫째 자리에서 반올림 해야하지만
    /// 부동소수 -> 정수로 갈때 버림을 하기 때문에 0.5를 더한다
    uint16_t temp = ( ( F_CPU / baud_rate / 4 ) - 1 ) / 2;

    ucsrb = LOT_RXCIE | LOT_RXEN | LOT_TXEN;

    /// @todo 9bit 지원
    if ( data_bits < 9 )
    {
        ucsrc = ( data_bits - 5 ) << 1;
    }
    else
    {
        ucsrc = ( 8 - 5 ) << 1;
    }

    switch ( parity )
    {
        /// odd parity
        case 1:
            ucsrc |= LOT_UPM0;
        /// even parity
        case 2:
            ucsrc |= LOT_UPM1;
            break;
    }

    if ( stop_bits == 2 )
    {
        ucsrc |= LOT_USBS;
    }

    ubrrh = temp >> 8;
    ubrrl = temp;

    sei();
}

uint8_t LOT_uart::put( uint8_t data )
{
    if ( ( tx_buf_head == tx_buf_tail ) && ( ucsra & LOT_UDRE ) )
    {
        udr = data;
        ucsra |= LOT_TXC;
        return 1;
    }

    tx_buf[tx_buf_head] = data;
    tx_buf_head         = ( tx_buf_head + 1 ) % LOT_UART_TX_BUF_SIZE;
    while ( tx_buf_head == tx_buf_tail )
    {
    }
    ucsrb |= LOT_UDRIE;
    return 1;
}

uint8_t LOT_uart::get( void )
{
    uint8_t data = rx_buf[rx_buf_tail];
    rx_buf_tail  = ( rx_buf_tail + 1 ) % LOT_UART_RX_BUF_SIZE;
    return data;
}

LOT_uart &LOT_uart::get( uint8_t *data, uint8_t n )
{
    uint8_t temp = ( LOT_UART_RX_BUF_SIZE + rx_buf_head - rx_buf_tail ) % LOT_UART_RX_BUF_SIZE;
    if ( temp < n )
    {
        n = temp;
    }
    for ( uint8_t i = 0; i < n; ++i )
    {
        data[i]     = rx_buf[rx_buf_tail];
        rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART_RX_BUF_SIZE;
    }

    return *this;
}

uint8_t LOT_uart::gcount( void )
{
    return ( LOT_UART_RX_BUF_SIZE + rx_buf_head - rx_buf_tail ) % LOT_UART_RX_BUF_SIZE;
}