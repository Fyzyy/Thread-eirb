#include "global.h"

void scheduler () {
    struct_thread_t *prev , *next = NULL;
    stop_time();

    if (is_empty(&ready_threads)) {
        //printf("No thread present in ready queue\n");
        return ;
    }

    prev = current_thread;

    if (cancel_current == 0) {
        enqueue(&ready_threads, prev);
        //print_queue(&ready_threads);
    } 
    else {
        cancel_current = 0;
        enqueue(&finished_threads, prev);
    }

    next = dequeue(&ready_threads);

    if (next == NULL) {
		printf("No thread present in ready queue\n");
		exit(EXIT_SUCCESS);
	} /*No thread present in queue*/
    
    current_thread = next;
    // printf("ready ");
    // print_queue(&ready_threads);
    // printf("finished ");
    // print_queue(&finished_threads);
    start_time();

    if (prev->context.uc_stack.ss_sp == NULL) {
        printf("Error while getting stack pointer prev\n");
        exit(EXIT_FAILURE);
    } /*Error while getting stack pointer*/

    if (next->context.uc_stack.ss_sp == NULL) {
        printf("Error while getting stack pointer next\n");
        exit(EXIT_FAILURE);
    } /*Error while getting stack pointer*/
    
    //printf("Switching from %p to %p\n", prev->id, next->id);
    if (swapcontext(&(prev->context), &(next->context)) == -1 ) {
		printf("Error while swap context\n"); /*calling the next thread*/
	}
}

int thread_yield(void) {
    
    stop_time();
    start_time();
    scheduler();
    return 0;
}