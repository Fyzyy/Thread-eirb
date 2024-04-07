#include "global.h"

void scheduler () {
    struct_thread_t *prev , *next = NULL;
    stop_time();

	if ( getcontext(&main_thread.context) == -1 ) {
		printf("Error while getting context\n");
		exit(EXIT_FAILURE);
	}

    prev = current_thread;

    if (!cancel_current) {
        enqueue(&ready_threads, prev);
        //print_queue(&ready_threads);
    } 
    else {
            cancel_current = 0;
    }

    next = dequeue(&ready_threads);

    if (next == NULL) {
		printf("No thread present in ready queue\n");
		exit(EXIT_SUCCESS);
	} /*No thread present in queue*/
    
    current_thread = next;
    start_time();
    
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