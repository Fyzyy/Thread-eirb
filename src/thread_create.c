#include <stdlib.h>
#include <ucontext.h>

#include "global.h"

struct_thread_t main_thread = {
    .id = &main_thread,
    .context = {0},
    .start_routine = NULL,
    .arg = NULL,
    .retval = NULL,
    .finished = 0
};

struct_thread_t *current_thread = &main_thread;

struct thread_list running_threads = STAILQ_HEAD_INITIALIZER(running_threads);
struct thread_list finished_threads = STAILQ_HEAD_INITIALIZER(finished_threads);



// Lance le thread sur la routine associée
static void thread_start(void *arg) {
    struct_thread_t *thread = (struct_thread_t *)arg;
    thread->retval = thread->start_routine(thread->arg);
}

int thread_create(thread_t *newthread, void *(*start_routine)(void *), void *arg) {

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
    new_struct_thread->context.uc_link = &current_thread->context;
    new_struct_thread->start_routine = start_routine;
    new_struct_thread->arg = arg;
    new_struct_thread->finished = 0;
    new_struct_thread->retval = NULL;

    if STAILQ_EMPTY(&running_threads)
        STAILQ_INSERT_TAIL(&running_threads, &main_thread, entries);
    STAILQ_INSERT_TAIL(&running_threads, new_struct_thread, entries);

    makecontext(&new_struct_thread->context, (void (*)(void)) thread_start, 1, new_struct_thread);
 
    return 0;
}