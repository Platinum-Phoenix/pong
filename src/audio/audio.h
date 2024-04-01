#ifndef AUDIO_H
#define AUDIO_H

#include <al.h>
#include <alut.h>

typedef enum {
    SOUND_BEEP,
    SOUND_BOUNCE,
    SOUND_WIN,
    SOUND_LOSE,
} Sound;

#define LAST_SOUND SOUND_LOSE
#define NUM_SOUNDS LAST_SOUND + 1

typedef struct {
    ALuint audio_buffers[NUM_SOUNDS];
    ALuint audio_sources[NUM_SOUNDS];
} AudioEngine;

int audio_engine_init(AudioEngine *self);
int audio_engine_play_sound(AudioEngine *self, Sound sound);

#endif
