#include "audio/audio.h"
#include "portaudio.h"
#include "utils.h"

static int pa_stream_callback(const void *inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo *timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData)
{
    float *out = (float *)outputBuffer;
    unsigned int i;
    (void)inputBuffer;

    return 0;
}
uint8_t audio_module_init(void)
{
    int exit_code = AUDIO_NO_ERR;

    PaError err = Pa_Initialize();
    CHECK_PA_ERR(err, "failed to initialize PortAudio");

cleanup:
    return exit_code;
}

uint8_t audio_module_deinit(void)
{
    int exit_code = AUDIO_NO_ERR;

    PaError err = Pa_Terminate();
    CHECK_PA_ERR(err, "failed to deinitialize PortAudio");
cleanup:
    return exit_code;
}

uint8_t stream_create(stream_t **result)
{
    uint8_t exit_code = AUDIO_NO_ERR;
    IS_NULL(result, AUDIO_ERR_INVALARG, "AUDIO");

cleanup:
    return exit_code;
}

uint8_t stream_destroy(stream_t *st)
{
    uint8_t exit_code = AUDIO_NO_ERR;

cleanup:
    return exit_code;
}

uint8_t stream_play(stream_t *st, sound_t *sn)
{
    uint8_t exit_code = AUDIO_NO_ERR;

cleanup:
    return exit_code;
}

uint8_t stream_pause(stream_t *st)
{
    uint8_t exit_code = AUDIO_NO_ERR;

cleanup:
    return exit_code;
}
