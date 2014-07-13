#include <stdlib.h>
#include "event_handler.h"

void event_handler_free (event_handler *handler)
{
  if (handler->data_destructor_func) {
    handler->data_destructor_func(handler->data); 
  }
  /* no destructor func? you must not want us to free it here! */
  free (handler);
}
