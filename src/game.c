#include "game.h"
#include "gfx/gfx.h"
#include "gfx/shader.h"
#include "state.h"
#include "util.h"

enum Attributes {
    Posistion,
};

void init(void) {
    puts("Welcome to Pong!");

    mat3 vertices = {
        {-0.5f, -0.5f, 0.0f}, 
        {0.5f, -0.5f, 0.0f}, 
        {0.0f, 0.5f, 0.0f},
    };
    
    state.shader = shader_create("res/shaders/2d.vs", "res/shaders/2d.fs");

    state.vao = vao_create();
    state.vbo = vbo_create(ArrayBuffer, StaticDraw); 
    vao_bind(state.vao);
    vbo_bind(state.vbo);
    vbo_data(state.vbo, vertices, sizeof(vertices));
    vao_attr(Posistion, sizeof(f32), GL_FLOAT, 3 * sizeof(f32), 0);
}

void destroy(void) {
    shader_destroy(state.shader);
    glfwDestroyWindow(state.window.handle);
    glfwTerminate();
}

void update(void) {}


void render(void) {
    glClearColor(0.9, 0.4, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shader_use(state.shader);
    vao_bind(state.vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
