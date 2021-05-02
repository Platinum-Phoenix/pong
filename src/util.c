#include "util.h"
#include <stdlib.h>

noreturn void panic(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}
