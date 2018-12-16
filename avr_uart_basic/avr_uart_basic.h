/**
 * @file avr_uart_basic.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief UART 통신 구축
 */

#ifndef _AVR_UART_BASIC_
#define _AVR_UART_BASIC_

#include <inttypes.h>

class LOT_uart_basic
{
public:
    LOT_uart_basic( volatile uint8_t &_ucsra, volatile uint8_t &_ucsrb,
    volatile uint8_t &_ucsrc, volatile uint8_t &_ubrrh,
    volatile uint8_t &_ubrrl, volatile uint8_t &_udr );

    virtual void rx_isr(void) = 0;
    virtual void udre_isr(void) = 0;

    virtual void setup(const uint32_t baud_rate) = 0;
    virtual uint8_t receive(uint8_t &one_byte) = 0;
    virtual void transmit(const uint8_t data) = 0;

    inline uint8_t receive(void){uint8_t temp; receive(temp); return temp;}
    
    inline void transmit(const char data) {transmit(static_cast<uint8_t>(data));}
    void transmit(const char data[]);

protected:
    volatile uint8_t &ucsra;
    volatile uint8_t &ucsrb;
    volatile uint8_t &ucsrc;
    volatile uint8_t &ubrrh;
    volatile uint8_t &ubrrl;
    volatile uint8_t &udr;

    volatile uint8_t tx_buf_head;
    volatile uint8_t tx_buf_tail;
    volatile uint8_t rx_buf_head;
    volatile uint8_t rx_buf_tail;
};

#endif // _AVR_UART_BASIC_