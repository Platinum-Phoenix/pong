#ifndef GFX_H
#define GFX_H

#include "util/types.h"
#include <glad/gl.h>
#include <stddef.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Buffer Object
// Some common ones are GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER
struct Bo {
    unsigned int handle;
    // The buffer type e.g. GL_ARRAY_BUFFER
    GLenum type;
    // Usage e.g. GL_STATIC_DRAW
    GLenum usage;
};

struct Bo bo_create(GLenum type, GLenum usage);
void bo_bind(struct Bo self);
void bo_data(struct Bo self, const void *data, size_t size);
void bo_destroy(struct Bo self);

// Vertex Array Object
// A VAO stores:
// Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
// Vertex attribute configurations via glVertexAttribPointer.
// Vertex buffer objects associated with vertex attributes by calls to
// glVertexAttribPointer.
struct Vao {
    unsigned int handle;
};

struct Vao vao_create(void);
void vao_bind(struct Vao self);
void vao_destroy(struct Vao self);
void vao_attr(unsigned int idx, int size, GLenum type, GLsizei stride,
              size_t offset);

struct Camera {
    mat4s view, proj;
};

void camera_init(struct Camera *self);

struct Mesh {
    struct Vao vao;
    struct Bo ebo, vbo;
    size_t indices_len;
};

#define SQUARE_VERTICES_LEN 8
#define SQUARE_INDICES_LEN 6
extern const f32 SQUARE_VERTICES[];
extern const u32 SQUARE_INDICES[]; 

void mesh_init(struct Mesh *self, const f32 *vertices, size_t vertex_count,
               const u32 *indices, size_t index_count);
/// model: transformation matrix
void mesh_render(const struct Mesh *self, mat3s model);
void mesh_destroy(const struct Mesh* self);

#endif
