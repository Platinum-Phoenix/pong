#ifndef SHADER_H
#define SHADER_H

#include "gfx.h"

struct Shader {
    unsigned int handle;
};

enum ShaderType {
    VertexShader = GL_VERTEX_SHADER,
    FragmentShader = GL_FRAGMENT_SHADER
};

struct Shader shader_create(const char* vs_path, const char* fs_path);
void shader_destroy(struct Shader self);
void shader_use(struct Shader self);

#endif
