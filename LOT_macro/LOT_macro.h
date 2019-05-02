/**
 * @file LOT_macro.h
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 자주 사용되는 타입, 함수 등
 */

#ifndef _LOT_MACRO_H_
#define _LOT_MACRO_H_

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
#        define __bool_true_false_are_defined
typedef enum
{
    false = 0x00,
    true
} bool;
#    endif
#endif

#ifndef _BV
#    define _BV( bit ) ( 1 << ( bit ) )
#endif

/**
 * @brief 레지스터의 비트를 1 로 변경
 * @param reg 레지스터
 * @param num_bit 변경하고싶은 비트 번호
 */
#define _set_bit( reg, num_bit ) ( reg |= _BV( num_bit ) )

/**
 * @brief 레지스터의 비트를 0 으로 변경
 * @param reg 레지스터
 * @param num_bit 변경하고싶은 비트 번호
 */
#define _clear_bit( reg, num_bit ) ( reg &= ~_BV( num_bit ) )

/**
 * @brief 레지스터의 비트가 1 인지 확인
 * @param reg 레지스터
 * @param num_bit 확인하고싶은 비트 번호
 * @return 1 이면 _BV(num_bit) 반환 아니면 0 반환
 */
#define _is_bit_set( reg, num_bit ) ( reg & _BV( num_bit ) )

/**
 * @brief 레지스터의 비트가 0 인지 확인
 * @param reg 레지스터
 * @param num_bit 확인하고싶은 비트 번호
 * @return 0 이면 1 반환, 아니면 0 반환
 */
#define _is_bit_clear( reg, num_bit ) ( !_is_bit_set( reg, num_bit ) )

#endif    // _LOT_MACRO_H_