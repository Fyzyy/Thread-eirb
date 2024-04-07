#include "global.h"

int thread_join(thread_t thread, void **retval){


    while (search_by_id(&finished_threads, thread) == NULL) {
        thread_yield();
    }

    struct_thread_t *thread_to_join = search_by_id(&finished_threads, thread);
        
    if (!remove_thread(&finished_threads, thread_to_join))
        return -1;
    
    if (retval != NULL)
        *retval = thread_to_join->retval;
    
    free(thread_to_join->context.uc_stack.ss_sp);
    free(thread_to_join);

    return 0;


}
