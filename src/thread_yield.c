#include "global.h"

int thread_yield(void) {

    if (STAILQ_EMPTY(&threads)) {
        current_thread = &main_thread;
        return 0;
    }

    struct_thread_t *next_thread = NULL;
    struct_thread_t *thread = current_thread;

    STAILQ_FOREACH(next_thread, &threads, entries) {
        if (next_thread->id == thread->id) {
            break;
        }
    }

    if (next_thread == NULL) {
        next_thread = STAILQ_FIRST(&threads);
        return 0;
    }

    STAILQ_REMOVE(&threads, next_thread, struct_thread_t, entries);
    STAILQ_INSERT_TAIL(&threads, next_thread, entries);

    swapcontext(&(thread->context), &(next_thread->context));

    return 0;
}