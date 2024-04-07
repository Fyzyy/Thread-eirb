#include <stdlib.h>
#include <ucontext.h>
#include <sys/time.h>
#include <signal.h>

#include "global.h"

int init = 0;

void init_thread() {
    init = 1;

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 10000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 10000;
    start_time();
    signal(SIGVTALRM, (void (*)(int)) scheduler);

    current_thread = &main_thread;

}

// Lance le thread sur la routine associée
static void thread_start(void *arg) {
    struct_thread_t *thread = (struct_thread_t *)arg;
    thread_exit(thread->start_routine(thread->arg));
}

int thread_create(thread_t *newthread, void *(*start_routine)(void *), void *arg) {

    if (init == 0) {
        init_thread();
    }

    struct_thread_t *new_struct_thread = (struct_thread_t *) malloc(sizeof(struct_thread_t));

    if (new_struct_thread == NULL)
        return -1;

    char *stack;
    stack = (char *) malloc(STACK_SIZE);
    if (stack == NULL)
        return -1;
    
    getcontext(&new_struct_thread->context);
    new_struct_thread->id = *newthread;
    new_struct_thread->context.uc_stack.ss_sp = stack;
    new_struct_thread->context.uc_stack.ss_size = STACK_SIZE;
    new_struct_thread->context.uc_link = &main_thread.context;
    new_struct_thread->start_routine = start_routine;
    new_struct_thread->arg = arg;
    new_struct_thread->retval = NULL;

    enqueue(&ready_threads, new_struct_thread);

    makecontext(&new_struct_thread->context, (void (*)(void)) thread_start, 1, new_struct_thread);
 
    return 0;
}