#include "avr_uart0.h"

#include <avr/interrupt.h>

const uint8_t txc {_BV(TXC0)};
const uint8_t udre {_BV(UDRE0)};
const uint8_t rxcie {_BV(RXCIE0)};
const uint8_t udrie {_BV(UDRIE0)};
const uint8_t rxen {_BV(RXEN0)};
const uint8_t txen {_BV(TXEN0)};
const uint8_t ucsz1{_BV(UCSZ01)};
const uint8_t ucsz0{_BV(UCSZ00)};

void LOT_uart0::rx_isr(void)
{
    rx_buf[rx_buf_head] = udr;
    rx_buf_head         = ( rx_buf_head + 1 ) % LOT_UART_TX_BUF_SIZE;
    if ( rx_buf_head == rx_buf_tail )
    {
        rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART_RX_BUF_SIZE;
    }
}

void LOT_uart0::udre_isr(void)
{
    udr     = tx_buf[tx_buf_tail];
    tx_buf_tail = ( tx_buf_tail + 1 ) % LOT_UART_TX_BUF_SIZE;
    ucsra |= txc;
    if ( tx_buf_head == tx_buf_tail )
    {
        ucsrb &= ~udrie;
    }
}

void LOT_uart0::setup(const uint32_t baud_rate)
{
    ucsra = 0;
    ucsrb = rxcie|rxen|txen;
    ucsrc = ucsz1|ucsz0;
    
    uint16_t temp = (F_CPU / baud_rate / 16) - 1;
    ubrrh = temp >> 8;
    ubrrl = temp;

    sei();
}

uint8_t LOT_uart0::receive(uint8_t &one_byte)
{
    if ( rx_buf_head != rx_buf_tail )
    {
        one_byte = rx_buf[rx_buf_tail];
        rx_buf_tail = ( rx_buf_tail + 1 ) % LOT_UART_RX_BUF_SIZE;
    }
    return (LOT_UART_RX_BUF_SIZE + rx_buf_head - rx_buf_tail) % LOT_UART_RX_BUF_SIZE;
}

void LOT_uart0::transmit(const uint8_t data)
{
    if ( ( tx_buf_head == tx_buf_tail ) && ( ucsra & udre ) )
    {
        udr = data;
        ucsra |= txc;
        return;
    }
    tx_buf[tx_buf_head] = data;
    tx_buf_head         = ( tx_buf_head + 1 ) % LOT_UART_TX_BUF_SIZE;
    while ( tx_buf_head == tx_buf_tail )
    {
    }
    ucsrb |= udrie;
}

LOT_uart0 uart0;

ISR(USART_RX_vect)
{
    uart0.rx_isr();
}

ISR(USART_UDRE_vect)
{
    uart0.udre_isr();
}