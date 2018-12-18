/**
 * @file cxa_virtual.cpp
 * @author Hyeon-ki, Hong (hhk7734@gmail.com)
 * @brief 가상 함수관련 문제 해결을 위한 소스
 */

#include <stdlib.h>

extern "C" void __cxa_pure_virtual( void ) __ATTR_NORETURN__;
extern "C" void __cxa_deleted_virtual( void ) __ATTR_NORETURN__;

void __cxa_pure_virtual( void ) { abort(); }

void __cxa_deleted_virtual( void ) { abort(); }