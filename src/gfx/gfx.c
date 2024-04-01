#include "gfx.h"
#include "state.h"
#include <stdlib.h>
#include <string.h>

void bo_init(Bo* self, GLenum type, GLenum usage) {
    self->type = type;
    self->usage = usage;
    glGenBuffers(1, &self->handle);
}

void bo_bind(const Bo* self) {
    glBindBuffer(self->type, self->handle);
}

void bo_destroy(const Bo* self) {
    glDeleteBuffers(1, &self->handle);
}

void bo_data(const Bo* self, const void* data, size_t size) {
    glBufferData(self->type, size, data, self->usage);
}

Vao vao_create(void) {
    Vao self;
    glGenVertexArrays(1, &self.handle);
    return self;
}

void vao_bind(Vao self) {
    glBindVertexArray(self.handle);
}
void vao_unbind(void) {
    glBindVertexArray(0);
}
void vao_destroy(Vao self) {
    glDeleteVertexArrays(1, &self.handle);
}

void vao_attr(u32 idx, int size, GLenum type, GLsizei stride, size_t offset) {
    glVertexAttribPointer(idx, size, type, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(idx);
}

void camera_init(Camera* self) {
    self->view = glms_mat4_identity();
    self->proj = glms_ortho(0.0f, (f32)state.window.size.x, 0.0f,
                            (f32)state.window.size.y, -1000.0f, 1000.0f);
}

/* clang-format off */
const f32 SQUARE_VERTICES[] = {
    0.0f, 1.0f, 
    1.0f, 1.0f, 
    1.0f, 0.0f, 
    0.0f, 0.0f,
};

const u32 SQUARE_INDICES[] = {
    0, 1, 2, 
    2, 3, 0 
};
/* clang-format on */

void mesh_init(Mesh* self, const f32* vertices, size_t vertices_len,
               const u32* indices, size_t indices_len) {
    const u32 position_attr = 0;
    const u32 components = 2;
    self->indices_len = indices_len;

    self->vao = vao_create();
    vao_bind(self->vao);

    bo_init(&self->vbo, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    bo_bind(&self->vbo);
    bo_data(&self->vbo, vertices, vertices_len * sizeof(*vertices));

    bo_init(&self->ebo, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    bo_bind(&self->ebo);
    bo_data(&self->ebo, indices, indices_len * sizeof(*indices));

    vao_attr(position_attr, sizeof(*vertices), GL_FLOAT,
             components * sizeof(*vertices), 0);
}

void mesh_render(const Mesh* self, mat3s model) {
    shader_use(state.shaders[SHADER_2D]);
    shader_uniform_mat3(state.shaders[SHADER_2D], "model", model);
    shader_uniform_mat4(state.shaders[SHADER_2D], "view", state.camera.view);
    shader_uniform_mat4(state.shaders[SHADER_2D], "projection",
                        state.camera.proj);
    vao_bind(self->vao);
    glDrawElements(GL_TRIANGLES, self->indices_len, GL_UNSIGNED_INT, 0);
}

void mesh_destroy(const Mesh* self) {
    vao_destroy(self->vao);
    bo_destroy(&self->vbo);
    bo_destroy(&self->ebo);
}
