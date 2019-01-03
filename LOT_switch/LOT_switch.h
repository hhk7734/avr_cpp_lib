/**
 * @file LOT_switch.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 스위치
 */

#ifndef _LOT_SWITCH_H_
#define _LOT_SWITCH_H_

#include <avr/io.h>

/// 멤버 변수로 레지스터를 설정하면 전역변수로 취급하기 때문에 최적화가 이루어지지 않음
/// 전역 변수로 선언과 동시에 초기화 하는 경우에만 최적화가 됨
volatile uint8_t &ddr;
volatile uint8_t &port;
const uint8_t     pin_num;

class LOT_switch {
public:
    inline void setup( void ) { ddr |= _BV( pin_num ); }
    inline void on( void ) { port |= _BV( pin_num ); }
    inline void off( void ) { port &= ~_BV( pin_num ); }
    inline void toggle( void ) { port ^= _BV( pin_num ); }
};

class LOT_active_low_switch {
public:
    inline void setup( void ) { ddr |= _BV( pin_num ); }
    inline void on( void ) { port &= ~_BV( pin_num ); }
    inline void off( void ) { port |= _BV( pin_num ); }
    inline void toggle( void ) { port ^= _BV( pin_num ); }
};

#endif    // _LOT_SWITCH_H_