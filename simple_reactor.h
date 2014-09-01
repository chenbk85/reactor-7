#include <stdlib.h>
#include <time.h>
#include "reactor.h"
void simple_reactor_new(reactor *self, void (*io_callback)(reactor *self));
