#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

void thread_exit(void *retval) {
    current_thread->retval = retval;
    current_thread->finished = 1;

    // Find the current thread in the storage and remove it
    for (size_t i = 0; i < storage_size; i++) {
        if (storage[i] == current_thread) {
            for (size_t j = i; j < storage_size - 1; j++) {
                storage[j] = storage[j + 1];
            }
            break;
        }
    }
    storage_size--;

    // Remove the current thread from the running_threads list
    STAILQ_REMOVE(&running_threads, current_thread, struct_thread_t, entries);

    // Switch to the context of the thread's successor
    setcontext(current_thread->context.uc_link);
}
