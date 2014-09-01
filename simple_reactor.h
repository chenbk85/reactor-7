#ifndef LIBREACTOR_SIMPLEREACTOR
#define LIBREACTOR_SIMPLEREACTOR

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <time.h>
#include "reactor.h"
void simple_reactor_new(reactor *self, int (*io_callback)(reactor *self));

#ifdef __cplusplus
}
#endif
#endif
