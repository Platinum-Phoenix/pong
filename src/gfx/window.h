#ifndef WINDOW_H
#define WINDOW_H

#include "gfx.h"
#include "util/util.h"

#define key(k) (state.window.kbd[k])

typedef struct {
    // Whether the key is held down
    bool down;
    // The value of `down` from the last frame
    bool last;
    // Whether the key has been pressed **once**
    bool tapped;
} Key;

typedef int (*WinFn)(void);

typedef struct {
    GLFWwindow *handle;
    // Keyboard
    Key kbd[GLFW_KEY_LAST];
    ivec2s size;
    WinFn init, destroy, update, render;

    u64 frames, fps;
    f64 last_second;
    bool wireframe;
} Window;

int window_create(void);
void update_kbd(void);

#endif
