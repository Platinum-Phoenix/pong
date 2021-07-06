#include "game.h"
#include "cglm/struct/io.h"
#include "cglm/struct/vec2.h"
#include "cglm/types.h"
#include "cglm/util.h"
#include "gfx/shader.h"
#include "gfx/window.h"
#include "state.h"
#include "util/util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PADDLE_SPEED 8.0f
#define PADDLE_WIDTH 16.0f
#define PADDLE_HEIGHT 100.0f
#define BALL_SIZE 12.0f
#define BALL_SPEED 8.0f

static void ball_init(struct Ball *self) {
    mesh_init(&self->mesh, SQUARE_VERTICES, SQUARE_VERTICES_LEN, SQUARE_INDICES,
              SQUARE_INDICES_LEN);

    self->pos = (vec2s){{(state.window.size.x / 2.0f) - BALL_SIZE / 2,
                         (state.window.size.y / 2.0f) - BALL_SIZE / 2}};
    self->dir = rand_float(0.0f, GLM_PI * 2);
}

static void ball_render(const struct Ball *self) {
    mat3s model = glms_translate2d_make(self->pos);
    model = glms_scale2d(model, (vec2s){{BALL_SIZE, BALL_SIZE}});
    mesh_render(&self->mesh, model);
}

static void ball_destroy(const struct Ball *self) { mesh_destroy(&self->mesh); }

static void paddle_init(struct Paddle *self) {
    mesh_init(&self->mesh, SQUARE_VERTICES, SQUARE_VERTICES_LEN, SQUARE_INDICES,
              SQUARE_INDICES_LEN);

    self->pos = (vec2s){
        {PADDLE_WIDTH, (state.window.size.y / 2.0f) - PADDLE_HEIGHT / 2}};
}

static void paddle_render(const struct Paddle *self) {
    mat3s model = glms_translate2d_make(self->pos);
    model = glms_scale2d(model, (vec2s){{PADDLE_WIDTH, PADDLE_HEIGHT - 1}});
    mesh_render(&self->mesh, model);
}

static void paddle_destroy(const struct Paddle *self) {
    mesh_destroy(&self->mesh);
}

void init(void) {
    // set the random seed
    srand(time(NULL));

    puts("Welcome to Pong!");
    state.score = 0;
    state.running = true;
    state.shader = shader_create("res/shaders/2d.vert", "res/shaders/2d.frag");

    // initialize game objects
    camera_init(&state.camera);
    ball_init(&state.ball);
    paddle_init(&state.player);
}

void destroy(void) {
    shader_destroy(state.shader);
    paddle_destroy(&state.player);
    ball_destroy(&state.ball);
    glfwDestroyWindow(state.window.handle);
    glfwTerminate();
}

void update(void) {
    update_kbd();

    // update ball position
    state.ball.pos.x += BALL_SPEED * cosf(state.ball.dir);
    state.ball.pos.y += BALL_SPEED * sinf(state.ball.dir);

    // when the ball collides with the paddle, I use where it lands to
    // change where it will bounce.
    // On the walls, I reflect the angle and add a random float for variation.
    if (rects_collide(state.player.pos, (vec2s){{PADDLE_WIDTH, PADDLE_HEIGHT}},
                      state.ball.pos, (vec2s){{BALL_SIZE, BALL_SIZE}})) {


        f32 land_dist_y =
            (state.player.pos.y + PADDLE_HEIGHT / 2) - state.ball.pos.y;
        f32 norm_land_dist_y = (land_dist_y / (PADDLE_HEIGHT / 2));
        state.ball.dir = norm_land_dist_y * glm_rad(75);
    } else if (state.ball.pos.x <= 0) {
        // hit the left; player loses
        printf("You lost!\n");
        printf("Score: %d\n", state.score);
        state.running = false;
        return;
    } else if (state.ball.pos.x + BALL_SIZE >= (f32)state.window.size.x) {
        // right wall
        state.ball.dir =
            -GLM_PI - state.ball.dir + glm_rad(rand_float(-15.0f, 15.0f));

        state.score += 100;
    } else if (state.ball.pos.y + BALL_SIZE >= (f32)state.window.size.y) {
        // top wall
        f32 variation;
        if (state.ball.dir >= glm_rad(90) && state.ball.dir < glm_rad(180)) {
            variation = glm_rad(rand_float(0.0f, 15.0f));
        } else {
            variation = glm_rad(rand_float(-15.0f, 0.0f));
        }
        state.ball.dir = -state.ball.dir + variation;
    } else if (state.ball.pos.y <= 0) {
        // bottom wall
        f32 variation;
        if (state.ball.dir >= glm_rad(90) && state.ball.dir < glm_rad(180)) {
            variation = glm_rad(rand_float(-15.0f, 0.0f));
        } else {
            variation = glm_rad(rand_float(0.0f, 15.0f));
        }
        state.ball.dir = -state.ball.dir + variation;
    }

    if (key(GLFW_KEY_RIGHT_BRACKET).tapped) {
        state.window.wireframe = !state.window.wireframe;

        if (state.window.wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    } else if (key(GLFW_KEY_J).down || key(GLFW_KEY_DOWN).down) {
        if (state.player.pos.y - PADDLE_SPEED >= 0) {
            state.player.pos.y -= PADDLE_SPEED;
        } else {
            state.player.pos.y = 0;
        }
    } else if (key(GLFW_KEY_K).down || key(GLFW_KEY_UP).down) {
        if (state.player.pos.y + PADDLE_SPEED + PADDLE_HEIGHT <=
            (f32)state.window.size.y) {
            state.player.pos.y += PADDLE_SPEED;
        } else {
            state.player.pos.y = (f32)state.window.size.y - PADDLE_HEIGHT;
        }
    }
}

void render(void) {
    glClearColor(0.9, 0.4, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    ball_render(&state.ball);
    paddle_render(&state.player);
}
