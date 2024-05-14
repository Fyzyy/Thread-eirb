#include <stdlib.h>
#include <ucontext.h>
#include <sys/time.h>
#include <signal.h>

#include "global.h"

// Lance le thread sur la routine associée
static void thread_start(void *arg) {
    struct_thread_t *thread = (struct_thread_t *)arg;
    void * retval = thread->start_routine(thread->arg);
    //stop_time();
    thread_exit(retval);
}

int thread_create(thread_t *newthread, void *(*start_routine)(void *), void *arg) {
    //stop_time();

    struct_thread_t *new_struct_thread = (struct_thread_t *) malloc(sizeof(struct_thread_t));
    if (new_struct_thread == NULL)
        return -1;

    getcontext(&new_struct_thread->context);
    new_struct_thread->id = new_struct_thread;
    *newthread = new_struct_thread->id;
    new_struct_thread->context.uc_stack.ss_size = STACK_SIZE;

    new_struct_thread->context.uc_stack.ss_sp = malloc(new_struct_thread->context.uc_stack.ss_size);
    int valgrind_stackid = VALGRIND_STACK_REGISTER(new_struct_thread->context.uc_stack.ss_sp, new_struct_thread->context.uc_stack.ss_sp + new_struct_thread->context.uc_stack.ss_size);
    new_struct_thread->stack_id = valgrind_stackid;
    if (new_struct_thread->context.uc_stack.ss_sp == NULL) {
        free(new_struct_thread);
        return -1;
    }

    new_struct_thread->context.uc_link = &current_thread->context;
    new_struct_thread->start_routine = start_routine;
    new_struct_thread->arg = arg;
    new_struct_thread->retval = NULL;

    enqueue(&ready_threads, new_struct_thread);

    //start_time();
    makecontext(&new_struct_thread->context, (void (*)(void)) thread_start, 1, new_struct_thread);
 
    return 0;
}