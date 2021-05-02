#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdnoreturn.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "types.h"
#include "ivec2s.h"

noreturn void panic(const char* fmt, ...);

#endif
