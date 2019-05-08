/**
 * @file LOT_uart0.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief AVR UART0 인터럽트 서비스 루틴
 */

#include <LOT_uart.h>

#if !defined( __AVR_ATmega328P__ )
#    warning "Untested device"
#endif

#if defined( _USE_LOT_UART_0_ )

LOT_uart uart0( UCSR0A, UCSR0B, UCSR0C, UBRR0L, UBRR0H, UDR0 );

#    if defined( USART_RX_vect )
ISR( USART_RX_vect )
#    elif defined( USART0_RX_vect )
ISR( USART0_RX_vect )
#    endif
{
    uart0.rx_isr();
}

#    if defined( USART_UDRE_vect )
ISR( USART_UDRE_vect )
#    elif defined( USART0_UDRE_vect )
ISR( USART0_UDRE_vect )
#    endif
{
    uart0.udre_isr();
}

#endif /// _USE_LOT_UART_0_