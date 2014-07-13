#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include "../simple_reactor.h"

static int test_cb (event_handler *arg)
{
  char *msg = (char *)arg->data;
  fprintf (stderr, "%s fired at %d\n",
    msg, (int)arg->valid_time);
  return REACTOR_EVENT_SUCCESS;
}
int main (void)
{
  int counter, retval, events_fired = 0;
  event_handler *eh, *eh2;
  eh = malloc (sizeof *eh);
  eh2 = malloc (sizeof *eh2);

  reactor *reactor_t = malloc (sizeof *reactor_t);
  time_t now = time(NULL);

  simple_reactor_new(reactor_t);
  memset(eh2, 0, sizeof *eh2); 
  eh2->data = strdup("shouldn't fire");
  eh2->callback = test_cb;

  memset(eh, 0, sizeof *eh);
  eh->data = strdup("I hate constants");
  eh->callback = test_cb;
  fprintf (stderr, "%s and %s started at %d\n",
    (char *)eh->data, (char *) eh2->data, (int)now);
  reactor_t->start_timer(reactor_t, eh2, 7);
  reactor_t->start_timer(reactor_t, eh, 5);
  for (counter = 0; counter < 20; counter++) {
    retval = reactor_t->handle_events(reactor_t);
    if (retval != REACTOR_EVENT_NOEVENT) {
      events_fired++;
    }
    if (retval == REACTOR_EVENT_SUCCESS) {
      fprintf (stderr, "event fired successfully!\n");
      reactor_t->stop_timer(reactor_t, eh2);
    }
    fprintf (stderr, "tick %d : %d\n", counter, retval);
    usleep(500000);
  }
  
  reactor_t->stop(reactor_t);
  if (events_fired == 1) {
    fprintf (stderr, "success has occurred\n");
    return 0;
  }
  else {
    fprintf (stderr, "upfuckage has occurred - %d events fired\n", events_fired);
    return 1;
  }
}
