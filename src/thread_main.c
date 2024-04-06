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

struct_thread_t* id_to_struct(thread_t id) {
    struct_thread_t *thread;
    STAILQ_FOREACH(thread, &threads, entries) {
        if (thread->id == id) {
            return thread;
        }
    }
    return NULL;
}
