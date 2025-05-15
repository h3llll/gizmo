#include "audio/audio.h"
#include "utils.h"
#include <AL/alc.h>
#include <stdlib.h>

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
