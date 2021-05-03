#ifndef _GFX_H_
#define _GFX_H_

#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

enum BufferType {
    ArrayBuffer = GL_ARRAY_BUFFER,
};

enum BufferUsage {
    // the data is set only once and used by the GPU at most a few times.
    StreamDraw = GL_STREAM_DRAW, 
    // the data is set only once and used many times. 
    StaticDraw = GL_STATIC_DRAW,
    // the data is changed a lot and used many times.
    DynamicDraw = GL_DYNAMIC_DRAW,
};

struct Vbo {
    unsigned int handle;
    enum BufferType type;
    enum BufferUsage usage;
};

struct Vbo vbo_create(enum BufferType type, enum BufferUsage usage);
void vbo_bind(struct Vbo self);
void vbo_data(struct Vbo self, void* data, size_t size);
void vbo_destroy(struct Vbo self);

// Vertex Array Object
// A VAO stores:
// Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
// Vertex attribute configurations via glVertexAttribPointer.
// Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.
struct Vao {
    unsigned int handle;
};

struct Vao vao_create(void);
void vao_bind(struct Vao self);
void vao_destroy(struct Vao self);
void vao_attr(unsigned int idx, int size, GLenum type, GLsizei stride,
              size_t offset); 
#endif
