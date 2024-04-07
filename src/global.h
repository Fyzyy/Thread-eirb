#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stddef.h>
#include <ucontext.h>
#include <sys/queue.h>
#include <stdlib.h>

#include "thread.h"

#define STACK_SIZE 8192
#define MAX_THREADS 100

typedef struct struct_thread_t{
    thread_t id; // Identifiant du thread
    ucontext_t context; // Contexte du thread
    void *(*start_routine)(void *); // Fonction à exécuter
    void *arg; // Argument de la fonction
    void *retval; // Valeur de retour
    int finished; // Indicateur de terminaison du thread
    STAILQ_ENTRY(struct_thread_t) entries;

} struct_thread_t;

STAILQ_HEAD(thread_list, struct_thread_t);
extern struct thread_list running_threads;
extern struct thread_list finished_threads;


extern struct_thread_t main_thread;
extern struct_thread_t *current_thread;

#endif // GLOBAL_H