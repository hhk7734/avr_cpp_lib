/**
 * @file LOT_macro.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 자주 사용되는 타입, 함수 등
 */

#ifndef _LOT_MACRO_H_
#define _LOT_MACRO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/io.h>

/// 상태 반환 타입
typedef enum
{
    LOT_OK = 0x00,
    LOT_ERROR,
    LOT_BUSY,
    LOT_TIMEOUT
} LOT_status_typedef;

#ifndef __cplusplus
#    if !defined( __bool_true_false_are_defined )
typedef enum
{
    false = 0x00,
    true
} bool;
#    endif
#endif

/**
 * @brief 비트 번호에 대응하는 이진수 값 반환
 * @param uint8_t num_bit 비트 번호
 * @return uint8_t 비트 번호에 대응하는 이진수 값
 */
static inline uint8_t _bv( uint8_t num_bit ) { return ( 1 << num_bit ); }
__attribute__( ( always_inline ) )

/**
 * @brief 레지스터의 비트를 1 로 변경
 * @param uint8_t *reg 레지스터 주소
 * @param uint8_t num_bit 변경하고싶은 비트 번호
 */
static inline void
    _set_bit( volatile uint8_t *reg, uint8_t num_bit )
{
    *reg |= _bv( num_bit );
}
__attribute__( ( always_inline ) )

/**
 * @brief 레지스터의 비트를 0 으로 변경
 * @param uint8_t *reg 레지스터 주소
 * @param uint8_t num_bit 변경하고싶은 비트 번호
 */
static inline void
    _clear_bit( volatile uint8_t *reg, uint8_t num_bit )
{
    *reg &= ~_bv( num_bit );
}
__attribute__( ( always_inline ) )

/**
 * @brief 레지스터의 비트가 1 인지 확인
 * @param uint8_t *reg 레지스터 주소
 * @param uint8_t num_bit 확인하고싶은 비트 번호
 * @return uint8_t 1 이면 _bv(num_bit) 반환 아니면 0 반환
 */
static inline uint8_t
    _is_set( volatile uint8_t *reg, uint8_t num_bit )
{
    return *reg & _bv( num_bit );
}
__attribute__( ( always_inline ) )

/**
 * @brief 레지스터의 비트가 0 인지 확인
 * @param uint8_t *reg 레지스터 주소
 * @param uint8_t num_bit 확인하고싶은 비트 번호
 * @return uint8_t 0 이면 1 반환, 아니면 0 반환
 */
static inline uint8_t
    _is_clear( volatile uint8_t *reg, uint8_t num_bit )
{
    return !_is_set( reg, num_bit );
}
__attribute__( ( always_inline ) )

#ifdef __cplusplus
}
#endif
#endif    // _LOT_MACRO_H_