#include "global.h"

thread_t thread_self() {
    return current_thread->id;
}