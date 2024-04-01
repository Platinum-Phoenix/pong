#ifndef STATE_H
#define STATE_H

#include "audio/audio.h"
#include "game.h"
#include "gfx/shader.h"
#include "gfx/text.h"
#include "gfx/window.h"

typedef enum {
    STATE_MENU,
    STATE_ACTIVE,
    STATE_PAUSE,
    STATE_END,
} GameState;

typedef enum {
    PLAYER_NONE,
    PLAYER_1,
    PLAYER_2,
} Winner;

typedef enum {
    SHADER_2D,
    SHADER_TEXT,
} ShaderType;

#define SHADER_LAST SHADER_TEXT
#define SHADER_COUNT SHADER_LAST + 1

struct State {
    TextRenderer text_renderer;
    AudioEngine audio_engine;
    Window window;
    Camera camera;
    Paddle player1, player2;
    Ball ball;
    Shader shaders[SHADER_COUNT];
    GameState game_state;
    Winner winner;
    bool running;
};

extern struct State state;

#endif
