
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

__attribute__ ((__noreturn__)) void thread_exit(void *retval) {
    cancel_current = 1;
    current_thread->retval = retval;
    enqueue(&finished_threads, current_thread);
    remove_thread(&ready_threads, current_thread);
    if (STAILQ_EMPTY(&ready_threads)) {
        exit(EXIT_SUCCESS);
        current_thread->context.uc_link = NULL;
        setcontext(&current_thread->context);
    }
    else {
        thread_yield();
    }
    __builtin_unreachable();
}
