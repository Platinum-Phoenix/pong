#include "game.h"
#include "audio/audio.h"
#include "cglm/util.h"
#include "gfx/shader.h"
#include "gfx/text.h"
#include "gfx/window.h"
#include "state.h"
#include "util/util.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PADDLE_SPEED 8.0f
#define PADDLE_WIDTH 16.0f
#define PADDLE_HEIGHT 128.0f
#define BALL_WIDTH 16.0f
#define BALL_HEIGHT 16.0f
#define BALL_SIZE                                                              \
    (vec2s) {                                                                  \
        { BALL_WIDTH, BALL_HEIGHT }                                            \
    }
#define PADDLE_SIZE                                                            \
    (vec2s) {                                                                  \
        { PADDLE_WIDTH, PADDLE_HEIGHT }                                        \
    }
#define BALL_SPEED_INIT 10.0f
enum Side { SIDE_LEFT, SIDE_RIGHT };

static void ball_init(struct Ball *self) {
    mesh_init(&self->mesh, SQUARE_VERTICES, SQUARE_VERTICES_LEN, SQUARE_INDICES,
              SQUARE_INDICES_LEN);

    self->pos = (vec2s){{(state.window.size.x / 2.0f) - BALL_WIDTH / 2,
                         (state.window.size.y / 2.0f) - BALL_HEIGHT / 2}};
    self->speed = BALL_SPEED_INIT;
    self->dir = 0.0f;
}

static void ball_render(const struct Ball *self) {
    mat3s model = glms_translate2d_make(self->pos);
    model = glms_scale2d(model, BALL_SIZE);
    mesh_render(&self->mesh, model);
}

static void ball_destroy(const struct Ball *self) { mesh_destroy(&self->mesh); }

static void paddle_init(struct Paddle *self, enum Side side) {
    mesh_init(&self->mesh, SQUARE_VERTICES, SQUARE_VERTICES_LEN, SQUARE_INDICES,
              SQUARE_INDICES_LEN);

    switch (side) {
    case SIDE_LEFT:
        self->pos.x = PADDLE_WIDTH;
        self->pos.y = (state.window.size.y / 2.0f) - (PADDLE_HEIGHT / 2);
        break;
    case SIDE_RIGHT:
        self->pos.x = state.window.size.x - (PADDLE_WIDTH * 2);
        self->pos.y = (state.window.size.y / 2.0f) - (PADDLE_HEIGHT / 2);
        break;
    }
}

static void paddle_render(const struct Paddle *self) {
    mat3s model = glms_translate2d_make(self->pos);
    model = glms_scale2d(model, PADDLE_SIZE);
    mesh_render(&self->mesh, model);
}

static void paddle_destroy(const struct Paddle *self) {
    mesh_destroy(&self->mesh);
}

int init(void) {
    state.game_state = STATE_MENU;
    state.running = true;
    state.winner = PLAYER_NONE;
    // set the random seed
    srand((unsigned long)time(NULL));

    puts("PONG v0 by Phoenix");
    state.player1.score = 0;
    state.player2.score = 0;
    if (shader_init(&state.shaders[SHADER_2D], "res/shaders/2d.vert",
                    "res/shaders/2d.frag") == ERR) {
        error("[shader] error: failed to initialize the 2D shader");
        return ERR;
    }
    if (shader_init(&state.shaders[SHADER_TEXT], "res/shaders/text.vert",
                    "res/shaders/text.frag") == ERR) {
        error("[shader] error: failed to initialize the text shader");
        return ERR;
    }

    audio_engine_init(&state.audio_engine);

    // for font rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if (text_renderer_init(&state.text_renderer) == ERR) {
        error("[text_renderer] error: failed to initialize");
        return ERR;
    }
    // initialize game objects
    camera_init(&state.camera);
    ball_init(&state.ball);
    paddle_init(&state.player1, SIDE_LEFT);
    paddle_init(&state.player2, SIDE_RIGHT);

    return 0;
}

