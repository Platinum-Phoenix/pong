#ifndef GAME_H
#define GAME_H

#include "gfx/gfx.h"

struct Paddle {
    vec2s pos;
    u32 score;
    struct Mesh mesh;
};

struct Ball {
    vec2s pos;
    // direction in radians
    f32 dir, speed;
    struct Mesh mesh;
};

int init(void);
int update(void);
int render(void);
int destroy(void);

#endif
