#include <stdlib.h>
#include "event_handler.h"

void event_handler_free (event_handler *handler)
{
  if (handler->data_destructor_func) {
    handler->data_destructor_func(handler->data); 
  }
  else if (handler->data) {
    free (handler->data);
  }
  handler->data = NULL;
  free (handler);
}
