#include <stdlib.h>
#include <ucontext.h>

#include "global.h"



struct thread_list threads;


// Lance le thread sur la routine associée
static void thread_start(void *arg) {
    struct_thread_t *thread = (struct_thread_t *)arg;
    thread->retval = thread->start_routine(thread->arg);
    thread->finished = 1;
    thread_exit(thread->retval);
}

int thread_create(thread_t *newthread, void *(*start_routine)(void *), void *arg) {

    struct_thread_t *new_struct_thread = (struct_thread_t *) malloc(sizeof(struct_thread_t));
    new_struct_thread->id = newthread;

    char *stack;
    stack = (char *) malloc(STACK_SIZE);

    if (stack == NULL)
        return -1;

    getcontext(&new_struct_thread->context);
    new_struct_thread->context.uc_stack.ss_sp = stack;
    new_struct_thread->context.uc_stack.ss_size = STACK_SIZE;
    new_struct_thread->context.uc_link = &main_thread.context;
    new_struct_thread->start_routine = start_routine;
    new_struct_thread->arg = arg;
    new_struct_thread->finished = 0;

    STAILQ_INSERT_TAIL(&threads, &main_thread, entries);

    makecontext(&new_struct_thread->context, (void (*)())thread_start, 1, new_struct_thread);

    return 0;
}