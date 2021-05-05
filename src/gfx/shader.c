#include "shader.h"
#include "util/util.h"
#include <stddef.h>
#include <stdio.h>

static unsigned int compile_shader(const char *path, GLenum type) {
    FILE *fp;
    char *txt;
    size_t sz;
    unsigned int handle;
    int sucessful;

    fp = fopen(path, "rb");
    if (!fp) {
        panic("[io] error: couldnt load shader at %s", path);
    }

    // read the whole file to `txt`
    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    txt = alloc(sz);
    fread(txt, 1, sz, fp);
    fclose(fp);

    handle = glCreateShader(type);
    glShaderSource(handle, 1, (const GLchar *const *)&txt, (const GLint *)&sz);
    free(txt);

    glCompileShader(handle);
    glGetShaderiv(handle, GL_COMPILE_STATUS, &sucessful);

    if (!sucessful) {
        char *log;
        int len;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
        log = alloc(len);
        glGetShaderInfoLog(handle, len, NULL, log);
        fprintf(stderr, "[%s shader] compliation error:\n%s",
                type == GL_VERTEX_SHADER ? "vertex" : "fragment", log);
        free(log);
        exit(1);
    }

    return handle;
}

struct Shader shader_create(const char *vs_path, const char *fs_path) {
    struct Shader self;
    int sucessful;
    unsigned int vs_handle = compile_shader(vs_path, GL_VERTEX_SHADER);
    unsigned int fs_handle = compile_shader(fs_path, GL_FRAGMENT_SHADER);
    self.handle = glCreateProgram();
    glAttachShader(self.handle, vs_handle);
    glAttachShader(self.handle, fs_handle);
    glLinkProgram(self.handle);

    glGetShaderiv(self.handle, GL_LINK_STATUS, &sucessful);

    if (!sucessful) {
        char *log;
        int len;
        glGetShaderiv(self.handle, GL_INFO_LOG_LENGTH, &len);
        log = alloc(len);
        glGetShaderInfoLog(self.handle, len, NULL, log);
        fprintf(stderr, "[shader] linkage error:\n%s", log);
        free(log);
        exit(1);
    }

    return self;
}

GLint get_uniform_location(struct Shader self, const char *name) {
    int loc = glGetUniformLocation(self.handle, name);
    if (loc < 0) {
        fprintf(stderr, "[shader] warning: %s is not a uniform in shader#%d",
                name, self.handle);
    }
    return loc;
}

void shader_use(struct Shader self) { glUseProgram(self.handle); }

void shader_uniform_mat3(struct Shader self, const char *name, mat3s m) {
    glUniformMatrix3fv(glGetUniformLocation(self.handle, name), 1, GL_FALSE,
                       (const GLfloat *)&m.raw);
}

void shader_uniform_mat4(struct Shader self, const char *name, mat4s m) {
    glUniformMatrix4fv(glGetUniformLocation(self.handle, name), 1, GL_FALSE,
                       (const GLfloat *)&m.raw);
}

void shader_uniform_vec3(struct Shader self, const char *name, vec3s v) {
    glUniform3f(glGetUniformLocation(self.handle, name), v.x, v.y, v.z);
}

void shader_destroy(struct Shader self) { glDeleteProgram(self.handle); }
