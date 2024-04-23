
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

__attribute__ ((__noreturn__)) void thread_exit(void *retval) {
    cancel_current = 1;
    current_thread->retval = retval;
    thread_yield();
    __builtin_unreachable();
}
