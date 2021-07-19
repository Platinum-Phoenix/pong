#include "util/util.h" 
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void *alloc(size_t sz) {
    void *tmp = malloc(sz);
    if (!tmp) {
        error("[misc] error: failed to allocate %d bytes of memory", sz);
    }
    return tmp;
}

void error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputs("\n", stderr);
    if (errno != 0) {
        fprintf(stderr, "    >>> [os] error(code:%d): %s\n", errno, strerror(errno));
    }
}

bool rects_collide(vec2s r1p, vec2s r1s, vec2s r2p, vec2s r2s) {
    // some beefy code :|
    f32 r1_right_edge = r1p.x + r1s.x;
    f32 r1_left_edge = r1p.x;
    f32 r1_top_edge = r1p.y + r1s.y;
    f32 r1_bottom_edge = r1p.y;

    f32 r2_right_edge = r2p.x + r2s.x;
    f32 r2_left_edge = r2p.x;
    f32 r2_top_edge = r2p.y + r2s.y;
    f32 r2_bottom_edge = r2p.y;

    if (r1_right_edge >= r2_left_edge && // r1 right edge past r2 left
        r1_left_edge <= r2_right_edge && // r1 left edge past r2 right
        r1_top_edge >= r2_bottom_edge && // r1 top edge past r2 bottom
        r1_bottom_edge <= r2_top_edge) { // r1 bottom edge past r2 top
        return true;
    }

    return false;
}

float rand_float(float min, float max) {
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}
