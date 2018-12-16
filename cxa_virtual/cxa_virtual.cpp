#include <stdlib.h>

extern "C" void __cxa_pure_virtual(void) __ATTR_NORETURN__;
extern "C" void __cxa_deleted_virtual(void) __ATTR_NORETURN__;

void __cxa_pure_virtual(void) {
    abort();
}

void __cxa_deleted_virtual(void) {
    abort();
}