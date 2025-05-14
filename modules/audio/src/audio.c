#include "audio/audio.h"
#include <AL/alc.h>

uint8_t audio_module_init(audio_state **result)
{
    int exit_code = AUDIO_NO_ERR;
    
    audio_state *_result = NULL;


    return exit_code;

cleanup:
    return exit_code;
}

uint8_t audio_module_deinit(audio_state *state)
{
    int exit_code = AUDIO_NO_ERR;

    return exit_code;

cleanup:
    return exit_code;
}
