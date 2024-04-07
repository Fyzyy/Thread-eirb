#include "global.h"

struct_thread_t main_thread = {
    .id = &main_thread,
    .context = {0},
    .start_routine = NULL,
    .arg = NULL,
    .retval = NULL,
};

struct_thread_t *current_thread = &main_thread;
int cancel_current = 0;

struct thread_list ready_threads = STAILQ_HEAD_INITIALIZER(ready_threads);
struct thread_list finished_threads = STAILQ_HEAD_INITIALIZER(finished_threads);


/*Timer starting*/
long period_t;
struct itimerval timer;
void start_time() {
	setitimer(ITIMER_VIRTUAL,&timer,0);
}

void stop_time() {
	setitimer(ITIMER_VIRTUAL, 0, 0);
}


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
    struct_thread_t *thread = STAILQ_FIRST(list);
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
