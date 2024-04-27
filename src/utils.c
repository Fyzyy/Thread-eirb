#include "global.h"

struct_thread_t * main_thread;

struct_thread_t *current_thread;

int main_thread_deleted = 0;

__attribute__((constructor))
void initialize_main_thread() {
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 10000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 10000;
    
    main_thread = (struct_thread_t *) malloc(sizeof(struct_thread_t));

    main_thread->id = main_thread;

    getcontext(&main_thread->context);

    main_thread->context.uc_stack.ss_size = STACK_SIZE;
    main_thread->context.uc_stack.ss_sp = malloc(main_thread->context.uc_stack.ss_size);
    int valgrind_stackid = VALGRIND_STACK_REGISTER(main_thread->context.uc_stack.ss_sp, main_thread->context.uc_stack.ss_sp + main_thread->context.uc_stack.ss_size);
    main_thread->stack_id = valgrind_stackid;

    current_thread = main_thread;

    start_time();
    // signal(SIGVTALRM, (void (*)(int)) scheduler);

}

__attribute__((destructor))
void destruct_main_thread() {
    VALGRIND_STACK_DEREGISTER(current_thread->stack_id);
    free(current_thread->context.uc_stack.ss_sp);
    free(current_thread);
}

/*Timer starting*/
long period_t;
struct itimerval timer;
void start_time() {
	setitimer(ITIMER_VIRTUAL,&timer,0);
}

void stop_time() {
	setitimer(ITIMER_VIRTUAL, 0, 0);
}

struct thread_list finished_threads = STAILQ_HEAD_INITIALIZER(finished_threads);
int cancel_current = 0;

struct thread_list ready_threads = STAILQ_HEAD_INITIALIZER(ready_threads);


struct_thread_t* search_by_id(struct thread_list* list ,thread_t id) {
    struct_thread_t *thread = NULL;
    STAILQ_FOREACH(thread, list, entries) {
        if (thread->id == id) {
            return thread;
        }
    }
    return thread;
}

void enqueue(struct thread_list* list, struct_thread_t* thread) {
    STAILQ_INSERT_TAIL(list, thread, entries);
}

struct_thread_t* dequeue(struct thread_list* list) {
    if (STAILQ_EMPTY(list)) {
        return NULL;
    }
    struct_thread_t *thread = (struct_thread_t*) STAILQ_FIRST(list);
    STAILQ_REMOVE_HEAD(list, entries);
    return thread;
}

int remove_thread(struct thread_list* list, struct_thread_t* thread) {
    if (search_by_id(list, thread->id) == NULL) {
        return -1;
    }

    STAILQ_REMOVE(list, thread, struct_thread_t, entries);
    return 0;
}

int size(struct thread_list* list) {
    struct_thread_t *thread = NULL;
    int i = 0;
    STAILQ_FOREACH(thread, list, entries) {
        i++;
    }
    return i;
}

int is_empty(struct thread_list* list) {
    return STAILQ_EMPTY(list);
}

void print_queue(struct thread_list* list) {
    struct_thread_t *thread = NULL;
    printf("Queue:\n");
    STAILQ_FOREACH(thread, list, entries) {
        printf("Thread %p\n", thread->id);
    }
}
