#include "global.h"

int thread_join(thread_t thread, void **retval){
    struct_thread_t *thread_to_join = id_to_struct(thread);
    if (thread_to_join == NULL) {
        return -1;
    }

    while (!thread_to_join->finished) {
        thread_yield();
    }

    if (retval != NULL) {
        *retval = thread_to_join->retval;
    }

    return 0;
}
