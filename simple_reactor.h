#include <stdlib.h>
#include <time.h>
#include "reactor.h"
void simple_reactor_new(reactor *self, int (*io_callback)(reactor *self));
