#ifndef GAME_H
#define GAME_H

#include "gfx/gfx.h"

typedef struct {
    vec2s pos;
    u32 score;
    Mesh mesh;
} Paddle;

typedef struct {
    vec2s pos;
    // direction in radians
    f32 dir;
    f32 speed;
    Mesh mesh;
} Ball;

int init(void);
int update(void);
int render(void);
int destroy(void);

#endif
