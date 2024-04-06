#include <stdio.h>
#include <stdlib.h>

#include "global.h"

typedef struct {
  thread_mutex_t mutex;
  ucontext_t context;
} custom_thread_mutex_t;

int thread_mutex_init(thread_mutex_t *mutex) {
  if (mutex == NULL) {
    return -1;
  }

  mutex->dummy = 0;
  return 0;
}

int thread_mutex_destroy(thread_mutex_t *mutex) { return 0; }

int thread_mutex_lock(thread_mutex_t *mutex) {
  printf("Locking\n");
  custom_thread_mutex_t *custom_mutex = (custom_thread_mutex_t *)mutex;
  while (__sync_lock_test_and_set(&(custom_mutex->mutex.dummy), 1)) {
    ucontext_t current_context;
    getcontext(&current_context);

    if (custom_mutex->context.uc_link == NULL) {
      makecontext(&(custom_mutex->context), (void (*)(void))thread_mutex_lock,
                  1, mutex);
    }

    swapcontext(&current_context, &(custom_mutex->context));
  }
  printf("Locked\n");
  return 0;
}

int thread_mutex_unlock(thread_mutex_t *mutex) {
  printf("Unlocking\n");
  custom_thread_mutex_t *custom_mutex = (custom_thread_mutex_t *)mutex;
  custom_mutex->mutex.dummy = 0;
  return 0;
}
