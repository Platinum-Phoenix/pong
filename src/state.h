#ifndef STATE_H
#define STATE_H

#include "gfx/gfx.h"
#include "gfx/window.h"
#include "gfx/shader.h"

struct State {
   struct Window window; 
   struct Shader shader;
   struct Vao vao;
   struct Vbo vbo;
};

extern struct State state;

#endif
