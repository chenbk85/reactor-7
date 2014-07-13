#define REACTOR_EVENT_CANCELLED -1
#define REACTOR_EVENT_NOEVENT   0
#define REACTOR_EVENT_SUCCESS   1
typedef struct {
  void *data; 
  // time at which the event callback should be fired
  // timers can set this in the future, immediate action events can set it to the current time  
  // set to REACTOR_EVENT_CANCELLED to cancel 
  time_t valid_at;
  void (*data_destructor_func)(void *data);
  void (*callback)(struct event_handler *arg);
} event_handler;


