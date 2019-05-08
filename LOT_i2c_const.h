/**
 * @file LOT_i2c_const.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 */

#include <avr/io.h>

#define LOT_I2C_DEFAULT_CLOCK 400000UL
#define LOT_I2C_TIME_OUT 255

/// TWSR
const uint8_t LOT_TWS_MASK = 0xF8;
const uint8_t LOT_TWPS1    = TWPS1;
const uint8_t LOT_TWPS0    = TWPS0;

/// TWCR
/// interrupt flag
const uint8_t LOT_TWINT = TWINT;
/// ACK
const uint8_t LOT_TWEA = TWEA;
/// start
const uint8_t LOT_TWSTA = TWSTA;
/// stop
const uint8_t LOT_TWSTO = TWSTO;
/// wirte collision
const uint8_t LOT_TWWC = TWWC;
/// i2c enable
const uint8_t LOT_TWEN = TWEN;
/// interrupt enable
const uint8_t LOT_TWIE = TWIE;