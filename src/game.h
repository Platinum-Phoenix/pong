#ifndef GAME_H
#define GAME_H

#include "gfx/gfx.h"

struct Paddle {
    vec2s pos;
    struct Mesh mesh;
};

struct Ball {
    vec2s pos;
    struct Mesh mesh;
};

struct Arena {
    struct Paddle player;
    struct Ball ball;
};

void init(void);
void update(void);
void render(void);
void destroy(void);

#endif
