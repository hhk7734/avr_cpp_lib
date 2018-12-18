#include <stdlib.h>

extern "C" void __cxa_pure_virtual( void ) __ATTR_NORETURN__;
extern "C" void __cxa_deleted_virtual( void ) __ATTR_NORETURN__;

/**
 * @brief 순수 가상 함수를 생성했을 때, 파생 클래스가 함수를 정의하지 않은 경우 문제 처리(?)
 */
void __cxa_pure_virtual( void ) { abort(); }

void __cxa_deleted_virtual( void ) { abort(); }