#include "text.h"
#include "gfx/gfx.h"
#include "gfx/shader.h"
#include "state.h"
#include <stdio.h>
#include FT_MODULE_H

int text_renderer_init(struct TextRenderer *self) {
    FT_Library ft;
    FT_Face face;
    int status = OK;
    // initialize fonts
    if (FT_Init_FreeType(&ft)) {
        error("[freetype] error: failed to initialize");
        status = ERR;
        goto cleanup;
    }

    if (FT_New_Face(ft, "res/font.ttf", 0, &face)) {
        error("[freetype] error: failed to load font face");
        status = ERR;
        goto cleanup_ft;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < CHAR_MAX; c++) {
        unsigned int texture;
        ivec2s size, bearing;
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            error("[freetype] error: failed to character glyph `%c`", c);
            // skip this glyph
            continue;
        }

        // load the texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                     face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        size = (ivec2s){{face->glyph->bitmap.width, face->glyph->bitmap.rows}};
        bearing = (ivec2s){{face->glyph->bitmap_left, face->glyph->bitmap_top}};
        self->chars[c] =
            (struct Char){texture, size, bearing, face->glyph->advance.x};
    }

    self->vao = vao_create();
    bo_init(&self->vbo, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
    vao_bind(self->vao);
    bo_bind(&self->vbo);
    bo_data(&self->vbo, NULL, sizeof(f32) * 6 * 4);
    vao_attr(0, 4, GL_FLOAT, 4 * sizeof(f32), 0);
    vao_unbind();

    FT_Done_Face(face);
cleanup_ft:
    FT_Done_Library(ft);
cleanup:
    return status;
}

void render_text(struct TextRenderer *renderer, const char *text, vec2s pos,
                 f32 scale, vec3s color) {
    unsigned char c;
    shader_use(state.shaders[SHADER_TEXT]);
    shader_uniform_vec3(state.shaders[SHADER_TEXT], "color", color);
    shader_uniform_mat4(state.shaders[SHADER_TEXT], "projection",
                        state.camera.proj);
    vao_bind(renderer->vao);

    while ((c = *text++) != '\0') {
        struct Char ch = renderer->chars[c];
        f32 xpos = pos.x + ch.bearing.x * scale;
        f32 ypos = pos.y - (ch.size.y - ch.bearing.y) * scale;
        f32 w = ch.size.x * scale;
        f32 h = ch.size.y * scale;

        // clang-format off
        float vertices[6][4] = {
            {xpos,     ypos + h, 0.0f, 0.0f}, 
            {xpos,     ypos,     0.0f, 1.0f},
            {xpos + w, ypos,     1.0f, 1.0f},
            {xpos,     ypos + h, 0.0f, 0.0f}, 
            {xpos + w, ypos,     1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f},
        };
        // clang-format on

        glBindTexture(GL_TEXTURE_2D, ch.texture);
        bo_bind(&renderer->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // advance "cursor" for the next char
        // shift right by 6 to convert to pixels
        pos.x += (ch.advance >> 6) * scale;
    }

    vao_unbind();
}
