#include "shader.h"
#include "util/util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static const char *shader_str(GLenum s) {
    switch (s) {
    case GL_VERTEX_SHADER:
        return "vertex";
    case GL_FRAGMENT_SHADER:
        return "fragment";
    case GL_GEOMETRY_SHADER:
        return "geometry";
    default:
        return NULL;
    }
}
/// @returns the handle to the shader or -1 on error
static int compile_shader(const char *path, GLenum type) {
    FILE *fp;
    char *txt = NULL;
    size_t sz;
    unsigned int handle;
    int sucessful, ret = -1;

    fp = fopen(path, "rb");
    if (!fp) {
        error("[io] error: couldnt load shader at %s", path);
        goto cleanup;
    }

    // read the whole file to `txt`
    if (fseek(fp, 0, SEEK_END) < 0) {
        error("[io] error: fseek() failed");
        goto cleanup_file;
    }
    if ((sz = ftell(fp)) < 0) {
        error("[io] error: ftell() failed");
        goto cleanup_file;
    }
    if (fseek(fp, 0, SEEK_SET) < 0) {
        error("[io] error: fseek() failed");
        goto cleanup_file;
    }

    if ((txt = malloc(sz)) == NULL) {
        error("[alloc] error: insufficent memory to load the shader");
        goto cleanup_file;
    }

    fread(txt, 1, sz, fp);

    if (ferror(fp)) {
        error("[io] error: fread() failed");
        goto cleanup_txt;
    }

    handle = glCreateShader(type);
    glShaderSource(handle, 1, (const GLchar *const *)&txt, (const GLint *)&sz);

    glCompileShader(handle);
    glGetShaderiv(handle, GL_COMPILE_STATUS, &sucessful);

    if (!sucessful) {
        char *log;
        int len;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
        log = malloc(len);
        if (!log) {
            error("[alloc] error: insufficent memory to load the Open GL error "
                  "log");
            goto cleanup_txt;
        }

        glGetShaderInfoLog(handle, len, NULL, log);
        error("[%s shader] compliation error:\n%s", shader_str(type), log);
        free(log);
    }

    ret = handle;

cleanup_txt:
    free(txt);
cleanup_file:
    fclose(fp);
cleanup:
    return ret;
}

int shader_init(struct Shader *self, const char *vs_path, const char *fs_path) {
    int sucessful;
    unsigned int vs_handle = compile_shader(vs_path, GL_VERTEX_SHADER);
    if (vs_handle < 0)
        return -1;
    unsigned int fs_handle = compile_shader(fs_path, GL_FRAGMENT_SHADER);
    self->handle = glCreateProgram();
    glAttachShader(self->handle, vs_handle);
    glAttachShader(self->handle, fs_handle);
    glLinkProgram(self->handle);

    glGetShaderiv(self->handle, GL_LINK_STATUS, &sucessful);

    if (!sucessful) {
        char *log;
        int len;
        glGetShaderiv(self->handle, GL_INFO_LOG_LENGTH, &len);
        log = malloc(len);
        if (!log) {
        }
        glGetShaderInfoLog(self->handle, len, NULL, log);
        fprintf(stderr, "[shader] linkage error:\n%s", log);
        free(log);
    }

    return 0;
}

GLint shader_uniform_loc(struct Shader self, const char *name) {
    int loc = glGetUniformLocation(self.handle, name);
    if (loc < 0) {
        fprintf(stderr, "[shader] warning: %s is not a uniform in shader#%d",
                name, self.handle);
    }
    return loc;
}

void shader_use(struct Shader self) { glUseProgram(self.handle); }

void shader_uniform_mat3(struct Shader self, const char *name, mat3s m) {
    glUniformMatrix3fv(shader_uniform_loc(self, name), 1, GL_FALSE,
                       (const GLfloat *)&m.raw);
}

void shader_uniform_mat4(struct Shader self, const char *name, mat4s m) {
    glUniformMatrix4fv(shader_uniform_loc(self, name), 1, GL_FALSE,
                       (const GLfloat *)&m.raw);
}

void shader_uniform_vec3(struct Shader self, const char *name, vec3s v) {
    glUniform3f(shader_uniform_loc(self, name), v.x, v.y, v.z);
}

void shader_destroy(struct Shader self) { glDeleteProgram(self.handle); }
