
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

    if (thread_self() == &main_thread) {
        VALGRIND_STACK_DEREGISTER(main_thread.stack_id);
        free(main_thread.context.uc_stack.ss_sp);
    }

    current_thread->retval = retval;
    enqueue(&finished_threads, current_thread);
    struct_thread_t * former_thread = current_thread;
    int res = thread_cancel(thread_self());
    scheduler();
    if (res == 0) {
        VALGRIND_STACK_DEREGISTER(former_thread->stack_id);
        free(former_thread->context.uc_stack.ss_sp);
        free(former_thread);
    }
    __builtin_unreachable();
}
