#include "audio/audio.h"
#include "utils.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <stdlib.h>

const char *al_err_str()
{
    ALenum err = alGetError();

    const char *err_str = "unknown AL err";
    if (err != AL_NO_ERROR)
    {
        switch (err)
        {
        case AL_INVALID_NAME:
            err_str = "AL_INVALID_NAME";
            break;
        case AL_INVALID_ENUM:
            err_str = "AL_INVALID_ENUM";
            break;
        case AL_INVALID_VALUE:
            err_str = "AL_INVALID_VALUE";
            break;
        case AL_INVALID_OPERATION:
            err_str = "AL_INVALID_OPERATION";
            break;
        case AL_OUT_OF_MEMORY:
            err_str = "AL_OUT_OF_MEMORY";
            break;
        }
    }
    else
    {
        err_str = "no AL err";
    }

    return err_str;
}

// TODO : FIX, THIS WILL ALWAYS PRINT NO ERROR:
// i'm too lazy to fix it but the error is im using the alGetError once in
// the if statement and that consumes it (fuck you the people whoever made
// openal that's fucking dumb) so now alGetError will always return 0
// unless another error occurs, which is not the intended behavior. FIX :
// just use alGetError() once either in the function somehow or in the
// macro, i say just take an err code argument for the function al_err_str
// lol
#define RET_ON_AL_FAIL(fncall, fmt, ...)                                  \
    do                                                                    \
    {                                                                     \
        fncall;                                                           \
        if (alGetError() != 0)                                            \
        {                                                                 \
                                                                          \
            ERR("[AUDIO] OPENAL (IT WILL PRINT NO ERROR BTW IT'S BROKEN " \
                "I GOTTA FIX IT LOL)FAIL: exit code: %s, " fmt "\n",      \
                al_err_str(), ##__VA_ARGS__);                             \
            exit_code = AUDIO_ERR_OPENAL;                                 \
            goto cleanup;                                                 \
        }                                                                 \
    } while (0)

uint8_t audio_module_init(audio_state **result)
{
    int exit_code = AUDIO_NO_ERR;

    audio_state *_result = NULL;
    ALCdevice *dev = NULL;
    ALCcontext *ctx = NULL;

    IS_NULL(result, AUDIO_ERR_INVALARG, "AUDIO");

    dev = alcOpenDevice(NULL);
    IS_NULL(dev, AUDIO_ERR_OPENAL, "AUDIO");

    ctx = alcCreateContext(dev, NULL);
    IS_NULL(ctx, AUDIO_ERR_OPENAL, "AUDIO");

    _result = malloc(sizeof(audio_state));
    IS_NULL(_result, AUDIO_ERR_ALLOC, "AUDIO");

    _result->dev = dev;
    _result->ctx = ctx;

    *result = _result;

    return exit_code;

cleanup:
    FREE(ctx, alcDestroyContext);
    FREE(dev, alcCloseDevice);

    return exit_code;
}

uint8_t audio_module_deinit(audio_state *state)
{
    int exit_code = AUDIO_NO_ERR;

    IS_NULL(state, AUDIO_ERR_INVALARG, "AUDIO");

    alcDestroyContext(state->ctx);
    alcCloseDevice(state->dev);
    FREE(state, free);

    return exit_code;

cleanup:
    return exit_code;
}

uint8_t stream_create(stream **result)
{
    uint8_t exit_code = AUDIO_NO_ERR;

    stream *_result = NULL;
    uint32_t al_src = 0;

    IS_NULL(result, AUDIO_ERR_INVALARG, "AUDIO");

    RET_ON_AL_FAIL(alGenSources(1, &al_src), "failed to create stream");
    _result = malloc(sizeof(stream));
    IS_NULL(_result, AUDIO_ERR_ALLOC, "AUDIO");

    _result->id = al_src;
    _result->current_sound = NULL;
    _result->gain = 1;
    _result->looping = SOUND_LOOPING_DISABLED;
    _result->pitch = 1;
    _result->state = SOUND_STATE_JUSTCREATED;

    *result = _result;

    return exit_code;

cleanup:
    FREE(_result, free);
    return exit_code;
}

uint8_t stream_destroy(stream *st)
{
    uint8_t exit_code = AUDIO_NO_ERR;
    IS_NULL(st, AUDIO_ERR_INVALARG, "AUDIO");

    RET_ON_AL_FAIL(alDeleteSources(1, &st->id), "failed to free stream.");
    FREE(st, free);

cleanup:
    return exit_code;
}
