#include "window.h"
#include "game.h"
#include "state.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

static void err_fn(int code, const char *desc) {
    panic("[GLFW] error(code:%d): %s", code, desc);
}

// updates the window size
static void size_fn(GLFWwindow *handle, int width, int height) {
    (void)handle;

    glViewport(0, 0, width, height);
    state.window.size = (ivec2s){{width, height}};
}

// updates the keyboard state
static void kbd_fn(GLFWwindow *handle, int key, int scancode, int action,
                   int mod) {
    (void)handle;
    (void)scancode;
    (void)mod;

    if (key < 0) {
        return;
    }

    if (action == GLFW_PRESS) {
        state.window.kbd[key].down = true;
    } else if (action == GLFW_RELEASE) {
        state.window.kbd[key].down = false;
    }
}

void update_kbd(void) {
    for (size_t key = 0; key < GLFW_KEY_LAST; key++) {
        struct Key *k = &state.window.kbd[key];
        k->tapped = k->down && !k->last;
        k->last = k->down;
    }
}

void window_create(void) {
    state.window.init = init;
    state.window.destroy = destroy;
    state.window.update = update;
    state.window.render = render;

    glfwSetErrorCallback(err_fn);

    if (!glfwInit()) {
        panic("[GLFW] error: failed to initialize");
    }

    state.window.size = (ivec2s){{WINDOW_WIDTH, WINDOW_HEIGHT}};
    state.window.wireframe = false;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    state.window.handle = glfwCreateWindow(
        state.window.size.x, state.window.size.y, "pong", NULL, NULL);

    if (!state.window.handle) {
        glfwTerminate();
        panic("[GLFW] error: failed to create window");
    }

    glfwMakeContextCurrent(state.window.handle);
    // Load OpenGL extensions and functions
    gladLoadGL(glfwGetProcAddress);

    // configure GLFW's callbacks
    glfwSetFramebufferSizeCallback(state.window.handle, size_fn);
    glfwSetKeyCallback(state.window.handle, kbd_fn);
}
