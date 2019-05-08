/**
 * @file LOT_uart_inline.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 */

inline __attribute__( ( always_inline ) ) void LOT_uart::setup( const uint32_t baud_rate )
{
    setup( baud_rate, 8, 1, 0 );
}

inline __attribute__( ( always_inline ) ) uint8_t LOT_uart::put( char c )
{
    return put( static_cast<uint8_t>( c ) );
}

inline __attribute__( ( always_inline ) ) LOT_uart &LOT_uart::get( char *p )
{
    *p = get();
    return *this;
}

inline __attribute__( ( always_inline ) ) LOT_uart &LOT_uart::get( char *p, uint8_t n )
{
    return get( ( uint8_t * )p, n );
}

inline __attribute__( ( always_inline ) ) void LOT_uart::rx_isr( void )
{
    rx_buf[rx_buf_head] = udr;
    rx_buf_head         = ( rx_buf_head + 1 ) % LOT_UART_TX_BUF_SIZE;
    if( rx_buf_head == rx_buf_tail ) { rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART_RX_BUF_SIZE; }
}

inline __attribute__( ( always_inline ) ) void LOT_uart::udre_isr( void )
{
    udr         = tx_buf[tx_buf_tail];
    tx_buf_tail = ( tx_buf_tail + 1 ) % LOT_UART_TX_BUF_SIZE;
    _set_bit( ucsra, LOT_TXC );
    if( tx_buf_head == tx_buf_tail ) { _clear_bit( ucsrb, LOT_UDRIE ); }
}
