#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "thread.h"

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
  return 0;
}

int thread_mutex_lock(thread_mutex_t *mutex) {
  struct_thread_t *prev;

  while (__sync_lock_test_and_set(&(mutex->dummy), 1)) {
    prev = current_thread;
    enqueue(&ready_threads, prev);
    remove_thread(&ready_threads, mutex->current);
    current_thread = mutex->current;
    swapcontext(&(prev->context), &(current_thread->context));
  }

  mutex->current = current_thread;
  return 0;
}

int thread_mutex_unlock(thread_mutex_t *mutex) {
  __sync_lock_release(&mutex->dummy);
  return 0;
}
