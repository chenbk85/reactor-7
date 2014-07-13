#include "../data_structures/heap/heap.h"
#include "event_handler.h"
#include <sys/time.h>
/* NOTE it'd be cool to have the functions be their own struct and let the impl decide things like _handler_heap */
  /* pure handler functions will come once we're doing actual I/O to the user 
     for now, we're only interested in timers */
typedef struct _reactor reactor;
struct _reactor {
  heap *_handler_heap;
  void (*start)(reactor *self);
  void (*stop)(reactor *self);
  /* useful for providing a generic interface around the handler_heap and directly adding events */
  void (*add_event)(reactor *self, event_handler *handler); 
  void (*start_timer)(reactor *self, event_handler *handler, time_t delta);
  void (*stop_timer)(reactor *self, event_handler *handler);
  // int (*stop_timer_by_id)(reactor *self, long timer_id);
  int (*handle_events)(reactor *self);
};

/* NOTES

event cancellation can be handled in one of the following ways:
  the scheduling code will hold a pointer to the actual timed event which it will then cancel on its own - the reactor only holds on to the heap
  the reactor dishes out IDs which it uses to map the heaped events to a hashtable so that the outside code only needs an ID
with IDs, the reactor could actually stick to having a simple array as opposed to a larger hashtable...
the only real difference is wheter you pass around a number or a pointer.  passing around a number is theoretically safer
i like passing the pointer b/c it avoids duplicating data, thing is that you'll need to put in ref counting for safety
*/
