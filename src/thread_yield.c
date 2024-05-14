#include "global.h"

#include <signal.h>


void scheduler () {

    if (cancel_current == 0) {
        enqueue(&ready_threads, current_thread);
        cancel_current = 0;
    }


    struct_thread_t *prev , *next = NULL;
    prev = current_thread;

    next = dequeue(&ready_threads);

    

    if (next == NULL) {
		next = current_thread;
	} /*No thread present in queue*/
    
    current_thread = next;
    // printf("ready ");
    // print_queue(&ready_threads);
    // printf("finished ");
    // print_queue(&finished_threads);

    if (prev->context.uc_stack.ss_sp == NULL) {
        printf("Error while getting stack pointer prev\n");
        exit(EXIT_FAILURE);
    } /*Error while getting stack pointer*/

    if (next->context.uc_stack.ss_sp == NULL) {
        printf("Error while getting stack pointer next\n");
        exit(EXIT_FAILURE);
    } /*Error while getting stack pointer*/
    
    //printf("Switching from %p to %p\n", prev->id, next->id);
    // start_time();
    
    if (swapcontext(&(prev->context), &(next->context))) {
        printf("Error while switching context\n");
        exit(EXIT_FAILURE);
    } /*Error while switching context*/
}

int thread_yield(void) {
    // stop_time();
    scheduler();
    //stop_time();
    return 0;
}
