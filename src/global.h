#include "thread.h"
#include "queue.h"

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

STAILQ_HEAD(thread_queue, struct_thread_t) threads = STAILQ_HEAD_INITIALIZER(threads);