#include "global.h"

struct_thread_t* search_by_id(struct thread_list* list ,thread_t id) {
    struct_thread_t *thread = NULL;
    STAILQ_FOREACH(thread, list, entries) {
        if (thread->id == id) {
            return thread;
        }
    }
    return thread;
}

int thread_join(thread_t thread, void **retval){
    struct_thread_t *thread_to_join = search_by_id(&running_threads ,thread);

    // Already finished
    if (thread_to_join == NULL) {
        struct_thread_t *thread_to_end = search_by_id(&finished_threads ,thread);
        
        if (thread_to_end == NULL)
            return -1;

        if (retval != NULL) {
            *retval = thread_to_end->retval;
        }
        
        STAILQ_REMOVE(&finished_threads, thread_to_end, struct_thread_t, entries);
        free(thread_to_end->context.uc_stack.ss_sp);
        free(thread_to_end);
        return 0;

    }

    while (!thread_to_join->finished) {
        thread_yield();
    }

    if (thread_to_join->finished == 1) {
       
        // Remove the current thread from the running_threads list
        STAILQ_REMOVE(&running_threads, thread_to_join, struct_thread_t, entries);

        // Add the current thread to the finished_threads list
        STAILQ_INSERT_TAIL(&finished_threads, thread_to_join, entries);
        
        // Switch to the context of the thread's successor
        setcontext(thread_to_join->context.uc_link);
    }

    return 0;
}
