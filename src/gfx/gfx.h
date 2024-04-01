#ifndef GFX_H
#define GFX_H

#include "util/util.h"
#include <glad/gl.h>
#include <stddef.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Buffer Object
// Some common ones are GL_ARRAY_BUFFER and GL_ELEMENT_ARRAY_BUFFER
typedef struct {
    unsigned int handle;
    // The buffer type e.g. GL_ARRAY_BUFFER
    GLenum type;
    // Usage e.g. GL_STATIC_DRAW
    GLenum usage;
} Bo;

void bo_init(Bo* self, GLenum type, GLenum usage);
void bo_bind(const Bo* self);
void bo_data(const Bo* self, const void* data, size_t size);
void bo_destroy(const Bo* self);

// Vertex Array Object
// A VAO stores:
// Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
// Vertex attribute configurations via glVertexAttribPointer.
// Vertex buffer objects associated with vertex attributes by calls to
// glVertexAttribPointer.
typedef struct {
    unsigned int handle;
} Vao;

Vao vao_create(void);
void vao_bind(Vao self);
void vao_unbind(void);
void vao_destroy(Vao self);
void vao_attr(unsigned int idx, int size, GLenum type, GLsizei stride,
              size_t offset);

typedef struct {
    mat4s view, proj;
} Camera;

void camera_init(Camera* self);

typedef struct {
    Vao vao;
    Bo ebo, vbo;
    size_t indices_len;
} Mesh;

#define SQUARE_VERTICES_LEN 8
#define SQUARE_INDICES_LEN 6
extern const f32 SQUARE_VERTICES[];
extern const u32 SQUARE_INDICES[];

void mesh_init(Mesh* self, const f32* vertices, size_t vertex_count,
               const u32* indices, size_t index_count);
/// model: transformation matrix
void mesh_render(const Mesh* self, mat3s model);
void mesh_destroy(const Mesh* self);

#endif
