/**
 * @file LOT_uart_const.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 */

#include <avr/io.h>

/// UCSRA
const uint8_t LOT_RXC  = RXC0;
const uint8_t LOT_TXC  = TXC0;
const uint8_t LOT_UDRE = UDRE0;
const uint8_t LOT_FE   = FE0;
const uint8_t LOT_DOR  = DOR0;
const uint8_t LOT_UPE  = UPE0;
const uint8_t LOT_U2X  = U2X0;
const uint8_t LOT_MPCM = MPCM0;

/// UCSRB
const uint8_t LOT_RXCIE = RXCIE0;
const uint8_t LOT_TXCIE = TXCIE0;
const uint8_t LOT_UDRIE = UDRIE0;
const uint8_t LOT_RXEN  = RXEN0;
const uint8_t LOT_TXEN  = TXEN0;
const uint8_t LOT_UCSZ2 = UCSZ02;
const uint8_t LOT_RXB8  = RXB80;
const uint8_t LOT_TXB8  = TXB80;

/// UCSRC
const uint8_t LOT_UMSEL1 = UMSEL01;
const uint8_t LOT_UMSEL0 = UMSEL00;
const uint8_t LOT_UPM1   = UPM01;
const uint8_t LOT_UPM0   = UPM00;
const uint8_t LOT_USBS   = USBS0;
const uint8_t LOT_UCSZ1  = UCSZ01;
const uint8_t LOT_UCSZ0  = UCSZ00;
const uint8_t LOT_UCPOL  = UCPOL0;