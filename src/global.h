#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <signal.h>
#include <ucontext.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <valgrind/valgrind.h>

#include "thread.h"

#define STACK_SIZE 8192

//STRUCTURE THREAD

typedef struct struct_thread_t{
    thread_t id; // Identifiant du thread
    ucontext_t context; // Contexte du thread
    void *(*start_routine)(void *); // Fonction à exécuter
    void *arg; // Argument de la fonction
    void *retval; // Valeur de retour
    STAILQ_ENTRY(struct_thread_t) entries;
    int stack_id;

} struct_thread_t;

// MAIN THREAD

extern struct_thread_t *current_thread;
extern int cancel_current;
extern int init;
extern int main_thread_deleted;

// TIME

extern struct itimerval timer;
extern long period_t;
extern void start_time(void);
extern void stop_time(void);

// QUEUE

STAILQ_HEAD(thread_list, struct_thread_t);

extern struct thread_list ready_threads;
extern struct thread_list finished_threads;

extern void enqueue(struct thread_list* list, struct_thread_t* thread);
extern struct_thread_t* dequeue(struct thread_list* list);
extern int remove_thread(struct thread_list* list, struct_thread_t* thread);
extern int size(struct thread_list* list);
extern int is_empty(struct thread_list* list);
extern void print_queue(struct thread_list* list);

extern struct_thread_t* search_by_id(struct thread_list* list ,thread_t id);

// THREAD

extern void scheduler(void);
extern struct_thread_t * main_thread __attribute__((section(".data")));

__attribute__((constructor))
void initialize_main_thread();

__attribute__((destructor))
void destruct_main_thread();

#endif // GLOBAL_H