int destroy(void) {
    for (size_t i = 0; i < SHADER_LAST; ++i)
        shader_destroy(state.shaders[i]);

    paddle_destroy(&state.player1);
    paddle_destroy(&state.player2);
    ball_destroy(&state.ball);
    glfwDestroyWindow(state.window.handle);
    glfwTerminate();

    return OK;
}

static void process_input(void) {
    if (key(GLFW_KEY_RIGHT_BRACKET).tapped) {
        state.window.wireframe = !state.window.wireframe;

        if (state.window.wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

    } else if (key(GLFW_KEY_J).down || key(GLFW_KEY_DOWN).down) {
        if (state.player1.pos.y - PADDLE_SPEED >= 0) {
            state.player1.pos.y -= PADDLE_SPEED;
        } else {
            state.player1.pos.y = 0;
        }
    } else if (key(GLFW_KEY_K).down || key(GLFW_KEY_UP).down) {
        if (state.player1.pos.y + PADDLE_SPEED + PADDLE_HEIGHT <=
            (f32)state.window.size.y) {
            state.player1.pos.y += PADDLE_SPEED;
        } else {
            state.player1.pos.y = (f32)state.window.size.y - PADDLE_HEIGHT;
        }
    } else if (key(GLFW_KEY_ESCAPE).down) {
        state.running = false;
    }
}
int update(void) {
    update_kbd();
    if (state.game_state == STATE_MENU) {
        if (key(GLFW_KEY_ENTER).tapped) {
            state.game_state = STATE_ACTIVE;
            audio_engine_play_sound(&state.audio_engine, SOUND_BEEP);
        }
        return OK;
    }

    if (state.game_state == STATE_END) {
        if (key(GLFW_KEY_ENTER).tapped) {
            state.game_state = STATE_ACTIVE;
            state.running = false;
        }
        return OK;
    }

    if (state.player1.score >= 11) {
        state.game_state = STATE_END;
        state.winner = PLAYER_1;
        audio_engine_play_sound(&state.audio_engine, SOUND_WIN);
    } else if (state.player2.score >= 11) {
        state.game_state = STATE_END;
        state.winner = PLAYER_2;
        audio_engine_play_sound(&state.audio_engine, SOUND_LOSE);
    }

    // when the ball collides with the paddle, I use where it lands to
    // change where it will bounce.
    // On the walls, I reflect the angle
    if (rects_collide(state.player1.pos, PADDLE_SIZE, state.ball.pos,
                      BALL_SIZE)) {
        f32 land_dist_y =
            (state.player1.pos.y + PADDLE_HEIGHT / 2) - state.ball.pos.y;
        f32 norm_land_dist_y = (land_dist_y / (PADDLE_HEIGHT / 2));
        state.ball.dir = norm_land_dist_y * glm_rad(75);
        state.ball.speed *= 1.05f;
        audio_engine_play_sound(&state.audio_engine, SOUND_BOUNCE);
    } else if (rects_collide(state.player2.pos, PADDLE_SIZE, state.ball.pos,
                             BALL_SIZE)) {
        f32 land_dist_y =
            (state.player2.pos.y + PADDLE_HEIGHT / 2) - state.ball.pos.y;
        f32 norm_land_dist_y = (land_dist_y / (PADDLE_HEIGHT / 2));
        state.ball.dir = -GLM_PI - (norm_land_dist_y * glm_rad(75));
        state.ball.speed *= 1.05f;
        state.ball.pos.x = state.player2.pos.x - PADDLE_WIDTH * 2;
        audio_engine_play_sound(&state.audio_engine, SOUND_BOUNCE);
    } else if (state.ball.pos.x <= 0) {
        // hit the left; player2 score increases
        state.player2.score += 1;
        // bounce
        state.ball.dir = -GLM_PI - state.ball.dir;
        // reset the ball's speed
        state.ball.speed = BALL_SPEED_INIT;
        state.ball.pos.x = BALL_WIDTH;
        audio_engine_play_sound(&state.audio_engine, SOUND_BEEP);
    } else if (state.ball.pos.x + BALL_WIDTH >= (f32)state.window.size.x) {
        // hit the right wall; player1 score increases
        state.player1.score += 1;
        // bounce
        state.ball.dir = -GLM_PI - state.ball.dir;
        // reset the ball's speed
        state.ball.speed = BALL_SPEED_INIT;
        state.ball.pos.x = (f32)state.window.size.x - BALL_WIDTH;
        audio_engine_play_sound(&state.audio_engine, SOUND_BEEP);
    } else if (state.ball.pos.y + BALL_HEIGHT >= (f32)state.window.size.y) {
        // top wall
        state.ball.dir = -state.ball.dir;
        state.ball.speed += 0.1f;
        state.ball.pos.y = (f32)state.window.size.y - BALL_HEIGHT;
        audio_engine_play_sound(&state.audio_engine, SOUND_BOUNCE);
    } else if (state.ball.pos.y <= 0) {
        // bottom wall
        state.ball.dir = -state.ball.dir;
        state.ball.speed += 0.1f;
        state.ball.pos.y = 0;
        audio_engine_play_sound(&state.audio_engine, SOUND_BOUNCE);
    }

    state.ball.pos.x += state.ball.speed * cos(state.ball.dir);
    state.ball.pos.y += state.ball.speed * sin(state.ball.dir);

    f32 pad_cen = state.player2.pos.y + PADDLE_HEIGHT / 2;
    f32 ball_cen = state.ball.pos.y + BALL_HEIGHT / 2;

    // only move when the ball is past the halfway mark
    if (state.ball.pos.x > (f32)state.window.size.x / 2) {
        if (ball_cen - pad_cen > PADDLE_HEIGHT / 3.0f) {
            if (state.player2.pos.y + PADDLE_SPEED + PADDLE_HEIGHT <=
                (f32)state.window.size.y) {
                state.player2.pos.y += PADDLE_SPEED;
            } else {
                state.player2.pos.y = (f32)state.window.size.y - PADDLE_HEIGHT;
            }
        } else if (ball_cen - pad_cen < -PADDLE_HEIGHT / 3.0f) {
            if (state.player2.pos.y - PADDLE_SPEED >= 0) {
                state.player2.pos.y -= PADDLE_SPEED;
            } else {
                state.player2.pos.y = 0;
            }
        }
    }

    process_input();

    return OK;
}

static void render_game(void) {
    char score[8];
    snprintf(score, sizeof(score), "PTS: %d", state.player1.score);
    render_text(&state.text_renderer, score, (vec2s){{50.0f, 552.0f}}, 0.7f,
                (vec3s){{1.0f, 1.0f, 1.0f}});
    snprintf(score, sizeof(score), "PTS: %d", state.player2.score);
    render_text(&state.text_renderer, score, (vec2s){{550.0f, 552.0f}}, 0.7f,
                (vec3s){{1.0f, 1.0f, 1.0f}});
    ball_render(&state.ball);
    paddle_render(&state.player1);
    paddle_render(&state.player2);
}

static void render_menu(void) {
    render_text(&state.text_renderer, "PONG", (vec2s){{325.0f, 525.0f}}, 1.0f,
                (vec3s){{1.0f, 1.0f, 1.0f}});

    render_text(&state.text_renderer, "PRESS <ENTER> TO START",
                (vec2s){{200.0f, 300.0f}}, 0.5f, (vec3s){{1.0f, 1.0f, 1.0f}});
}

void render_winner(void) {
    char text[36];
    snprintf(text, sizeof(text), "CONGRATULATIONS, PLAYER %d. YOU WIN!",
             state.winner);

    render_text(&state.text_renderer, text, (vec2s){{60.0f, 300.0f}}, 0.5f,
                (vec3s){{1.0f, 1.0f, 1.0f}});
}

int render(void) {
    glClearColor(0.9, 0.4, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (state.game_state) {
    case STATE_MENU:
        render_menu();
        break;
    case STATE_ACTIVE:
        render_game();
        break;
    case STATE_END:
        render_winner();
        break;
    }

    return OK;
}
