#include "gfx.h"
#include <stddef.h>

struct Vbo vbo_create(enum BufferType type, enum BufferUsage usage) {
    struct Vbo self;
    self.type = type;
    self.usage = usage;
    glGenBuffers(1, &self.handle);
    return self;
}

void vbo_bind(struct Vbo self) { glBindBuffer(self.type, self.handle); }

void vbo_destroy(struct Vbo self) { glDeleteBuffers(1, &self.handle); }

void vbo_data(struct Vbo self, void *data, size_t size) {
    glBufferData(self.type, size, data, self.usage);
}

struct Vao vao_create(void) {
    struct Vao self;
    glGenVertexArrays(1, &self.handle);
    return self;
}

void vao_bind(struct Vao self) { glBindVertexArray(self.handle); }

void vao_destroy(struct Vao self) { glDeleteVertexArrays(1, &self.handle); }

void vao_attr(unsigned int idx, int size, GLenum type, GLsizei stride,
              size_t offset) {
    glVertexAttribPointer(idx, size, type, GL_FALSE, stride, (void *) offset);
    glEnableVertexAttribArray(idx);
}
