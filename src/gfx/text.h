#ifndef TEXT_H
#define TEXT_H

#include "cglm/types-struct.h"
#include "gfx/gfx.h"
#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct {
    unsigned int texture;
    ivec2s size;
    ivec2s bearing;
    unsigned int advance;
} Char;

typedef struct {
    Vao vao;
    Bo vbo;
    Char chars[CHAR_MAX];
} TextRenderer;

int text_renderer_init(TextRenderer *self);
void render_text(TextRenderer *renderer, const char *text, vec2s pos, f32 scale,
                 vec3s color);
#endif
