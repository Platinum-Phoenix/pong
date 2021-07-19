#ifndef STATE_H
#define STATE_H

#include "audio/audio.h"
#include "game.h"
#include "gfx/shader.h"
#include "gfx/text.h"
#include "gfx/window.h"

enum GameState { STATE_MENU, STATE_ACTIVE, STATE_PAUSE, STATE_END };

enum Winner { PLAYER_NONE, PLAYER_1, PLAYER_2 };

enum ShaderType {
    SHADER_2D,
    SHADER_TEXT,
};

#define SHADER_LAST SHADER_TEXT

struct State {
    struct TextRenderer text_renderer;
    struct AudioEngine audio_engine;
    struct Window window;
    struct Camera camera;
    struct Paddle player1, player2;
    struct Ball ball;
    struct Shader shaders[SHADER_LAST + 1];
    enum GameState game_state;
    enum Winner winner;
    bool running;
};

extern struct State state;

#endif
