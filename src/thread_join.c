#include "global.h"
#include "thread.h"
#include <errno.h>
#include <stdio.h>

#include <time.h>
#define TIMEOUT_SECONDS 5

int thread_join(thread_t thread, void **retval) {

  time_t start_time_mutex = time(NULL);
  while (search_by_id(&finished_threads, thread) == NULL) {
    if (difftime(time(NULL), start_time_mutex) > TIMEOUT_SECONDS) {
      if (is_empty(&finished_threads)) {
        cancel_current = 1;
        dead = 1;
        thread_yield();
      }
    } else {

      thread_yield();
    }
  }

  struct_thread_t *thread_to_join = search_by_id(&finished_threads, thread);

  if (remove_thread(&finished_threads, thread_to_join) == 0) {

    if (retval != NULL)
      *retval = thread_to_join->retval;

    free_thread(thread_to_join);
    if (size(&ready_threads) > 0) {
      struct_thread_t *thread_to_free = dequeue(&ready_threads);
      free_thread(thread_to_free);
      thread_yield();
    }

    if (dead) {
      dead = 0;
      return EDEADLK;
    }
    return 0;
  }
  if (dead) {
    dead = 0;
    return EDEADLK;
  }
  return -1;
}
