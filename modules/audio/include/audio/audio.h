#ifndef MODULES_AUDIO_H
#define MODULES_AUDIO_H

#include "dyarr/dyarr.h"
#include <AL/alc.h>
#include <stddef.h>
#include <stdint.h>

// clang-format off
#define AUDIO_NO_ERR        0
#define AUDIO_ERR_ALLOC     1
#define AUDIO_ERR_INVALARG  2
#define AUDIO_ERR_OPENAL    3
// clang-format on


// Holds listener position and velocity
typedef struct camera
{
    
} camera;

// 
typedef struct audio_state
{
    ALCdevice *dev;
    ALCcontext *ctx;
    camera *cam;
} audio_state;

// Stores sound data and pointers to streams which use such sound.
// Can be played via a stream.
typedef struct sound
{

} sound;

typedef struct stream
{

} stream;

typedef struct channel_settings
{

} channel_settings;

// Manages streams and sounds
typedef struct channel
{
    sound *sound;
    array *streams;
    channel_settings settings;

} channel;

// Initiates an audio state in result pointer.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t audio_module_init(audio_state **result);

// Creates a stream in result pointer.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_create(stream **result);

// Attaches sound to target, attach NULL to unattach current sound,
// Note that it will result in an error(AUDIO_ERR_UNATTACH_NULL) if you
// attempted to unattach a target->sound that is already unattached.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_attach(stream *target, sound *sound);

// Plays attached sound.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_play(stream *stream);

// Stops sound if playing.
// Return AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_stop(stream *stream);

// Creates a sound in result pointer.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t sound_create(sound **result);

// Deattaches sound from all streams.
// Note that you DON'T have to do that before destroying a sound, it happens
// automatically.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t sound_unattach_all(sound *sound);

// Deattaches sound from all streams and frees its data.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t sound_destroy(sound *sound);

// Frees targetted stream.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t stream_destroy(stream *stream);

// Frees targetted audio state.
// Returns AUDIO_NO_ERR on success, 0< otherwise.
uint8_t audio_module_deinit(audio_state *state);

#endif // MODULES_AUDIO_H
