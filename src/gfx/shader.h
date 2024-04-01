#ifndef SHADER_H
#define SHADER_H

#include "gfx.h"

typedef struct {
    unsigned int handle;
} Shader;

int shader_init(Shader* self, const char* vs_path, const char* fs_path);
void shader_destroy(Shader self);
void shader_use(Shader self);
void shader_uniform_vec3(Shader self, const char* name, vec3s v);
void shader_uniform_mat3(Shader self, const char* name, mat3s m);
void shader_uniform_mat4(Shader self, const char* name, mat4s m);
#endif
