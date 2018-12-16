/**
 * @file avr_uart_basic.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief UART 통신 구축
 */

#include "avr_uart_basic.h"

LOT_uart_basic::LOT_uart_basic(volatile uint8_t &_ucsra, volatile uint8_t &_ucsrb,
    volatile uint8_t &_ucsrc, volatile uint8_t &_ubrrh,
    volatile uint8_t &_ubrrl, volatile uint8_t &_udr) :
    ucsra(_ucsra), ucsrb(_ucsrb), ucsrc(_ucsrc),
    ubrrh(_ubrrh), ubrrl(_ubrrl), udr(_udr),
    tx_buf_head(0), tx_buf_tail(0),
    rx_buf_head(0), rx_buf_tail(0)
{
}

void LOT_uart_basic::transmit(const char data[])
{
    while(*data)
    {
        transmit(*data++);
    }
}