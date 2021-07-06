#ifndef GAME_H
#define GAME_H

#include "gfx/gfx.h"

struct Paddle {
    vec2s pos;
    struct Mesh mesh;
};

struct Ball {
    vec2s pos;
    // direction in radians
    f32 dir;
    struct Mesh mesh;
};

void init(void);
void update(void);
void render(void);
void destroy(void);

#endif
