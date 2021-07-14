#ifndef TEXT_H
#define TEXT_H

#include "cglm/types-struct.h"
#include "gfx/gfx.h"
#include "util/util.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Char {
    unsigned int texture;
    ivec2s size;
    ivec2s bearing;
    unsigned int advance;
};

struct TextRenderer {
    struct Vao vao;
    struct Bo vbo;
    struct Char chars[CHAR_MAX];
};

int text_renderer_init(struct TextRenderer *self);
void render_text(struct TextRenderer *renderer, const char *text, vec2s pos,
                 f32 scale, vec3s color);
#endif
