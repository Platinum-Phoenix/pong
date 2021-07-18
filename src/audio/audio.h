#ifndef AUDIO_H
#define AUDIO_H

#include "al.h"
#include "alut.h"
enum Sound {
    SOUND_BEEP,
    SOUND_BOUNCE,
};

#define LAST_SOUND SOUND_BOUNCE
#define NUM_SOUNDS LAST_SOUND + 1

struct AudioEngine {
    ALuint audio_buffers[NUM_SOUNDS];
    ALuint audio_sources[NUM_SOUNDS];
};

int audio_engine_init(struct AudioEngine *self);
int audio_engine_play_sound(struct AudioEngine *self, enum Sound sound);

#endif

