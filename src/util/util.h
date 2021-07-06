#ifndef UTIL_H
#define UTIL_H

#include "types.h"
#include <cglm/struct/io.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

/// @brief prints formatted output to stderr and then exits with code
/// EXIT_FAILURE
noreturn void panic(const char *fmt, ...);
/// A version of malloc that never returns null
void *alloc(size_t sz);

/// @brief Detects if two rectangles collide
/// @param r1p pos of rectangle 1
/// @param r1s size of rect 1
/// @param r2p pos of rect 2
/// @param r2s size of rect 2
bool rects_collide(vec2s r1p, vec2s r1s, vec2s r2p, vec2s r2s);

float rand_float(float min, float max);

#endif
