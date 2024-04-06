#include "thread.h"
#include "queue.h"
#include <ucontext.h>
#include <stddef.h>

struct struct_thread_t {
    thread_t id;
    ucontext_t context;
    int stack_id;
    void *ret_val;
};

typedef struct node
{
    struct struct_thread_t thread;
    SIMPLEQ_ENTRY(node) nodes;
} node_t;

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

STAILQ_HEAD(thread_queue, node_t) threads = STAILQ_HEAD_INITIALIZER(threads);

#endif