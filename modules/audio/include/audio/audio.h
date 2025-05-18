#ifndef MODULES_AUDIO_H
#define MODULES_AUDIO_H

#include <stddef.h>
#include <stdint.h>

// clang-format off
#define AUDIO_NO_ERR                    0
#define AUDIO_ERR_ALLOC                 1
#define AUDIO_ERR_INVALARG              2
#define AUDIO_ERR_PORTAUDIO             3
// clang-format on

#define CHECK_PA_ERR(err, msg)                                            \
    do                                                                    \
    {                                                                     \
        if ((err) != paNoError)                                           \
        {                                                                 \
            ERR(msg ": %s", Pa_GetErrorText(err));                        \
            exit_code = AUDIO_ERR_PORTAUDIO;                              \
            goto cleanup;                                                 \
        }                                                                 \
    } while (0)

typedef float float32_t;
typedef double float64_t;

// Stores sound data.
// Can be played via a stream.
typedef struct sound
{
    void *data;
    uint32_t buffer;
    size_t size;
    int32_t freq;

} sound_t;

typedef uint8_t aud_bool_t;

// Stores stream settings and sound.
// fields:
// current_sound: either holds sound or NULL.
// state: read-only, will be overwritten and manually editing it is
// dangerous, SOUND_STATE_JUSTCREATED is default.
//
// looping: set to 1 to loop, 1 is default.
//
// gain: multiplier for volume, 1.0f is default.
//
// pitch: speed multiplier for playback, 1.0f is default.
typedef struct stream
{
    uint32_t id;
    int32_t state;
    aud_bool_t looping;
    float gain;
    float pitch;

} stream_t;

// Initiates a heap audio state in result pointer.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t audio_module_init(void);

// Frees targeted audio state.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t audio_module_deinit(void);

// Creates a heap allocated stream object into result pointer.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_create(stream_t **result);

// Frees given stream.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_destroy(stream_t *st);

// Plays given sound through stream.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_play(stream_t *st, sound_t *sn);

// Pauses stream if playing, outputs a warning if stream is requesting
// pause while not playing. Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_pause(stream_t *st);

// Stops stream completely.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_stop(stream_t *st);

// Sets loop to given value.
// 0(false) to not loop, 1(true) to loop.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_loop(stream_t *st, aud_bool_t loop);

// Sets gain (audio multiplier) to given gain value.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_set_gain(stream_t *st, float gain);

// Sets pitch (speed multiplier) to given pitch value.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_set_pitch(stream_t *st, float pitch);

// Loads given path to file into a sound object in the result pointer
// if the file had a supported audio format.
// Note that the caller gains ownership of the memory and must free it.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t soud_create(sound_t **result, const char *path);

// Frees given sound.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t sound_destroy(sound_t *sn);

#endif // MODULES_AUDIO_H
