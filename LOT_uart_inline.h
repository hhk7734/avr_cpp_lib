/**
 * @file LOT_uart_inline.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 */

void LOT_uart::rx_isr( void )
{
    rx_buf[rx_buf_head] = udr;
    rx_buf_head         = ( rx_buf_head + 1 ) % LOT_UART_TX_BUF_SIZE;
    if ( rx_buf_head == rx_buf_tail )
    {
        rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART_RX_BUF_SIZE;
    }
}

void LOT_uart::udre_isr( void )
{
    udr         = tx_buf[tx_buf_tail];
    tx_buf_tail = ( tx_buf_tail + 1 ) % LOT_UART_TX_BUF_SIZE;
    ucsra |= LOT_TXC;
    if ( tx_buf_head == tx_buf_tail )
    {
        ucsrb &= ~LOT_UDRIE;
    }
}