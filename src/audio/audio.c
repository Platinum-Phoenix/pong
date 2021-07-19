#include "audio.h"
#include "util/util.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#define alClearErrorState alGetError

char *alGetErrorString(ALenum err) {
    switch (err) {
    case AL_NO_ERROR:
        return "no error";
    case AL_INVALID_NAME:
        return "invalid name/identifier";
    case AL_INVALID_ENUM:
        return "invalid enum value";
    case AL_INVALID_OPERATION:
        return "invalid operation";
    case AL_OUT_OF_MEMORY:
        return "out of memory";
    default:
        return "unknown error";
    }
}

int add_sound(struct AudioEngine *self, enum Sound sound, char *path) {
    int result = OK;
    ALenum err;
    ALsizei size, freq;
    ALenum format;
    ALvoid *data;
#ifdef __APPLE_CC__
    alutLoadWAVFile(path, &format, &data, &size, &freq);
#else
    Albool loop;
    alutLoadWAVFile((ALbyte *)path, &format, &data, &size, &freq, &loop);
#endif

    alBufferData(self->audio_buffers[sound], format, data, size, freq);

    if ((err = alGetError()) != AL_NO_ERROR) {
        error("[openal::alBufferData] error: %s", alGetErrorString(err));
        result = ERR;
        goto cleanup;
    }

    alClearErrorState();

    if ((err = alGetError()) != AL_NO_ERROR) {
        error("[openal::alGenSources] error: %s", alGetErrorString(err));
        result = ERR;
        goto cleanup;
    }

    alSourcef(self->audio_sources[sound], AL_PITCH, 1.0f);
    alSourcef(self->audio_sources[sound], AL_GAIN, 1.0f);
    alSource3f(self->audio_sources[sound], AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(self->audio_sources[sound], AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(self->audio_sources[sound], AL_BUFFER,
              self->audio_buffers[sound]);
    alSourcei(self->audio_sources[sound], AL_LOOPING, AL_FALSE);

cleanup:
    alutUnloadWAV(format, data, size, freq);
    return result;
}

int audio_engine_init(struct AudioEngine *self) {
    ALenum err;

    // FIXME: This should called with argc and argv
    alutInit(NULL, NULL);

    alListenerf(AL_GAIN, 1.0f);
    alListenerfv(AL_POSITION, GLM_VEC3_ZERO);
    alListenerfv(AL_VELOCITY, GLM_VEC3_ZERO);
    // clang-format off
    alListenerfv(AL_ORIENTATION, (f32[]) {
        0.0f, 0.0f, 1.0f, 
        0.0f, 1.0f, 0.0f
    });
    // clang-format on

    alClearErrorState();

    alGenBuffers(NUM_SOUNDS, self->audio_buffers);
    alGenSources(NUM_SOUNDS, self->audio_sources);

    if ((err = alGetError()) != AL_NO_ERROR) {
        error("[openal::alGenBuffers] error: %s", alGetErrorString(err));
        return ERR;
    }

    if (add_sound(self, SOUND_BEEP, "res/audio/beep.wav") != OK) {
        error("[audio_engine] error: failed to add the beep sound effect");
        return ERR;
    }

    if (add_sound(self, SOUND_BOUNCE, "res/audio/bounce.wav") != OK) {
        error("[audio_engine] error: failed to add the bounce sound effect");
        return ERR;
    }

    alSourcef(self->audio_sources[SOUND_BOUNCE], AL_GAIN, 0.5f);

    if (add_sound(self, SOUND_WIN, "res/audio/win.wav") != OK) {
        error("[audio_engine] error: failed to add the win sound effect");
        return ERR;
    }

    if (add_sound(self, SOUND_LOSE, "res/audio/lose.wav") != OK) {
        error("[audio_engine] error: failed to add the lose sound effect");
        return ERR;
    }

    alSourcef(self->audio_sources[SOUND_LOSE], AL_GAIN, 100.0f);

    if ((err = alGetError()) != AL_NO_ERROR) {
        error("[openal::alSourcef] error: %s", alGetErrorString(err));
        return ERR;
    }

    return OK;
}

int audio_engine_play_sound(struct AudioEngine *self, enum Sound sound) {
    ALenum err;

    alClearErrorState();

    alSourcePlay(self->audio_sources[sound]);

    if ((err = alGetError()) != AL_NO_ERROR) {
        error("[openal::alSourcePlay] error: %s", alGetErrorString(err));
        return ERR;
    }

    return OK;
}
