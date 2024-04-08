
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"


int thread_cancel(thread_t thread) {
    struct_thread_t *thread_to_cancel = search_by_id(&ready_threads, thread);

    if (thread_to_cancel == NULL) {
        return -1;
    }

    remove_thread(&ready_threads, thread_to_cancel);

    return 0;
}


__attribute__ ((__noreturn__)) void thread_exit(void *retval) {

    current_thread->retval = retval;
    enqueue(&finished_threads, current_thread);
    thread_cancel(thread_self());
    scheduler();
    __builtin_unreachable();
}
