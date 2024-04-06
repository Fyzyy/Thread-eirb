#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"

void thread_exit(void *retval) {

    thread_t current_id = thread_self();
    struct_thread_t *current_thread = id_to_struct(current_id);

    main_thread.retval = retval;
    main_thread.finished = 1;

    free(current_thread->context.uc_stack.ss_sp); // Libération de la pile
    free(current_thread); // Libération de la structure


    setcontext(&main_thread.context); // Retour au thread principal
    assert(0); // Ne doit jamais arriver
}