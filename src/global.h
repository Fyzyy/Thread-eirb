#ifndef GLOBAL_H
#define GLOBAL_H

#include <stddef.h>
#include <ucontext.h>
#include <sys/queue.h>
#include <stdlib.h>

#include "thread.h"

#define STACK_SIZE 8192

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
extern struct thread_list threads;


extern struct_thread_t main_thread;
extern struct_thread_t *current_thread;

struct_thread_t* id_to_struct(thread_t id);

#endif // GLOBAL_H