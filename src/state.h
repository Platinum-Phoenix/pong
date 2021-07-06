#ifndef STATE_H
#define STATE_H

#include "game.h"
#include "gfx/shader.h"
#include "gfx/window.h"

struct State {
    struct Window window;
    struct Shader shader;
    struct Camera camera;
    struct Paddle player;
    struct Ball ball;
    u32 score;
    bool running;
};

extern struct State state;

#endif
