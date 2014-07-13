#include "simple_reactor.h"

// TODO: I don't get what these start/stop functions should do... look into that
/* these are constructor/destructor functions, i'll do them later */
static void simple_reactor_start(reactor *self)
{
}

static void simple_reactor_stop(reactor *self)
{
  heap_free(self->_handler_heap);
  free(self);
}

static void simple_reactor_start_timer(reactor *self, event_handler *handler, time_t delta)
{
  time_t now = time(NULL);
  handler->valid_time = (now + delta);
  handler->interval = delta;
  heap_add(self->_handler_heap, handler, handler->valid_time);
}

static void simple_reactor_stop_timer(reactor *self, event_handler *handler)
{
  handler->valid_time = REACTOR_EVENT_CANCELLED;
}

/*
  This function polls for new events and runs as appropriate.  It returns one of the REACTOR_EVENT macros
  in the reactor header file based on the outcome
*/
int simple_reactor_handle_events(reactor *self)
{
  int retval;
  event_handler *handler = NULL;
  time_t now = time(NULL);
  /* This code currently only manages built in events.  In the future it would also handle I/O as per the reactor paper*/
  while (1) {
    handler = (event_handler *)heap_peek(self->_handler_heap);
    if (!handler || handler->valid_time > now) {
      /* no events are ready - no need to free the handler b/c we were only peeking */
      return REACTOR_EVENT_NOEVENT;
    }
    else if (handler->valid_time == REACTOR_EVENT_CANCELLED) {
      fprintf(stderr, "cancelled event, gtfo\n"); 
      handler = heap_pop(self->_handler_heap);
      event_handler_free(handler);
    }
    else {
      break;
    }
  }
  handler = heap_pop(self->_handler_heap);
  if (handler->valid_time == REACTOR_EVENT_CANCELLED) {
    // maybe this? TODO: figure out TRWTDI 
    retval = REACTOR_EVENT_CANCELLED;
  }
  else if (handler->valid_time <= now) {
    retval = handler->callback(handler); 
    if (retval == REACTOR_EVENT_RESCHEDULE) {
      self->start_timer(self, handler, handler->interval);
      return retval;
    }
  }

  /* TODO we prbly need ref counting */
  /* we popped - better free the now useless pointer */
  event_handler_free (handler);
  return retval;
}
/*  
    This function assembles a simple_reactor.
    The ASSIGN_IMPL_FUNC macro will assign the appropriate 'simple_reactor' function to the
    reactor struct that gets passed in.
    The caller is expected to allocate *self before passing it in
*/
#define ASSIGN_IMPL_FUNC(__x) self->__x = simple_reactor_##__x
void simple_reactor_new(reactor *self)
{
  ASSIGN_IMPL_FUNC (start); // start the loop
  ASSIGN_IMPL_FUNC (stop); // stop the loop
  ASSIGN_IMPL_FUNC (start_timer);
  ASSIGN_IMPL_FUNC (stop_timer);
  //ASSIGN_IMPL_FUNC (stop_timer_by_id); //make this NULL
  ASSIGN_IMPL_FUNC (handle_events); //make this happen as event loop
  self->_handler_heap = (heap *)heap_new(100);
}