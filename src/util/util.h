#ifndef UTIL_H
#define UTIL_H

#include "types.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <cglm/struct/io.h>

// prints formated output to stderr and then exits
noreturn void panic(const char *fmt, ...);
// a version of malloc that never returns null
void *alloc(size_t sz);

#endif
