#include <stdlib.h>
#include <ucontext.h>

#include "global.h"

struct thread_list running_threads = STAILQ_HEAD_INITIALIZER(running_threads);

struct_thread_t* storage[MAX_THREADS];
size_t storage_size = 0;

int thread_create(thread_t *newthread, void *(*start_routine)(void *), void *arg) {

    struct_thread_t *new_struct_thread = (struct_thread_t *) malloc(sizeof(struct_thread_t));

    if (new_struct_thread == NULL)
        return -1;

    char *stack;
    stack = (char *) malloc(STACK_SIZE);

    if (stack == NULL)
        return -1;

    new_struct_thread->id = *newthread;
    new_struct_thread->context.uc_stack.ss_sp = stack;
    new_struct_thread->context.uc_stack.ss_size = STACK_SIZE;
    new_struct_thread->context.uc_link = &current_thread->context;
    new_struct_thread->start_routine = start_routine;
    new_struct_thread->arg = arg;
    new_struct_thread->finished = 0;
    new_struct_thread->retval = NULL;

    storage[storage_size++] = new_struct_thread;



    return 0;
}