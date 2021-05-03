#include "util.h"
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

noreturn void panic(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputs("\n", stderr);
    if (errno != 0) {
        fprintf(stderr, "os error#%d: %s", errno, strerror(errno));
    }
    exit(EXIT_FAILURE);
}

void* alloc(size_t sz) {
   void* tmp = malloc(sz);
   if (!tmp) {
       panic("[misc]: memory allocation error");
   }
   return tmp;
}
