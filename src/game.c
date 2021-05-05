#include "game.h"
#include "cglm/struct/affine2d.h"
#include "gfx/gfx.h"
#include "gfx/shader.h"
#include "gfx/window.h"
#include "state.h"
#include "util.h"

#define PADDLE_SPEED 10.0f
#define PADDLE_WIDTH 16.0f
#define PADDLE_HEIGHT 128.0f
#define BALL_SIZE 16.0f

static void ball_init(struct Ball *self) {
    mesh_init(&self->mesh, SQUARE_VERTICES, SQUARE_VERTICES_LEN, SQUARE_INDICES,
              SQUARE_INDICES_LEN);

    self->pos = (vec2s) {{
        (state.window.size.x / 2.0f) - 8.0f,
        (state.window.size.y / 2.0f) - 8.0f
    }};
}

static void ball_render(const struct Ball *self) {
    mat3s model = glms_translate2d_make(self->pos);
    model = glms_scale2d(model, (vec2s){{BALL_SIZE, BALL_SIZE}});
    mesh_render(&self->mesh, model);
}

static void ball_destroy(const struct Ball *self) {
    mesh_destroy(&self->mesh);
}

static void paddle_init(struct Paddle *self) {
    mesh_init(&self->mesh, SQUARE_VERTICES, SQUARE_VERTICES_LEN, SQUARE_INDICES,
              SQUARE_INDICES_LEN);

    self->pos = (vec2s) {{
        PADDLE_WIDTH,
        (state.window.size.y / 2.0f) - PADDLE_HEIGHT / 2
    }};
}

static void paddle_render(const struct Paddle *self) {
    mat3s model = glms_translate2d_make(self->pos);
    model = glms_scale2d(model, (vec2s){{PADDLE_WIDTH, PADDLE_HEIGHT}});
    mesh_render(&self->mesh, model);
}

static void paddle_destroy(const struct Paddle *self) {
    mesh_destroy(&self->mesh);
}

void init(void) {
    puts("Welcome to Pong!");

    state.shader = shader_create("res/shaders/2d.vs", "res/shaders/2d.fs");
    camera_init(&state.camera);
    ball_init(&state.arena.ball);
    paddle_init(&state.arena.player); 
}

void destroy(void) {
    shader_destroy(state.shader);
    paddle_destroy(&state.arena.player);
    ball_destroy(&state.arena.ball);
    glfwDestroyWindow(state.window.handle);
    glfwTerminate();
}

void update(void) {
    update_kbd();

    if (key(GLFW_KEY_RIGHT_BRACKET).tapped) {
        state.window.wireframe = !state.window.wireframe;

        if (state.window.wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }

    if (key(GLFW_KEY_J).down) {
        state.arena.player.pos.y -= PADDLE_SPEED;
    }
    if (key(GLFW_KEY_K).down) {
        state.arena.player.pos.y += PADDLE_SPEED;
    }
}

void render(void) {
    glClearColor(0.9, 0.4, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    ball_render(&state.arena.ball);
    paddle_render(&state.arena.player);
}
