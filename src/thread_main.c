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
    struct_thread_t *thread = NULL;
    for (size_t i = 0; i < storage_size; i++) {
        if (storage[i]->id == id) {
            //printf("Found thread %p\n", storage[i]->id);
            thread = storage[i];
            break;
        }
    }
    return thread;
}
