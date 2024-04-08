#include "global.h"
#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

int thread_mutex_init(thread_mutex_t *mutex) {
  if (mutex == NULL) {
    return -1;
  }
  mutex->dummy = 0;
  __sync_synchronize();
  return 0;
}

int thread_mutex_destroy(thread_mutex_t *mutex) {
  if (mutex == NULL) {
    return -1;
  }
  // free(mutex);
  return 0;
}

int thread_mutex_lock(thread_mutex_t *mutex) {
  while (__sync_lock_test_and_set(&(mutex->dummy), 1)) {
    // ucontext_t current_context;
    // getcontext(&current_context);
    //
    // makecontext(&(current_context), (void (*)(void))thread_mutex_lock, 1,
    // mutex,
    //             NULL);
    sched_yield();
  }
  // printf("locked\n");
  return 0;
}

int thread_mutex_unlock(thread_mutex_t *mutex) {
  // printf("Unlocking\n");
  __sync_lock_release(&mutex->dummy);
  return 0;
}
