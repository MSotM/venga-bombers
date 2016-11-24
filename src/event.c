#include <stdlib.h>
#include "game.h"

#define EVENT_COUNT 32

static event_t events[EVENT_COUNT];
static uint8_t next_event_index = 0;

void queue_event(event_type_t event_type, uint8_t player_id) {
  event_t *event = &(events[next_event_index]);

  /* We assume the event is unused, so we can just override its values. */
  event->event_type = event_type;
  event->player_id = player_id;

  if (++next_event_index == EVENT_COUNT) {
    next_event_index = 0;
  }
}

event_t *dequeue_event() {
  uint8_t event_index = next_event_index;
  event_t *event;

  do {
    event = &(events[event_index]);
    event_index = (event_index + 1) % EVENT_COUNT;

    if (event->event_type != EVENT_TYPE_PROCESSED) {
      return event;
    }

  } while (event_index != next_event_index);

  return NULL;
}

void free_event(event_t *event) {
  event->event_type = EVENT_TYPE_PROCESSED;
  event->player_id = 0;
}
