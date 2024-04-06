#include "global.h"

struct_thread_t* policy_fifo(void) {
    struct_thread_t *thread = STAILQ_FIRST(&running_threads);
    STAILQ_REMOVE_HEAD(&running_threads, entries);
    STAILQ_INSERT_TAIL(&running_threads, thread, entries);
    return STAILQ_FIRST(&running_threads);
}

int thread_yield(void) {

    if (STAILQ_FIRST(&running_threads) == &main_thread && STAILQ_NEXT(STAILQ_FIRST(&running_threads), entries) == NULL) {
        //printf("No other thread to run\n");
        return 0;
    }

    struct_thread_t *thread = current_thread;

    current_thread = policy_fifo(); //next thread to run


    //printf("Switching from thread %p to thread %p\n", thread, current_thread);
    swapcontext(&(thread->context), &(current_thread->context));

    return 0;
}