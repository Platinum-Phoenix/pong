#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdnoreturn.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "types.h"
#include "ivec2s.h"

// prints formated output to stderr and then exits
noreturn void panic(const char* fmt, ...);
// a version of malloc that never returns null
void* alloc(size_t sz);

#endif
