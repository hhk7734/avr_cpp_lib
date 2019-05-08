/**
 * @file LOT_uart_const.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 */

#include <avr/io.h>

/// UCSRA
const uint8_t LOT_RXC  = _BV( RXC0 );
const uint8_t LOT_TXC  = _BV( TXC0 );
const uint8_t LOT_UDRE = _BV( UDRE0 );
const uint8_t LOT_FE   = _BV( FE0 );
const uint8_t LOT_DOR  = _BV( DOR0 );
const uint8_t LOT_UPE  = _BV( UPE0 );
const uint8_t LOT_U2X  = _BV( U2X0 );
const uint8_t LOT_MPCM = _BV( MPCM0 );

/// UCSRB
const uint8_t LOT_RXCIE = _BV( RXCIE0 );
const uint8_t LOT_TXCIE = _BV( TXCIE0 );
const uint8_t LOT_UDRIE = _BV( UDRIE0 );
const uint8_t LOT_RXEN  = _BV( RXEN0 );
const uint8_t LOT_TXEN  = _BV( TXEN0 );
const uint8_t LOT_UCSZ2 = _BV( UCSZ02 );
const uint8_t LOT_RXB8  = _BV( RXB80 );
const uint8_t LOT_TXB8  = _BV( TXB80 );

/// UCSRC
const uint8_t LOT_UMSEL1 = _BV( UMSEL01 );
const uint8_t LOT_UMSEL0 = _BV( UMSEL00 );
const uint8_t LOT_UPM1   = _BV( UPM01 );
const uint8_t LOT_UPM0   = _BV( UPM00 );
const uint8_t LOT_USBS   = _BV( USBS0 );
const uint8_t LOT_UCSZ1  = _BV( UCSZ01 );
const uint8_t LOT_UCSZ0  = _BV( UCSZ00 );
const uint8_t LOT_UCPOL  = _BV( UCPOL0 );