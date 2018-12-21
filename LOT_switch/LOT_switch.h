/**
 * @file LOT_switch.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 스위치
 */

#ifndef _LOT_SWITCH_H_
#define _LOT_SWITCH_H_

#include <inttypes.h>

class LOT_switch {
public:
    inline LOT_switch( volatile uint8_t &_ddr, volatile uint8_t &_port, const uint8_t _pin_num )
        : ddr( _ddr )
        , port( _port )
        , pin_num( _pin_num )
    {
    }
    inline void setup( void ) { ddr |= 1 << pin_num; }
    inline void on( void ) { port |= 1 << pin_num; }
    inline void off( void ) { port &= ~( 1 << pin_num ); }
    inline void toggle( void ) { port ^= 1 << pin_num; }

private:
    volatile uint8_t &ddr;
    volatile uint8_t &port;
    const uint8_t     pin_num;
};

class LOT_active_low_switch {
public:
    inline LOT_active_low_switch( volatile uint8_t &_ddr,
                                  volatile uint8_t &_port,
                                  const uint8_t     _pin_num )
        : ddr( _ddr )
        , port( _port )
        , pin_num( _pin_num )
    {
    }
    inline void setup( void ) { ddr |= 1 << pin_num; }
    inline void on( void ) { port &= ~( 1 << pin_num ); }
    inline void off( void ) { port |= 1 << pin_num; }
    inline void toggle( void ) { port ^= 1 << pin_num; }

private:
    volatile uint8_t &ddr;
    volatile uint8_t &port;
    const uint8_t     pin_num;
};

#endif    // _LOT_SWITCH_H_