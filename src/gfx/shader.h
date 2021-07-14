#ifndef SHADER_H
#define SHADER_H

#include "gfx.h"

struct Shader {
    unsigned int handle;
};

int shader_init(struct Shader *self, const char *vs_path, const char *fs_path);
void shader_destroy(struct Shader self);
void shader_use(struct Shader self);
void shader_uniform_vec3(struct Shader self, const char *name, vec3s v);
void shader_uniform_mat3(struct Shader self, const char *name, mat3s m);
void shader_uniform_mat4(struct Shader self, const char *name, mat4s m);
#endif
