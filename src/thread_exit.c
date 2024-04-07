#include "global.h"
#include "thread.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__)) void thread_exit(void *retval) {
  // fprintf(stderr, "Exiting thread...\n");
  // assert(retval != NULL);
  // assert(threads.sqh_first != NULL);
  // threads.sqh_first->thread.ret_val = retval;
  // if (SIMPLEQ_EMPTY(&threads)) {
  //   fprintf(stderr, "Error: queue is empty\n");
  // }
  //
  // else {
  //   node_t *first_node = SIMPLEQ_FIRST(&threads);
  //   if (first_node == SIMPLEQ_END(queue)) {
  //     fprintf(stderr, "Error: first thread same as last\n");
  //   } else {
  //     SIMPLEQ_REMOVE_HEAD(&threads, nodes);
  //     SIMPLEQ_INSERT_TAIL(&threads, first_node, nodes);
  //   }
  // }
  //
  // fprintf(stderr, "Thread Exited.\n");
}

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
    ucontext_t current_context;
    getcontext(&current_context);

    makecontext(&(current_context), (void (*)(void))thread_mutex_lock, 1,
                mutex);
  }
  // printf("locked\n");
  return 0;
}

int thread_mutex_unlock(thread_mutex_t *mutex) {
  // printf("Unlocking\n");
  __sync_lock_release(&mutex->dummy);
  return 0;
}
