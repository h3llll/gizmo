#include "audio/audio.h"
#include "portaudio.h"
#include "utils.h"

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

uint8_t stream_create(stream **result)
{
    uint8_t exit_code = AUDIO_NO_ERR;
    
cleanup:
    return exit_code;
}

uint8_t stream_destroy(stream *st)
{
    uint8_t exit_code = AUDIO_NO_ERR;

cleanup:
    return exit_code;
}

uint8_t stream_play(stream *st, sound *sn)
{
    uint8_t exit_code = AUDIO_NO_ERR;

cleanup:
    return exit_code;
}

uint8_t stream_pause(stream *st)
{
    uint8_t exit_code = AUDIO_NO_ERR;

cleanup:
    return exit_code;
}
