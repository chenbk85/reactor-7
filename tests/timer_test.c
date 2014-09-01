#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <simple_reactor.h>

typedef struct test_cb_arg {
  char *msg;
  event_handler *eh_to_kill; 
} test_cb_arg;
static reactor *reactor_t = NULL;
static int events_fired = 0;

static int test_cb (event_handler *arg)
{
  test_cb_arg *tca = (test_cb_arg *)arg->data;
  char *msg = (char *)tca->msg;
  fprintf (stderr, "%s fired at %d\n",
    msg, (int)arg->valid_time);
  reactor_t->stop_timer(reactor_t, tca->eh_to_kill);
  events_fired++;
  fprintf (stderr, "%d events fired\n", events_fired);
  reactor_t->stop(reactor_t, 5);
  return REACTOR_EVENT_SUCCESS;
}

int run_results()
{
  if (events_fired == 1) {
    fprintf (stderr, "success has occurred\n");
    return 0;
  }
  else {
    fprintf (stderr, "upfuckage has occurred - %d events fired\n", events_fired);
    return 1;
  }
}

int main (void)
{
  int counter, retval, events_fired = 0;
  event_handler *eh, *eh2;
  eh = malloc (sizeof *eh);
  eh2 = malloc (sizeof *eh2);

  reactor_t = malloc (sizeof *reactor_t);
  time_t now = time(NULL);

  simple_reactor_new(reactor_t, NULL);
  test_cb_arg *eh_cb_arg2 = malloc (sizeof *eh_cb_arg2);
  eh_cb_arg2->msg = "i should never fire!";
  eh_cb_arg2->eh_to_kill = NULL;
  memset(eh2, 0, sizeof *eh2); 
  eh2->data = strdup("shouldn't fire");
  eh2->callback = test_cb;
  eh2->data_destructor_func = free;

  test_cb_arg *eh_cb_arg = malloc (sizeof *eh_cb_arg);
  eh_cb_arg->msg = "i'm so cool!";
  eh_cb_arg->eh_to_kill = eh2;

  memset(eh, 0, sizeof *eh);
  eh->callback = test_cb;
  eh->data = eh_cb_arg;
  eh->data_destructor_func = free;

  fprintf (stderr, "%s and %s started at %d\n",
    (char *)eh_cb_arg->msg, (char *) eh_cb_arg2->msg, (int)now);
  reactor_t->start_timer(reactor_t, eh2, 7);
  reactor_t->start_timer(reactor_t, eh, 5);
  reactor_t->start(reactor_t);
   
  exit (run_results());
}
