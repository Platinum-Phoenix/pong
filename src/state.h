#ifndef STATE_H
#define STATE_H

#include "gfx/gfx.h"
#include "gfx/window.h"
#include "gfx/shader.h"
#include "game.h"

struct State {
   struct Window window; 
   struct Shader shader;
   struct Camera camera;
   struct Arena arena;
};

extern struct State state;

#endif
