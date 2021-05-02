#include "gfx.h"
#include "state.h"
#include <stdio.h>

// global state
struct State state;

static void init(void) { puts("Welcome to Pong!"); }

static void destroy(void) {
    glfwDestroyWindow(state.window.handle);
    glfwTerminate();
}

static void update(void) {
    // update keys
    for (size_t key = 0; key < GLFW_KEY_LAST; key++) {
        struct Key *k = &state.window.kbd[key];
        k->tapped = k->down && !k->last;
        k->last = k->down;
    }
}

static void render(void) {
    glClearColor(0.9, 0.4, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(void) {
    window_create(init, destroy, update, render);

    state.window.init();

    while (!glfwWindowShouldClose(state.window.handle)) {
        state.window.update();
        state.window.render();

        // Move the rendered buffer onto the screen buffer
        // I beleive this is done to stop screen tear due to the whole screen
        // not being rendered at once
        glfwSwapBuffers(state.window.handle);
        // Poll the OS for events like keyboard input
        glfwPollEvents();
    }

    state.window.destroy();
    return 0;
}
