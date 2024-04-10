#include "global.h"

thread_t thread_self() {
    if (init == 0) {
        init_thread();
    }
    return current_thread->id;
}