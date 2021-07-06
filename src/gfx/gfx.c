#include "gfx.h"
#include "state.h"
#include <stdlib.h>
#include <string.h>

struct Bo bo_create(GLenum type, GLenum usage) {
    struct Bo self;
    self.type = type;
    self.usage = usage;
    glGenBuffers(1, &self.handle);
    return self;
}

void bo_bind(struct Bo self) { glBindBuffer(self.type, self.handle); }

void bo_destroy(struct Bo self) { glDeleteBuffers(1, &self.handle); }

void bo_data(struct Bo self, const void *data, size_t size) {
    glBufferData(self.type, size, data, self.usage);
}

struct Vao vao_create(void) {
    struct Vao self;
    glGenVertexArrays(1, &self.handle);
    return self;
}

void vao_bind(struct Vao self) { glBindVertexArray(self.handle); }

void vao_destroy(const struct Vao self) { glDeleteVertexArrays(1, &self.handle); }

void vao_attr(u32 idx, int size, GLenum type, GLsizei stride, size_t offset) {
    glVertexAttribPointer(idx, size, type, GL_FALSE, stride, (void *)offset);
    glEnableVertexAttribArray(idx);
}

void camera_init(struct Camera *self) {
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


void mesh_init(struct Mesh *self, const f32 *vertices, size_t vertices_len,
               const u32 *indices, size_t indices_len) {
    const u32 position_attr = 0;
    const u32 components = 2;
    self->indices_len = indices_len;

    self->vao = vao_create();
    vao_bind(self->vao);

    self->vbo = bo_create(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
    bo_bind(self->vbo);
    bo_data(self->vbo, vertices, vertices_len * sizeof(*vertices));

    self->ebo = bo_create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
    bo_bind(self->ebo);
    bo_data(self->ebo, indices, indices_len * sizeof(*indices));

    vao_attr(position_attr, sizeof(*vertices), GL_FLOAT,
             components * sizeof(*vertices), 0);
}

void mesh_render(const struct Mesh *self, mat3s model) {
    shader_use(state.shader);
    shader_uniform_mat3(state.shader, "model", model);
    shader_uniform_mat4(state.shader, "view", state.camera.view);
    shader_uniform_mat4(state.shader, "projection", state.camera.proj);
    vao_bind(self->vao);
    glDrawElements(GL_TRIANGLES, self->indices_len, GL_UNSIGNED_INT, 0);
}

void mesh_destroy(const struct Mesh *self) {
    vao_destroy(self->vao);
    bo_destroy(self->vbo);
    bo_destroy(self->ebo);
}
