#ifndef MODULES_RENDERER_H
#define MODULES_RENDERER_H

// clang-format off
#include "dyarr/dyarr.h"
#define RENDERER_NO_ERR         0 // no error
#define RENDERER_ERR_ALLOC      1 // error allocating memory
#define RENDERER_ERR_GLADFAIL   2 // glad error occured
#define RENDERER_ERR_INVALARG   3 // null/bad argument

#define FLOATS_PER_VERTEX    11
#define MAX_FRAME_VERTICES  1000000
#define MAX_FRAME_FLOATS    (FLOATS_PER_VERTEX * MAX_FRAME_VERTICES)
#define MAX_FRAME_INDICES  (MAX_FRAME_VERTICES * 3)
// clang-format on

#include "libraries.h"
#include RENDERER_SHADER_INCLUDE
#include <stdint.h>

// Int representation of color.
typedef struct colori colori_t;

// Float representation of color, tiny bit faster to use
// since there's no division needed to transform
typedef struct colorf colorf_t;

typedef struct vertex vertex_t;

// Holds position and other information for the viewport
typedef struct camera camera_t;

// Holds renderer information such as color and buffer objects.
typedef struct renderer renderer_t;

/**
 * Initializes gl loader with the procedure address getter argument.
 * Returns RENDERER_NO_ERR on success, 0< on failure.
 *
 * Important note : THIS WILL FAIL IF NO WINDOW WAS CREATED BEFOREHAND
 * IF IT RETURNS RENDERER_ERR_GLADFAIL CHECK THE FOLLOWING :
 * 1 - if you manually sat the context major and minor versions make sure
 * they match with the provided glad headers
 *
 * 2 - check if a window is created beforehand and if there's a valid gl
 * context
 *
 * 3 - check if the provided glad headers include extra extensions
 *
 * 4 - check if your drivers are compatible with given glad version
 */
uint8_t renderer_module_init(void *(*get_proc_address_func)(const char *));

/**
 * Initializes a renderer to the result pointer.
 * Returns RENDERER_NO_ERR on success, 0< on failure.
 */
uint8_t renderer_create(renderer_t **result, const char *vert_path,
                        const char *frag_path);

/**
 * Sets the renderer's colors to the specified channel values.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_colori(renderer_t *renderer, uint8_t r, uint8_t g,
                        uint8_t b, uint8_t a);

/**
 * Sets the renderer's colors to the specified channel values.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_colorf(renderer_t *renderer, float r, float g, float b,
                        float a);

/**
 * Clears the screen with the renderer's color.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_clear(renderer_t *renderer);

/**
 * Sets renderer viewport.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_set_viewport(renderer_t *renderer, int32_t x, int32_t y,
                              int32_t width, int32_t height);

// Binds GPU buffers and objects and resets vertex/index arrays.
// Returns RENDERER_NO_ERR on success, 0< otherwise.
uint8_t renderer_draw_begin(renderer_t *renderer);

// Puts rectangle shaped vertices in target renderer's vertex/index array
// Returns RENDERER_NO_ERR on success, 0< otherwise.
uint8_t renderer_draw_rect(renderer_t *renderer, int32_t x, int32_t y,
                           uint32_t width, uint32_t height);

// Resets GPU buffers and objects after buffering vertex/index array
// contents to the GPU via said buffers then drawing.
// Returns RENDERER_NO_ERR on success, 0< otherwise.
uint8_t renderer_draw_end(renderer_t *renderer);

/**
 * Frees the renderer's memory.
 * Returns RENDERER_NO_ERR on success.
 */
uint8_t renderer_destroy(renderer_t **renderer);

#endif
