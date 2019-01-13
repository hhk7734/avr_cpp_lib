#include <avr/io.h>
#include <util/delay.h>

#include "LOT_uart0.h"
#include "LOT_spi0.h"

class LOT_active_low_switch {
public:
    inline void setup( void ) { DDRB |= _BV( DDB2 ); }
    inline void on( void ) { PORTB &= ~_BV( PB2 ); }
    inline void off( void ) { PORTB |= _BV( PB2 ); }
    inline void toggle( void ) { PORTB ^= _BV( PB2 ); }
};

LOT_active_low_switch mpu;

int main( void )
{
    uart0.setup( 57600 );
    uart0.transmit_CR_NL( "hello" );

    spi0.setup( LOT_SPI_SCK_DIV_128 );
    mpu.setup();

    mpu.on();
    spi0.transmit( 0x6B, 1 );
    mpu.off();

    _delay_ms( 100 );

    for( ;; )
    {
        uint16_t temp;
        uint8_t  data[2];
        mpu.on();
        spi0.receive( 0x3B, data, 2 );
        mpu.off();
        temp = ( data[0] << 8 ) | data[1];
        uart0.transmit_CR_NL( temp );
        _delay_ms( 200 );
    }
}