#include <time.h>
/* TODO: we've hit the point where these should be masks */
#define REACTOR_EVENT_CANCELLED -1
#define REACTOR_EVENT_NOEVENT   0
#define REACTOR_EVENT_SUCCESS   1
#define REACTOR_EVENT_RESCHEDULE   2
typedef struct _event_handler event_handler;
struct _event_handler {
  void *data; 
  /* 
    time at which the event callback should be fired
    timers can set this in the future, immediate action events can set it to the current time  
    set to REACTOR_EVENT_CANCELLED to cancel 
  */
  time_t valid_time;
  /* if this is an event that should be rescheduled, do so at this interval */
  time_t interval;
  void (*data_destructor_func)(void *data);
  int (*callback)(event_handler *arg);
};

void event_handler_free (event_handler *handler);
