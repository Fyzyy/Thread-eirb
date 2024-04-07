
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"


int thread_cancel(thread_t thread) {
    struct_thread_t *thread_to_cancel = search_by_id(&ready_threads, thread);

    if (thread_to_cancel == NULL) {
        return -1;
    }

    else {
        remove_thread(&ready_threads, thread_to_cancel);
        free(thread_to_cancel->context.uc_stack.ss_sp);
        free(thread_to_cancel);
    }

    if (thread_to_cancel == thread_self()) {
        cancel_current = 1;
        scheduler();
        return 0;
    }

    return 0;
}


__attribute__ ((__noreturn__)) void thread_exit(void *retval) {

    current_thread->retval = retval;

    enqueue(&finished_threads, current_thread);
    remove_thread(&ready_threads, current_thread);
    thread_cancel(thread_self());
    __builtin_unreachable();
}
