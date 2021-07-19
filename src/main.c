#include "gfx/gfx.h"
#include "state.h"

// global state
struct State state;

int main(void) {
    window_create();

    state.window.init();

    while (!glfwWindowShouldClose(state.window.handle) && state.running) {
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
