#include <stdlib.h>
#include <inttypes.h>
#include "timeout.h"

#define TIMEOUT_HEAP_SIZE 20

typedef struct {
  bool *fired;
  int timeout;
} timeout_t;

typedef struct {
  int size;
  uint8_t count;
  timeout_t *data;
} timeout_heap_t;

static bool heap_lock;
static timeout_heap_t timeout_heap;

static int heap_index;
static int lost_ticks;

void timeout_push(timeout_t timeout);

void timeout_pop();

void timeout_timer_init() {
  lost_ticks = 0;
  heap_lock = false;
  timeout_heap.size = TIMEOUT_HEAP_SIZE;
  timeout_heap.count = 0;
  timeout_heap.data = malloc(sizeof(timeout_t) * TIMEOUT_HEAP_SIZE);
}

void timeout_timer_interrupt() {
  if (heap_lock) {
    lost_ticks++;
  } else if (timeout_heap.count) {
    if (lost_ticks) {
      /* Decrease all timeout counters and update if elapsed */
      for (heap_index = 0; heap_index < timeout_heap.count; heap_index++)
        if ((timeout_heap.data[heap_index].timeout -= lost_ticks) < 1)
          *timeout_heap.data[heap_index].fired = true;
      lost_ticks = 0;
    } else {
      for (heap_index = 0; heap_index < timeout_heap.count; heap_index++)
        if (!--timeout_heap.data[heap_index].timeout)
          *timeout_heap.data[heap_index].fired = true;
    }
  }
}

void set_timeout(bool *fired, int timeout) {
  timeout_push((timeout_t) {fired, timeout});
}

void clear_elapsed_timeouts() {
  while (timeout_heap.count > 0 && timeout_heap.data[0].timeout < 1)
    timeout_pop();
}

void timeout_push(timeout_t timeout) {
  /* Push and Pop will be called from the main loop. Only the timer will
   * wait if it's locked */
  heap_lock = true;
  uint8_t index, parent;

  /* Make sure there is enough space for the timeout */
  if (timeout_heap.count == timeout_heap.size) {
    timeout_heap.size += TIMEOUT_HEAP_SIZE;
    timeout_heap.data = realloc(timeout_heap.data,
                                sizeof(timeout_t) * timeout_heap.size);
  }

  /* Find the right spot to add the timeout and update searched nodes */
  for (index = timeout_heap.count++; index; index = parent) {
    parent = (index - 1) >> 1;
    if (timeout_heap.data[parent].timeout < timeout.timeout)
      break;
    timeout_heap.data[index] = timeout_heap.data[parent];
  }
  timeout_heap.data[index] = timeout;
  heap_lock = false;
}

void timeout_pop() {
  /* Push and Pop will be called from the main loop. Only the timer will
   * wait if it's locked */
  heap_lock = true;
  uint8_t index, swap, other;

  /* Take the last element and update the count */
  timeout_t tmp = timeout_heap.data[--timeout_heap.count];

  /* Reorder all elements */
  for (index = 0; 1; index = swap) {
    swap = (index << 1) + 1;
    if (swap >= timeout_heap.count) break;
    other = swap + 1;
    if (other < timeout_heap.count
        && timeout_heap.data[other].timeout < timeout_heap.data[swap].timeout)
      swap = other;
    if (tmp.timeout < timeout_heap.data[swap].timeout) break;
    timeout_heap.data[index] = timeout_heap.data[swap];
  }
  timeout_heap.data[index] = tmp;
  heap_lock = false;
}
