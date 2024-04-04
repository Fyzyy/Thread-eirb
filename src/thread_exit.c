#include "thread.h"
#include <stdio.h>
#include <assert.h>
#include "global.h"

__attribute__ ((__noreturn__)) void thread_exit(void *retval) {   
    fprintf(stderr, "Exiting thread...\n");
    assert(retval!=NULL);
    assert(threads.sqh_first!=NULL);
    threads.sqh_first->thread.ret_val = retval;
    if (SIMPLEQ_EMPTY(&threads)) {
        fprintf(stderr, "Error: queue is empty\n");
    }

    else{
        node_t *first_node = SIMPLEQ_FIRST(&threads);
        if (first_node == SIMPLEQ_END(queue)) {
            fprintf(stderr, "Error: first thread same as last\n");
        }
        else{
            SIMPLEQ_REMOVE_HEAD(&threads, nodes);
            SIMPLEQ_INSERT_TAIL(&threads, first_node, nodes);
        }
    }

    fprintf(stderr, "Thread Exited.\n");
}

int thread_mutex_init(__attribute__ ((__unused__)) thread_mutex_t *mutex){return 0;}
int thread_mutex_destroy(__attribute__ ((__unused__)) thread_mutex_t *mutex){return 0;}
int thread_mutex_lock(__attribute__ ((__unused__)) thread_mutex_t *mutex){return 0;}
int thread_mutex_unlock(__attribute__ ((__unused__)) thread_mutex_t *mutex){return 0;}