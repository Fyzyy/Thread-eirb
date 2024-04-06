#include "global.h"

// Lance le thread sur la routine associée
static void thread_start(void *arg) {
    struct_thread_t *thread = (struct_thread_t *)arg;
    thread->retval = thread->start_routine(thread->arg);
    thread->finished = 1;
}

int thread_join(thread_t thread, void **retval){
    struct_thread_t *thread_to_join = id_to_struct(thread);

    if (thread_to_join == NULL) {
        printf("Thread to join not found\n");
        return -1;
    }

    getcontext(&thread_to_join->context);
    makecontext(&thread_to_join->context, (void (*)())thread_start, 1, thread_to_join);

    if (STAILQ_EMPTY(&running_threads))
        STAILQ_INSERT_HEAD(&running_threads, &main_thread, entries);
    STAILQ_INSERT_TAIL(&running_threads, thread_to_join, entries);

    while (!thread_to_join->finished) {
        thread_yield();
    }

    if (retval != NULL) {
        *retval = thread_to_join->retval;
    }

    return 0;
}
