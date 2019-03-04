
#include <avr/io.h>
#include <util/delay.h>

int main( void )
{
    DDRA |= _BV( DDA5 );

    for ( ;; )
    {
        PORTA ^= _BV( PA5 );
        _delay_us( 100 );
    }
}