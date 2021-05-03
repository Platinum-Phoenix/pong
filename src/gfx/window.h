#ifndef WINDOW_H
#define WINDOW_H

#include "gfx.h"
#include "../util/util.h"

struct Key {
    // Whether the key is held down
    bool down; 
    // The value of `down` from the last frame
    bool last;
    // Whether the key has been pressed **once**
    bool tapped;
};

typedef void (*WinFn)(void);

struct Window {
    GLFWwindow* handle;
    // Keyboard
    struct Key kbd[GLFW_KEY_LAST];
    ivec2s size; 
    WinFn init, destroy, update, render;
};

void window_create(void);

#endif