#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

void thread_exit(void *retval) {

    current_thread->retval = retval;    
    current_thread->finished = 1;
    thread_join(current_thread->id, NULL);

}
