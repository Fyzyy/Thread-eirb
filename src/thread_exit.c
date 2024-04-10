
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"


int thread_cancel(thread_t thread) {
    STAILQ_REMOVE(&finished_threads, thread, struct_thread_t, entries);

    return 0;
}


__attribute__ ((__noreturn__)) void thread_exit(void *retval) {

    cancel_current = 1;

    current_thread->retval = retval;
    enqueue(&finished_threads, current_thread);
    thread_cancel(thread_self());
    scheduler();
    __builtin_unreachable();
}
