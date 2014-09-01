#include "simple_reactor.h"
#include <unistd.h>

static void simple_reactor_start(reactor *self)
{
    while(self->teardown == 0) {
      if (self->handle_IO) {
        self->handle_IO(self);
      }
      self->handle_events(self);
      usleep(500000);
    }
    /* runs when reactor stops */
    heap_free(self->_handler_heap);
    free(self);
}

static int set_teardown (event_handler *arg)
{
  reactor *self = (reactor *)arg->data;
  self->teardown = 1;
  return REACTOR_EVENT_SUCCESS;
}
static void simple_reactor_stop(reactor *self, time_t interval)
{
  if (interval) {
    event_handler *eh = malloc (sizeof *eh);
    eh->callback = set_teardown;
    eh->data = (void *)self;
    self->start_timer(self, eh, interval); 
  }
  else {
    self->teardown = 1;
  }
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
  This function polls for new events and runs as appropriate. Returns the number of events run 
*/
int simple_reactor_handle_events(reactor *self)
{
  int events_handled = 0;
  int retval;
  event_handler *handler = NULL;
  time_t now = time(NULL);
  for (;;) {
    /* run all events scheduled for this moment in time */
    handler = (event_handler *)heap_peek(self->_handler_heap);
    if (!handler || handler->valid_time > now) {
      /* this is the exit point, we've looped through all the current events and are ready to back off */
      break;
    }
    else if (handler->valid_time == REACTOR_EVENT_CANCELLED) {
      fprintf(stderr, "cancelled event, gtfo\n"); 
      handler = heap_pop(self->_handler_heap);
      event_handler_free(handler);
      /* keep looking for events */
    }
    else {
      /* got a legit event */
      handler = heap_pop(self->_handler_heap);
      retval = handler->callback(handler); 
      if (retval == REACTOR_EVENT_RESCHEDULE) {
        self->start_timer(self, handler, handler->interval);
      }
      else {
        event_handler_free (handler);
      }
      /* keep looking for events */
    }
    events_handled++;
  }
    /* TODO we prbly need ref counting */
  return events_handled;
}

/*  
    This function assembles a simple_reactor.
    The ASSIGN_IMPL_FUNC macro will assign the appropriate 'simple_reactor' function to the
    reactor struct that gets passed in.
    The caller is expected to allocate *self before passing it in
*/
#define ASSIGN_IMPL_FUNC(__x) self->__x = simple_reactor_##__x
void simple_reactor_new(reactor *self, int (*io_callback)(reactor *self) )
{
  ASSIGN_IMPL_FUNC (start); // start the loop
  ASSIGN_IMPL_FUNC (stop); // stop the loop
  ASSIGN_IMPL_FUNC (start_timer);
  ASSIGN_IMPL_FUNC (stop_timer);
  //ASSIGN_IMPL_FUNC (stop_timer_by_id); //make this NULL
  ASSIGN_IMPL_FUNC (handle_events); //make this happen as event loop
  self->handle_IO = io_callback;
  self->_handler_heap = (heap *)heap_new(100);
}
